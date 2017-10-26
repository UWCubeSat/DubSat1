#!/usr/bin/env python3

from data_collector import DataCollector
from data_server import DataServer
import signal

class InstrumentBridge:
    def __init__(self):
        with DataCollector() as collector:
            result_channels = collector.get_telemetry_outputs()
            with DataServer(result_channels) as transmitter:
                print("Press ctrl+c to exit")
                signal.signal(signal.SIGINT, self.exit_handler)
                # TODO signal.pause() is not available on Windows systems
                signal.pause()

    def exit_handler(self, signal, frame):
        print("\n SIGINT recieved. Please wait as we close cleanly.")

if __name__ == "__main__":
    InstrumentBridge()

