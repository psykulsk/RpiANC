from numpy import genfromtxt, histogram, savetxt, column_stack
from matplotlib import pyplot as plt

file = "./charts_data/timing_prio.dat"
out_file = "hist_data.dat"

data = genfromtxt(file, delimiter='\t', dtype=None, autostrip=True, skip_header=1)

hist_data, bin_edges = histogram(data[:, 1], bins=20, range=(0.0, 1000.0))
out_data = []
out_data.append(bin_edges[0:-1])
for i in range(0, 4):
    hist_data, bin_edges = histogram(data[:, i], bins=20, range=(0.0, 1000.0))
    out_data.append(hist_data)

with open(out_file, "w") as f:
    savetxt(f, column_stack(out_data), delimiter='\t', header='bin\tcaptime\tproctime\tplaytime\ttotaltime',
            fmt=["%6i", "%6i", "%6i", "%6i", "%6i"])
# plt.plot(hist_data)
# plt.show()
print(hist_data)
pass
