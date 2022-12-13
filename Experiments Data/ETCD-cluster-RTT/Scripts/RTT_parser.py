from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')


# Function used to compute the data related to the communication RTT.
def compute_data_RTT(n, file):
    file = open(file, 'r')
    Lines = file.readlines()
    total = 0
    total_count = 0
    partial = 0
    partial_count = 0
    data = []
    for line in Lines:
        if total < n:
            line_string = line.strip()
            line_split = line_string.split("Current RTT time: ")
            rtt_split = line_split[1].split("s")[0]
            rtt_value = float(rtt_split)
            if partial_count < 10:
                partial += rtt_value
                partial_count += 1
            else:
                data.append(partial/10)
                total_count += 1
                partial_count = 0
                partial = 0
            total += rtt_value
        else:
            break
    rtt_average = mean(data)
    rtt_max = max(data)
    rtt_sdeviation = stdev(data)
    print("RTT - m: " + str(rtt_average) + " s: " + str(rtt_sdeviation))
    print("Number of samples: " + str(total_count))
    print("Maximum RTT: " + str(rtt_max))
    print("")


if __name__ == '__main__':
    compute_data_RTT(90, "output_RTT_etcd_cluster.txt")

