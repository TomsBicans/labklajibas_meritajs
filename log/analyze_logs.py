import re
import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os.path as path
import os
import sys
import numpy as np

SemanticValue = {
    # Sensor readings
    "SENSOR_HUMIDITY": 1,
    "SENSOR_TEMPERATURE": 2,
    "SENSOR_AIR_PRESSURE": 3,
    "SENSOR_ALTITUDE": 4,
    # Errors
    "ERROR_1": 50,
    "ERROR_2": 51,
    "ERROR_3": 52,
    "ERROR_4": 53,
    "ERROR_5": 54,
    # System status messages
    "SYSTEM_STATUS_1": 100,
    "SYSTEM_STATUS_2": 101,
    "SYSTEM_STATUS_3": 102,
    # Device State
    "DEVICE_STATE_FIELD1": 150,
    # Transmission Stats
    "TRANMISSION_PACKETS_SENT": 155,
    "TRANSMISSION_AVERAGE_SENDING_TIME": 156,
    "TRANSMISSION_TOTAL_BYTES_SENT": 157,
    "TRANSMISSION_TOTAL_ON_AIR_TIME": 158,
    # Receiver Stats
    "RECEIVER_TOTAL_PACKETS_RECEIVED": 160,
    "RECEIVER_TOTAL_BYTES_RECEIVED": 161,
    "RECEIVER_AVERAGE_RSSI": 162,
    "RECEIVER_AVERAGE_SNR": 163,
    # Device Information
    "DEVICE_INFORMATION_FIELD1": 165,
    "DEVICE_INFORMATION_FIELD2": 166,
    # Measurement entry
    "ENTRY_TIME": 170,
    "ENTRY_ATM_TEMPERATURE": 171,
    "ENTRY_ATM_HUMIDITY": 172,
    "ENTRY_ATM_AIR_PRESSURE": 173,
    "ENTRY_ALTITUDE": 174,
}


# Function to map a number to the field name
def number_to_field_name(number):
    for name, value in SemanticValue.items():
        if value == number:
            return name
    return None  # If no matching field name is found


def log_file_graph_loc(logfile: str):
    """Returns the path of the image for a logfile semantic value."""
    log_dir = os.path.dirname(logfile)
    logfile_basename = os.path.basename(logfile)
    logfile_name, _ = os.path.splitext(logfile_basename)
    plots_dir = os.path.join(log_dir, logfile_name)
    if not os.path.exists(plots_dir):
        os.mkdir(plots_dir)
    return plots_dir


def log_file_graph_name(plots_dir: str, semantic_value: int):
    plot_loc = os.path.join(plots_dir, f"{number_to_field_name(semantic_value)}.png")
    return plot_loc


def determine_role(filepath: str):
    fname = path.basename(filepath)
    if "monitor" in fname.lower():
        return "Monitor device"
    elif "administrator" in fname.lower():
        return "Administrator device"
    else:
        return "No role"


def main(logfile: str):
    # List to store the log entries
    log_data = parse_log_data(logfile)
    print(f"{len(log_data)} Log entries found in {logfile}")

    # Convert the list into a DataFrame
    df = pd.DataFrame(log_data)

    # Shift time so it starts at 0
    df["Timestamp"] -= df["Timestamp"].min()
    print("Total benchmark time: ", df["Timestamp"].max())

    # Get unique Semantic Values
    unique_semantic_values = df["Semantic Value"].unique()

    # Create a figure for each Semantic Value
    for semantic_value in unique_semantic_values:
        semantic_df = df[df["Semantic Value"] == semantic_value]
        fig, ax = plt.subplots(figsize=(10, 5))
        ax.plot(
            semantic_df["Timestamp"],
            semantic_df["Numeric Value"],
            label=number_to_field_name(semantic_value),
        )

        # Find minima and maxima
        min_value = semantic_df["Numeric Value"].min()
        max_value = semantic_df["Numeric Value"].max()
        min_time = semantic_df[semantic_df["Numeric Value"] == min_value][
            "Timestamp"
        ].values[0]
        max_time = semantic_df[semantic_df["Numeric Value"] == max_value][
            "Timestamp"
        ].values[0]

        # Mark minima and maxima
        ax.plot(
            min_time, min_value, marker="o", markersize=5, label="Min", color="blue"
        )
        ax.plot(max_time, max_value, marker="o", markersize=5, label="Max", color="red")

        # Annotate minima and maxima
        ax.annotate(
            str(min_value),
            (min_time, min_value),
            textcoords="offset points",
            xytext=(-10, -10),
            ha="center",
        )
        ax.annotate(
            str(max_value),
            (max_time, max_value),
            textcoords="offset points",
            xytext=(-10, 10),
            ha="center",
        )
        # Here we adjust the xticks
        ax.set_xticks(
            np.arange(
                min(semantic_df["Timestamp"]), max(semantic_df["Timestamp"]) + 1, 100
            )
        )

        role = determine_role(logfile)
        ax.set_title(
            f"({role}) Numeric Value by Timestamp for {number_to_field_name(semantic_value)}"
            f" - {len(semantic_df)} samples"
        )
        ax.set_xlabel("Timestamp (seconds)")
        ax.set_ylabel("Numeric Value")
        ax.legend()
        ax.grid(True)  # Add this line to display gridlines

        # Save the figure to a file instead of displaying it
        save_dir = log_file_graph_loc(logfile)
        fig.savefig(
            log_file_graph_name(save_dir, semantic_value)
        )  # Save the plots in a 'plots' directory
        plt.close(fig)
    return df


def parse_log_data(logfile):
    log_data = []

    # Open the log file and parse the entries
    with open(logfile, "r") as file:
        for line in file:
            match = re.search(
                r"Log (\d+): X_ID: (\d+), Target Device: (\d+), Origin Device: (\d+), Semantic Value: (\d+), Timestamp: (\d+), Numeric Value: (-?[\d\.]+), Is Float: (True|False)",
                line,
            )
            if match:
                log_data.append(
                    {
                        "Log Number": int(match.group(1)),
                        "X_ID": int(match.group(2)),
                        "Target Device": int(match.group(3)),
                        "Origin Device": int(match.group(4)),
                        "Semantic Value": int(match.group(5)),
                        "Timestamp": int(match.group(6)) / 1000,  # Convert ms to s
                        "Numeric Value": float(match.group(7))
                        if match.group(8) == "True"
                        else int(match.group(7)),
                        "Is Float": match.group(8) == "True",
                    }
                )

    return log_data


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("logfiles", nargs="*", help="The log files to be analyzed")
    args = parser.parse_args()
    logfiles = args.logfiles
    for logfile in logfiles:
        if not path.exists(logfile) and not path.isfile(logfile):
            print(f"Log file {logfile} does not exist.")
            sys.exit(1)
    all_data = pd.concat([main(f) for f in logfiles])
