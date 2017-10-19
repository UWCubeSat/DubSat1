import visa

INSTRUMENT_CONFIGURATION_FOLDER = "instrument-configurations"

'''
    Collect data from instruments and provide an interface to get it
'''
class DataCollector:
    def __init__(self):
        self.resource_manager = visa.ResourceManager()

        self.instruments = []
        self.get_connected_instruments_resources()

    ''' 
        For every instrument connected, open the VISA resource and add it to
        class's list of instruments.
    '''
    def get_connected_instruments_resources(self):
        for instrument_identifier in self.resource_manager.list_resources():
            instrument_resource = self.resource_manager.open_resource(instrument_identifier)
            self.instruments.append(instrument_resource)
            print(instrument_resource.query('*IDN?'))

    '''
        Close every device's VISA session and then close the resource manager
    '''
    def close(self):
        for instrument in self.instruments:
            if instrument is not None:
                instrument.close()

        if self.resource_manager is not None:
            self.resource_manager.close()

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

