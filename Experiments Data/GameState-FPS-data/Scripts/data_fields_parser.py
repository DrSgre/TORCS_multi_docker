from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')

# Function used to parse the data for the system with a certain number of game state data fields and a certain latency.
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


# Function used to plot the data obtained the framerate measurements, to be introduced manually.
def plot_data_FPS():
    y = [6.324, 0.457, 0.184, 0.184, 0.106, 0.083]
    x = [0, 4, 8, 12, 16, 20]
    ci_l = [6.106, 0.414, 0.176, 0.125, 0.101, 0.080]
    ci_u = [6.542, 0.501, 0.192, 0.136, 0.153, 0.114]
    # Plot the sinus function
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    ax.plot(x, y, 'o-', label='Average FPS')
    ax.set_xlabel('latency (ms)', size=12)
    ax.set_ylabel('framerate (fps)', size=12)
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0)
    plt.fill_between(x, ci_l, ci_u, color='blue', alpha=0.1)
    plt.ylim([-1, 80])
    plt.title("FPS on moderate latency - 3 game state fields")
    # Plot the confidence interval
    plt.show()

# Function used to plot the data obtained by the requests measurements, to be introduced manually.
def plot_data_requests():
    y4 = [222.8, 29.2, 20.2, 18.2, 15.0, 11.7]
    x = [0, 4, 8, 12, 16, 20]
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    ax.plot(x, y4, 'o-', label='Average apply requests - 3 fields')
    ax.set_xlabel('latency (ms)', size=12)
    ax.set_ylabel('apply requests (req/s)', size=12)
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0)
    plt.ylim([-5, 230])
    plt.title("Requests on moderate latency - 3 game state fields")
    # Plot the confidence interval
    plt.show()

if __name__ == '__main__':
    # Set the number of seconds and the file to input
    compute_data_FPS(90, "output.txt")
    plot_data_requests()
    plot_data_FPS()



