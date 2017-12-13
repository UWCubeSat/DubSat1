import threading

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

    def run(self):
        self.alive = True

        while self.alive:
            reading = self.data_source.get()
            print("Reading from thread " + self.name + " :" + reading)
             
    def stop(self):
        self.alive = False
        self.join()

