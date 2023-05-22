import os
import serial
import time
import os.path as path
import argparse
from datetime import datetime


def parse_arguments():
    parser = argparse.ArgumentParser(description="Serial port monitor.")
    parser.add_argument(
        "-p",
        "--port",
        type=str,
        help="The COM port to monitor.",
        default="COM6",
    )
    parser.add_argument(
        "-b",
        "--baudrate",
        type=int,
        help="The baudrate for the COM port.",
        default=115200,
    )
    return parser.parse_args()


def generate_log_file(comport: str, baudrate: int) -> str:
    timestamp = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    return path.join(
        path.dirname(__file__), f"logs_{comport}_{baudrate}_{timestamp}.log"
    )


def monitor(comport: str, baudrate: int, log_file: str):
    ser = None
    try:
        while 1:
            try:
                ser = serial.Serial(comport, baudrate, timeout=0.3)
                while 1:
                    line = ser.readline()
                    if line != b"":
                        line = line.decode()
                        print(line)
                        # get time to milliseconds
                        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                        with open(log_file, "a") as f:
                            f.write(f"{timestamp} {line}")
                    time.sleep(0.01)
            except serial.SerialException as e:
                print(f"Failed to open serial port: {e}")
                print("Retrying in 300ms...")
                time.sleep(0.3)
            except KeyboardInterrupt:
                print("Interrupted by user")
                break
    finally:
        if ser is not None:
            ser.close()
            print("Serial port closed")
        print("Stop Monitoring")


if __name__ == "__main__":
    print("Start Serial Monitor")
    args = parse_arguments()
    comport = args.port if args.port else "COM6"
    baudrate = args.baudrate if args.baudrate else 115200
    log_file = generate_log_file(comport, baudrate)
    monitor(comport, baudrate, log_file)
