from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')


# Function used to parse the data for the system with a certain amount of delay in the simulation module
# or other FPS measurement files.
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

# Function used to plot the data obtained by the measurements, to be introduced manually.
def plot_data_FPS():
    y = [59.836, 59.032, 28.246, 27.653, 17.797, 13.146, 9.989, 8.153, 7.190, 6.418, 5.567]
    x = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    ci_l = [59.515, 58.541, 27.881, 27.187, 17.546, 12.748, 9.744, 8.124, 7.096, 6.340, 5.451]
    ci_u = [60.158, 59.523, 28.611, 28.119, 18.048, 13.544, 10.234, 8.181, 7.284, 6.496, 5.684]
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    ax.plot(x, y, 'o-', label='Average FPS')
    ax.set_xlabel('latency (ms)', size=12)
    ax.set_ylabel('framerate (fps)', size=12)
    ax.set_xticks(np.arange(0, len(x) + 1, 1))
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0)
    plt.fill_between(x, ci_l, ci_u, color='blue', alpha=0.1)
    plt.ylim([-5, 65])
    plt.title("TORCS FPS on sim module delay")
    plt.show()

if __name__ == '__main__':
    # Set the number of seconds and the file to input
    compute_data_FPS(90, "output.txt")
    plot_data_FPS()

