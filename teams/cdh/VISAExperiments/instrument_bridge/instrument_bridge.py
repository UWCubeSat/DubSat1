#!/usr/bin/env python3

import data_collector
import data_server

class InstrumentBridge:
    def __init__(self):
        self.instrument_data_collector = data_collector.DataCollector();
        self.data_transmitter = data_server.DataServer();

    def shutdown(self):
        if self.instrument_data_collector is not None:
            self.instrument_data_collector.close()

if __name__ == "__main__":
    InstrumentBridge()

