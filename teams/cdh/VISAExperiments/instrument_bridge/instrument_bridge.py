#!/usr/bin/env python3

from data_collector import DataCollector
from data_server import DataServer
import signal

class InstrumentBridge:
    def __init__(self):
        with DataCollector() as collector, DataServer() as transmitter:
            print("Press ctrl+c to exit")
            signal.signal(signal.SIGINT, self.exit_handler)
            signal.pause()

    def exit_handler(self, signal, frame):
        print("\nClosing cleanly.")

if __name__ == "__main__":
    InstrumentBridge()

