from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')


# Function used to compute the data related to the framerate in presence of latency.
def compute_data_FPS(n, file):
    file = open(file, 'r')
    Lines = file.readlines()
    total_count = 0
    data = []
    for line in Lines:
        if total_count < n:
            line_string = line.strip()
            line_split = line_string.split("Current FPS: ")
            fps_split = line_split[1]
            fps_value = float(fps_split)
            data.append(fps_value)
            total_count += 1
        else:
            break
    rtt_average = mean(data)
    rtt_max = max(data)
    rtt_sdeviation = stdev(data)
    print("FPS - m: " + str(rtt_average) + " s: " + str(rtt_sdeviation))
    print("Maximum FPS: " + str(rtt_max))
    print("")


# Function used to plot the data obtained the measurements, to be introduced manually.
def plot_data_FPS():
    y = [7.697, 7.682, 7.680, 7.689, 7.654, 7.657, 6.621, 5.418, 4.381, 3.373]
    x = [0, 4, 8, 12, 16, 20, 40, 60, 80, 100]
    ci_l = [7.265, 7.289, 7.318, 7.395, 7.212, 7.316, 6.301, 5.037, 4.291, 3.315]
    ci_u = [8.129, 8.075, 8.043, 7.983, 8.100, 8.000, 6.940, 5.799, 4.472, 3.430]
    # Plot the sinus function
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    ax.plot(x, y, 'o-', label='Average FPS')
    ax.set_xlabel('latency (ms)', size=12)
    ax.set_ylabel('framerate (fps)', size=12)
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0)
    plt.fill_between(x, ci_l, ci_u, color='blue', alpha=0.1)
    plt.ylim([-5, 80])
    # The subject of the graph needs also to be changed in the title of the graph.
    plt.title("screenpipe FPS on variable latency")
    # Plot the confidence interval
    plt.show()


if __name__ == '__main__':
    # Set the number of seconds and the file to input
    compute_data_FPS(90, "output_TORCS_0.txt")
    plot_data_FPS()

