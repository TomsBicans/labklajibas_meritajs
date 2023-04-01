import sys
import os
import serial
import threading
import time
import os.path as path
from pathlib import Path

LOG_DIR = path.dirname(__file__)
LOG_FILE = path.join(LOG_DIR, "measurement_data.log")


def get_data_lines(data: list[str]):
    res = []
    for l in data:
        if any(p.lower() in l.lower() for p in ["OBS", "Total"]):
            continue
        res.append(l)
    return res


def get_time_lines(data: list[str]):
    res = []
    for l in data:
        if any(p.lower() in l.lower() for p in ["Total"]):
            res.append(l)
    return res


def write_to_file(loc: str, data: str):
    with open(loc, "w") as f:
        f.write(data)
    return loc


def main():
    file = Path(LOG_FILE)
    data = str(file.read_text())
    data = data.splitlines()
    data = [l for l in data if l]
    time_data = get_time_lines(data)
    # time_data =
    data = get_data_lines(data)
    [print(l) for l in data]
    data = "\n".join(data) + "\n"
    loc = path.join(LOG_DIR, "results.csv")
    write_to_file(loc, data)


if __name__ == "__main__":
    main()
