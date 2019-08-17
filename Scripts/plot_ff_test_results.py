#!/usr/bin/env python3

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rcParams['font.family'] = "Times New Roman"
plt.rcParams['font.size'] = 12
plt.rcParams['font.weight'] = "light"
error_file = "audio_files/rec/err_mic.dat"
reference_file = "audio_files/rec/ref_mic.dat"
correction_file = "audio_files/rec/corr_sig.dat"
del matplotlib.font_manager.weight_dict['roman']
matplotlib.font_manager._rebuild()

# Load all test results files
error_signal = np.fromfile(error_file, dtype=np.int32)
reference_signal = np.fromfile(reference_file, dtype=np.int32)
correction_signal = np.fromfile(correction_file, dtype=np.int32)

ii32 = np.iinfo(np.int32)

error_signal = error_signal.astype(np.float64) / ii32.max
reference_signal = reference_signal.astype(np.float64) / ii32.max
correction_signal = correction_signal.astype(np.float64) / ii32.max

starti = 290000
endi = 890000
fig, axs = plt.subplots(3, 1)
xax = [x / 44100.0 for x in range(0, endi - starti)]
axs[0].plot(xax, reference_signal[starti:endi])
axs[0].set_title('Sygnał referencyjny')
axs[0].set_ylabel("Wartość sygnału")
axs[0].set_xlabel("Czas [s]")
axs[0].grid(True)
axs[0].set_ylim(-0.0450, -0.0525)
axs[1].plot(xax, correction_signal[starti:endi])
axs[1].set_title('Sygnał korekcyjny')
axs[1].grid(True)
axs[1].set_ylim(-0.13, -0.09)
axs[1].set_ylabel("Wartość sygnału")
axs[1].set_xlabel("Czas [s]")
axs[2].plot(xax, error_signal[starti:endi])
axs[2].set_title('Sygnał błędu')
axs[2].grid(True)
axs[2].set_ylim(-0.0013, 0.0013)
axs[2].set_ylabel("Wartość sygnału")
axs[2].set_xlabel("Czas [s]")

# err_array = np.asarray(error_signal[starti:endi])
# ref_array = np.asarray(reference_signal[starti:endi])
# corr_array = np.asarray(correction_signal[starti:endi])
# np.savetxt(error_file+".out", err_array, delimiter="\n")
# np.savetxt(reference_file+".out", ref_array, delimiter="\n")
# np.savetxt(correction_file+".out", corr_array, delimiter="\n")

fig.tight_layout()
plt.show()
