#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

error_file = "audio_files/rec/err_mic.dat"
reference_file = "audio_files/rec/ref_mic.dat"
correction_file = "audio_files/rec/corr_sig.dat"

# Load all test results files
error_signal = np.fromfile(error_file, dtype=np.int32)
reference_signal = np.fromfile(reference_file, dtype=np.int32)
correction_signal = np.fromfile(correction_file, dtype=np.int32)

ii32 = np.iinfo(np.int32)

error_signal = error_signal.astype(np.float64) / ii32.max
reference_signal = reference_signal.astype(np.float64) / ii32.max
correction_signal = correction_signal.astype(np.float64) / ii32.max

fig, axs = plt.subplots(3, 1)
axs[0].plot(reference_signal)
axs[0].set_title('Reference signal')
axs[0].grid(True)
axs[0].set_ylim(-1.0, 1.0)
axs[1].plot(correction_signal)
axs[1].set_title('Correction signal')
axs[1].grid(True)
axs[1].set_ylim(-1.0, 1.0)
axs[2].plot(error_signal)
axs[2].set_title('Error signal')
axs[2].grid(True)
axs[2].set_ylim(-1.0, 1.0)

fig.tight_layout()
plt.show()
