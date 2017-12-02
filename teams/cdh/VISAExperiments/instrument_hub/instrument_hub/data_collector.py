import datetime
import pytz
import visa
import threading
from time import sleep
from queue import Queue
from tzlocal import get_localzone

'''
    Collect data from instruments and provide an interface to get it
'''
class DataCollector:
    def __init__(self, instrument_configs):
        self.instrument_configs = instrument_configs
        self.resource_manager = visa.ResourceManager()
        self.instruments = self._get_connected_instruments_resources()
        self.telemetry_results = self._map_workers_to_outputs(self.instruments)

    def __enter__(self):
        return self

    '''
        Close every device's VISA session and then close the resource manager
    '''
    def __exit__(self, exc_type, exc_value, traceback):
        for thread in self.telemetry_results.keys():
            thread.stop()

        for instrument in self.instruments:
            if instrument is not None:
                instrument.close()

        if self.resource_manager is not None:
            self.resource_manager.close()

    def get_telemetry_outputs(self):
            result = []
            for thread, output_queue in self.telemetry_results.items():
                result.append(output_queue)
            return result
    ''' 
        For every instrument connected, open the VISA resource and return
        a list of instruments.
    '''
    def _get_connected_instruments_resources(self):
        instrument_resources = []

        for instrument_identifier in self.resource_manager.list_resources():
            instrument_resource = self.resource_manager.open_resource(instrument_identifier)
            instrument_resources.append(instrument_resource)

        return instrument_resources

    '''
        Returns a map of telemetry gathering workers to their outputs,
        which are Queues. The telemetry gathering workers are created
        and run, data is ready to be consumed from the output queues.
    '''
    def _map_workers_to_outputs(self, instruments):
        worker_to_output = {}

        for instrument in instruments:
            output = Queue()
            telemetry_thread = InstrumentWorker(instrument, output, self.instrument_configs)
            telemetry_thread.start()
            worker_to_output[telemetry_thread] = output

        return worker_to_output

class InstrumentWorker(threading.Thread):
    def __init__(self, instrument, output, telemetry_configs):
        threading.Thread.__init__(self)
        self.instrument = instrument
        self.output = output
        self.clock = datetime.datetime
        self.my_timezone = get_localzone()
        self.instrument_config = self.choose_telemetry_config(telemetry_configs)

    def choose_telemetry_config(self, telemetry_configs):
        id = self.instrument.query("*IDN?")
        id = id.split(",")
        make = id[0]
        model = id[1]
        for telemetry_config in telemetry_configs:
            if make == telemetry_config["instrument-information"]["visa-make"]:
                if model == telemetry_config["instrument-information"]["visa-model"]:
                    return telemetry_config
        raise Exception("Unable to find config file for device: " + id)

    '''
        Worker function to interface with the given instrument. Writes telemetry
        data to the queue provided. Should be run seperate thread.
    '''
    def run(self):
        self.alive = True

        counter = 0
        while self.alive:
            sleep(0.5)
            query_string = self.instrument_config["telemetry-item"][counter]["command"]
            query_result = {}
            query_result["value"] = float(self.instrument.query(query_string))
            query_result["time"] = self.clock.now(tz=self.my_timezone)
            query_result["id"] = self.instrument_config["telemetry-item"][counter]["id"]
            self.output.put(query_result)
            counter += 1
            counter %= len(self.instrument_config["telemetry-item"])

    def stop(self):
        self.alive = False
        self.join()


