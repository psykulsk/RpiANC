#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

cap_file = "audio_files/rec/cap_time.dat"
proc_file = "audio_files/rec/proc_time.dat"
play_file = "audio_files/rec/play_time.dat"

# Load all test results files
cap_time = np.fromfile(cap_file, dtype=np.long)
proc_time = np.fromfile(proc_file, dtype=np.long)
play_time = np.fromfile(play_file, dtype=np.long)

fig, axs = plt.subplots(3, 1)
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

fig.tight_layout()
plt.show()
