#!/usr/bin/env python3

from instrument_hub.data_collector import DataCollector
from instrument_hub.data_server import DataServer
from instrument_hub.configuration_parser import ConfigurationParser
import signal

class InstrumentBridge:
    def __init__(self):
        instrument_configurations = ConfigurationParser.get_instrument_configurations()
        with DataCollector(instrument_configurations) as collector:
            result_channels = collector.get_telemetry_outputs()
            with DataServer(result_channels) as transmitter:
                print("[i] Press ctrl+c to exit")
                signal.signal(signal.SIGINT, self.exit_handler)
                # TODO signal.pause() is not available on Windows systems
                signal.pause()

    def exit_handler(self, signal, frame):
        print("\nSIGINT recieved. Please wait as we close cleanly.")

def main():
    InstrumentBridge()
