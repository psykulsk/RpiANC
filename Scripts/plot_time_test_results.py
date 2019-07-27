#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np


def read_from_file(filename):
    with open(filename) as f:
        content = f.readlines()
    # you may also want to remove whitespace characters like `\n` at the end of each line
    content = [int(x.strip()) for x in content]
    return content


cap_file = "audio_files/rec/cap_time.dat"
proc_file = "audio_files/rec/proc_time.dat"
play_file = "audio_files/rec/play_time.dat"
total_file = "audio_files/rec/total_time.dat"

# Load all test results files
cap_time = read_from_file(cap_file)
proc_time = read_from_file(proc_file)
play_time = read_from_file(play_file)
total_time = read_from_file(total_file)

fig, axs = plt.subplots(4, 1)
axs[0].plot(proc_time)
axs[0].set_title('Proc time')
axs[0].grid(True)
# axs[0].set_ylim(-1.0, 1.0)
axs[1].plot(play_time)
axs[1].set_title('Play time')
axs[1].grid(True)
# axs[1].set_ylim(-1.0, 1.0)
axs[2].plot(cap_time)
axs[2].set_title('Cap time')
axs[2].grid(True)
# axs[2].set_ylim(-1.0, 1.0)
axs[3].plot(cap_time)
axs[3].set_title('Total time')
axs[3].grid(True)

fig.tight_layout()
plt.show()
