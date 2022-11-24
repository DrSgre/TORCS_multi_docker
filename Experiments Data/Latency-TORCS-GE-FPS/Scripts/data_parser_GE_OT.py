from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')


# Function used to compute the values of Game Engine and Graphics framerate.
def compute_data_FPS(n, file):
    file = open(file, 'r')
    Lines = file.readlines()
    total_count = 0
    data = []
    data_time = []
    data_GE = []
    for line in Lines:
        if total_count < (2*n):
            if total_count % 2 == 0:
                line_string = line.strip()
                line_split = line_string.split("Current FPS: ")
                first_split = line_split[1].split(" with ")
                fps_split = first_split[0]
                ot_split = first_split[1].split("ms")[0]
                fps_value = float(fps_split)
                ot_value = float(ot_split)
                data.append(fps_value)
                data_time.append(ot_value)
                total_count += 1
            else:
                line_string = line.strip()
                line_split = line_string.split("Current RE FPS: ")
                first_split = line_split[1].split(" with ")
                fps_split = first_split[0]
                fps_value = float(fps_split)
                data_GE.append(fps_value)
                total_count += 1
        else:
            break
    fps_average = mean(data)
    fps_max = max(data)
    fps_sdeviation = stdev(data)
    GE_fps_average = mean(data_GE)
    GE_fps_max = max(data_GE)
    GE_fps_sdeviation = stdev(data_GE)
    tot_op_time = sum(data_time)

    print("FPS - m: " + str(fps_average) + " s: " + str(fps_sdeviation))
    print("Maximum FPS: " + str(fps_max))
    print("")
    print("GE FPS - m: " + str(GE_fps_average) + " s: " + str(GE_fps_sdeviation))
    print("Maximum GE FPS: " + str(GE_fps_max))
    print("")
    print("Tot operational time: " + str(tot_op_time) + "ms")
    print("")


# Function used to plot the data obtained the measurements, to be introduced manually.
def plot_data_FPS():
    y = [59.924, 59.169, 27.913, 26.483, 17.906, 14.145, 9.618, 8.106, 7.192, 6.422, 5.644]
    y1 = [498.467, 498.689, 412.289, 285.000, 223.656, 185.000, 155.289, 134.622, 119.344, 106.289, 95.944]
    x = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    ci_l = [59.774, 58.719, 27.555, 25.680, 17.663, 13.835, 9.397, 8.056, 7.093, 6.356, 5.561]
    ci_u = [60.074, 59.619, 28.273, 27.286, 18.148, 14.454, 9.840, 8.156, 7.290, 6.489, 5.728]
    ci_l_1 = [496.960, 497.38, 407.83, 282.733, 222.137, 184.193, 154.505, 134.030, 119.150, 106.088, 95.805]
    ci_u_1 = [499.970, 500.00, 416.75, 287.267, 225.174, 185.807, 156.073, 135.214, 119.538, 106.490, 96.084]
    # Plot the sinus function
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    ax.plot(x, y, 'o-', color='blue', label='Average TORCS FPS')
    ax.plot(x, y1, 'o-', color='green', label='Average GE FPS')
    ax.set_xlabel('latency (ms)', size=12)
    ax.set_ylabel('framerate (fps)', size=12)
    ax.set_xticks(np.arange(0, len(x) + 1, 1))
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0)
    plt.fill_between(x, ci_l, ci_u, color='blue', alpha=0.1)
    plt.fill_between(x, ci_l_1, ci_u_1, color='green', alpha=0.1)
    plt.ylim([-5, 505])
    plt.title("TORCS FPS on sim module delay")
    # Plot the confidence interval
    plt.show()


if __name__ == '__main__':
    # Set the number of seconds and the file to input
    compute_data_FPS(90, "output.txt")
    plot_data_FPS()

