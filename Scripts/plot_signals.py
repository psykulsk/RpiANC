#!/usr/bin/env python3
import sys

import matplotlib.pyplot as plt
import numpy as np


def plot_signal_from_file(file_name):
    signal = np.fromfile(file_name, dtype=np.int32)
    plt.plot(signal)
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Not enough arguments")
        exit(1)

    for file_name in sys.argv[1:]:
        plot_signal_from_file(file_name)
