# Importing required libraries
from datetime import datetime
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from geopy.distance import geodesic
import os.path as path
from analyze_logs import log_file_graph_loc


def calculate_distances(df):
    distances = [0.0]  # start with distance of 0
    start_coords = (df.iloc[0]["Latitude"], df.iloc[0]["Longitude"])
    for i in range(1, len(df)):
        current_coords = (df.iloc[i]["Latitude"], df.iloc[i]["Longitude"])
        distance = geodesic(start_coords, current_coords).meters
        distances.append(distance)  # distance from starting point
    return distances


def plot_distances(df, output_file):
    fig, ax = plt.subplots(figsize=(10, 5))
    ax.plot(df["Relative Time"], df["Distance"], label="Distance from start point")

    # Find minima and maxima
    min_index = df["Distance"].idxmin()
    max_index = df["Distance"].idxmax()

    min_value = round(df.loc[min_index, "Distance"], 2)
    max_value = round(df.loc[max_index, "Distance"], 2)

    min_time = df.loc[min_index, "Relative Time"]
    max_time = df.loc[max_index, "Relative Time"]

    # Mark minima and maxima
    ax.plot(min_time, min_value, marker="o", markersize=5, label="Min", color="blue")
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
    x_ticks = np.arange(df["Relative Time"].min(), df["Relative Time"].max(), 100)
    ax.set_xticks(x_ticks)

    ax.set_title("Overtime Distance from Start Point")
    ax.set_xlabel("Relative Time (seconds)")
    ax.set_ylabel("Distance (meters)")
    ax.legend()
    ax.grid(True)

    fig.savefig(output_file)
    plt.close(fig)


def main(data_file):
    # Read the CSV file
    df = pd.read_csv(data_file, skiprows=3)
    df["Time"] = pd.to_datetime(df["Time"])  # convert Time column to datetime

    # Calculate relative time in seconds
    df["Relative Time"] = (df["Time"] - df["Time"][0]).dt.total_seconds()

    # Calculate distances
    df["Distance"] = calculate_distances(df)

    # Plot distances
    save_loc = log_file_graph_loc(data_file)
    plot_distances(df, path.join(save_loc, "gps_data.png"))


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("data_file", help="The GPS data file to be analyzed")
    args = parser.parse_args()
    main(args.data_file)
