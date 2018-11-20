#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

error_file_right_channel = "audio_files/err_mic_right_channel.dat"
error_file_left_channel = "audio_files/err_mic_left_channel.dat"
correction_file_right_channel = "audio_files/corr_sig_right_channel.dat"
correction_file_left_channel = "audio_files/corr_sig_left_channel.dat"

# Load all test results files
error_signal_right_channel = np.fromfile(error_file_right_channel, dtype=np.int32)
error_signal_left_channel = np.fromfile(error_file_left_channel, dtype=np.int32)
correction_signal_right_channel = np.fromfile(correction_file_right_channel, dtype=np.int32)
correction_signal_left_channel = np.fromfile(correction_file_left_channel, dtype=np.int32)

ii32 = np.iinfo(np.int32)

error_signal_right_channel = error_signal_right_channel.astype(np.float64) / ii32.max
error_signal_left_channel = error_signal_left_channel.astype(np.float64) / ii32.max
correction_signal_right_channel = correction_signal_right_channel.astype(np.float64) / ii32.max
correction_signal_left_channel = correction_signal_left_channel.astype(np.float64) / ii32.max

fig, axs = plt.subplots(2, 2)
axs[0][0].plot(error_signal_right_channel)
axs[0][0].set_title('Error signal right channel')
axs[0][0].grid(True)
axs[0][0].set_ylim(-1.0, 1.0)
axs[0][1].plot(correction_signal_right_channel)
axs[0][1].set_title('Correction signal right channel')
axs[0][1].grid(True)
axs[0][1].set_ylim(-1.0, 1.0)
axs[1][0].plot(error_signal_left_channel)
axs[1][0].set_title('Error signal left channel')
axs[1][0].grid(True)
axs[1][0].set_ylim(-1.0, 1.0)
axs[1][1].plot(correction_signal_left_channel)
axs[1][1].set_title('Correction signal left channel')
axs[1][1].grid(True)
axs[1][1].set_ylim(-1.0, 1.0)

fig.tight_layout()
plt.show()
