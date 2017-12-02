import datetime
import threading
import socketserver
from queue import Queue
import struct

PACKET_ID_LENGTH_BITS = 8
PACKET_SIZE_INDICATOR_LENGTH_BITS = 8

class DataServer:
    def __init__(self, telemetry_outputs, base_port=13000):
        self.base_port = base_port

        # start meta information server
        self.meta_instrumentation_server = MetaServerWorker(self.base_port);
        self.meta_instrumentation_server.start()

        # start worker threads; ports higher than meta server
        worker_base_port = self.base_port + 1
        self.instrumentation_servers = self._create_servers(telemetry_outputs, 
                                        worker_base_port)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        if self.meta_instrumentation_server is not None:
            self.meta_instrumentation_server.stop()

        for server in self.instrumentation_servers:
            server.stop()

    def _create_servers(self, outputs, base_port):
        all_worker_threads = []

        worker_port = base_port
        for output in outputs:
            server_thread = InstrumentationServerWorker(output, worker_port)
            server_thread.start()
            worker_port += 1
            all_worker_threads.append(server_thread)

        return all_worker_threads
'''
    Serve meta-information about instrumentation data. For example,
    what instruments were are serving and what ports they are on.
'''
class MetaServerWorker(threading.Thread):
    def __init__(self, port):
        threading.Thread.__init__(self)
        self.port = port

    def run(self):
        self.alive = True

        while self.alive:
            pass

    def stop(self):
        self.alive = False
        self.join()

'''
    Serves instrumentation data from a particular source (e.x. a specific
    multimeter in the lab)
'''
class InstrumentationServerWorker(threading.Thread):
    def __init__(self, data_source, port):
        threading.Thread.__init__(self)
        self.data_source = data_source
        self.port = port
        self.server = ThreadedTCPServer(("0.0.0.0", port), TCPHandler)
        self.server.largest_handler_id = 0;

    def run(self):
        self.alive = True

        self.server.copy_queues = dict()
        self.server_thread = threading.Thread(target=self.server.serve_forever)
        self.server_thread.start()
        print("Serving on port " + str(self.port))

        while self.alive:
            # TODO if collector thread crashes, will self.data_source become None?
            # if so, we can shut this thread off and prevent cascading crashes
            reading = self.data_source.get()
            for handler_id, worker_queue in self.server.copy_queues.items():
                worker_queue.put(reading)
             
    def stop(self):
        if self.server is not None:
            self.server.shutdown()

        if self.server_thread is not None:
            self.server_thread.join()

        self.alive = False
        self.join()

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

class TCPHandler(socketserver.BaseRequestHandler):
    def setup(self):
        self.id = self.server.largest_handler_id;
        self.server.largest_handler_id += 1;
        self.server.copy_queues[self.id] = Queue()
        self.packet_generator = CosmosLengthPacketGenerator()

    def handle(self):
        instrumentation_queue = self.server.copy_queues[self.id]
        while True:
            reading = instrumentation_queue.get()
            packet_bytes = self.packet_generator.new_packet(reading["id"], 
                reading["value"], reading["time"])

            self.request.sendall(packet_bytes)

    def finish(self):
        del self.server.copy_queues[self.id]

"""
    Packet format:
    [Packet length (not including self) in bytes | 8 bits]
    [Packet ID | 8 bits]
    [Packet timestamp, ISO 8601 | 232 bits]
    [Telemetry value, float | 32 bits]
"""
class CosmosLengthPacketGenerator:
    def __init__(self):
        self.clock = datetime.datetime;

    def new_packet(self, id, value, time):
        packet = b""

        packet_id_bytes = id.to_bytes(int(PACKET_ID_LENGTH_BITS / 8),
            byteorder='big', signed=False)
        packet += packet_id_bytes

        # time expressed as string in format exactly as '2017-10-30T13:37:39.814-07:00'
        time_ISO_8601 = time.isoformat(timespec="milliseconds")
        time_bytes = time_ISO_8601.encode('ascii')
        assert 29, len(time_bytes)
        packet += time_bytes

        value_bytes = bytearray(struct.pack(">f", value))
        assert 4, len(value_bytes)
        packet += value_bytes

        # cosmos expects packet length in bytes
        packet_length_bytes = (len(packet)+1).to_bytes(int(PACKET_SIZE_INDICATOR_LENGTH_BITS / 8),
            byteorder='big', signed=False)
        # append as suffix
        packet = packet_length_bytes + packet

        return packet
