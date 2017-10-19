import visa
import threading
from time import sleep

INSTRUMENT_CONFIGURATION_FOLDER = "instrument-configurations"

'''
    Collect data from instruments and provide an interface to get it
'''
class DataCollector:
    def __init__(self):
        self.resource_manager = visa.ResourceManager()
        self.instruments = self.get_connected_instruments_resources()
        self.instrument_listeners = self.attach_instrument_listeners(self.instruments)

    '''
        Close every device's VISA session and then close the resource manager
    '''
    def close(self):
        # check that this is not done already by resource_manager.close
        for instrument in self.instruments:
            if instrument is not None:
                instrument.close()

        if self.resource_manager is not None:
            self.resource_manager.close()

    ''' 
        For every instrument connected, open the VISA resource and return
        a list of instruments.
    '''
    def get_connected_instruments_resources(self):
        instrument_resources = []

        for instrument_identifier in self.resource_manager.list_resources():
            instrument_resource = self.resource_manager.open_resource(instrument_identifier)
            instrument_resources.append(instrument_resource)

        return instrument_resources

    '''
        Start a new process for each instrument to interface with it.
    '''
    def attach_instrument_listeners(self, instruments):
        threads = []
        for instrument in instruments:
            t = threading.Thread(target=self.instrument_listening_worker, args=(instrument,))
            t.start()
            threads.append(t)
        for thread in threads:
            thread.join()

    '''
        Interfaces with the given instrument. Designed to run on its own process.
    '''
    def instrument_listening_worker(self, instrument):
        for x in range(10):
            sleep(0.25)
            print(instrument.query("*IDN?"))

    '''
        Read configuration files in INSTRUMENT_CONFIGURATION_FOLDER
    '''
    def load_instrument_configurations(self):
        None

    '''
        Detect when instruments are inserted or removed and make neccesary changes
    '''
    def detect_instrument_changes(self):
        None

