import sys
import os
import serial
import threading
import time
import os.path as path

LOG_FILE = path.join(path.dirname(__file__), "running_logs_test.log")


def monitor(comport: str, baudrate: int):
    ser = serial.Serial(comport, baudrate, timeout=0)

    while 1:
        line = ser.readline()
        # print(line, type(line))
        if line != b"":
            # line = line.decode("ascii")
            line = line.decode()
            print(line)
            # print line[:-1]         # strip \n
            # fields = line[:-1].split('; ')

            # ID = fields[0]
            # TIME = int(fields[1])
            # print fields
            # print("device ID: ", ID)
            # write to file
            text_file = open(LOG_FILE, "a")
            # line = str(TIME) + ": " + str(CT) + "\n"
            text_file.write(line)
            text_file.close()
        time.sleep(0.01)

        # do some other things here

    print("Stop Monitoring")


if __name__ == "__main__":
    print("Start Serial Monitor")
    COMPORT = "COM6"
    BAUDRATE = 115200
    monitor(COMPORT, BAUDRATE)
