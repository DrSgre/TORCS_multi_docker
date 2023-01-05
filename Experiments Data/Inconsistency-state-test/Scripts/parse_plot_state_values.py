import math
from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')

# Function used to parse the data for the state manager inconsistency between local and remote.
def compute_data_state(file1, file2):
    file_1 = open(file1, 'r')
    file_2 = open(file2, 'r')
    Lines_1 = file_1.readlines()
    Lines_2 = file_2.readlines()
    pos = []
    speed = []
    acc = []
    j = 0
    for i in range(len(Lines_1)):
        line_string1 = Lines_1[i].strip()
        line_split1 = line_string1.split(" ")
        time1 = int(line_split1[1])
        position1 = line_split1[3]
        position1_string = position1.split(",")
        position1_values = []
        for k in position1_string:
            position1_values.append(float(k))
        speed1 = line_split1[4]
        speed1_string = speed1.split(",")
        speed1_values = []
        for k in speed1_string:
            speed1_values.append(float(k))
        acc1 = line_split1[5]
        acc1_string = acc1.split(",")
        acc1_values = []
        for k in acc1_string:
            acc1_values.append(float(k))

        line_string2 = Lines_2[j].strip()
        line_split2 = line_string2.split(" ")
        time2 = int(line_split2[1])
        if time2 < time1:
            while time2 < time1:
                j += 1
                line_string2 = Lines_2[j].strip()
                line_split2 = line_string2.split(" ")
                time2 = int(line_split2[1])
        position2 = line_split2[3]
        position2_string = position2.split(",")
        position2_values = []
        for k in position2_string:
            position2_values.append(float(k))
        speed2 = line_split2[4]
        speed2_string = speed2.split(",")
        speed2_values = []
        for k in speed2_string:
            speed2_values.append(float(k))
        acc2 = line_split2[5]
        acc2_string = acc2.split(",")
        acc2_values = []
        for k in acc2_string:
            acc2_values.append(float(k))
        pos_norm = math.sqrt(math.pow(position2_values[0]-position1_values[0], 2) +
                             math.pow(position2_values[1]-position1_values[1], 2) +
                             math.pow(position2_values[2]-position1_values[2], 2) +
                             math.pow(position2_values[3]-position1_values[3], 2) +
                             math.pow(position2_values[4]-position1_values[4], 2) +
                             math.pow(position2_values[4]-position1_values[5], 2))

        speed_norm = math.sqrt(math.pow(speed2_values[0]-speed1_values[0], 2) +
                             math.pow(speed2_values[1]-speed1_values[1], 2) +
                             math.pow(speed2_values[2]-speed1_values[2], 2) +
                             math.pow(speed2_values[3]-speed1_values[3], 2))

        acc_norm = math.sqrt(math.pow(acc2_values[0]-acc1_values[0], 2) +
                             math.pow(acc2_values[1]-acc1_values[1], 2) +
                             math.pow(acc2_values[2]-acc1_values[2], 2))
        pos.append(pos_norm)
        speed.append(speed_norm)
        acc.append(acc_norm)
    average_value_pos = mean(pos)
    average_value_speed = mean(speed)
    average_value_acc = mean(acc)
    sdeviation_value_pos = stdev(pos)
    sdeviation_value_speed = stdev(speed)
    sdeviation_value_acc = stdev(acc)
    print("Number of samples: " + str(len(pos)))
    print("Position variation in states - m: " + str(average_value_pos) + " with SD: " + str(sdeviation_value_pos))
    print("Speed variation in states - m: " + str(average_value_speed) + " with SD: " + str(sdeviation_value_speed))
    print("Acceleration variation in states - m: " + str(average_value_acc) + " with SD: " + str(sdeviation_value_acc))
    print("")

# Function used to plot the data obtained the state inconsistency measurements, to be introduced manually.
# y -> position
# y1 -> speed
# y2 -> acceleration
def plot_data_states():
    y = [2.040, 17.035, 34.081, 51.205, 66.560, 81.920]
    y1 = [0.097, 2.076, 3.261, 4.462, 5.415, 7.122]
    y2 = [0.938, 7.029, 8.157, 8.975, 10.460, 10.471]
    #y4 = []
    x = [0, 1, 2, 3, 4, 5]
    ci_l = [2.033, 16.971, 33.951, 51.013, 66.318, 81.624]
    ci_u = [2.047, 17.101, 34.212, 51.398, 66.802, 82.215]
    ci_l_1 = [0.096, 2.061, 3.236, 4.426, 5.379, 7.069]
    ci_u_1 = [0.098, 2.092, 3.286, 4.499, 5.451, 7.174]
    ci_l_2 = [0.922, 6.968, 8.091, 8.906, 10.383, 10.403]
    ci_u_2 = [0.953, 7.090, 8.224, 9.045, 10.536, 10.539]
    #ci_l_3 = []
    #ci_u_3 = []
    # Plot the sinus function
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    ax.plot(x, y, 'o-', label='Position')
    ax.plot(x, y1, 'o-', label='Speed')
    ax.plot(x, y2, 'o-', label='Acceleration')
    ax.set_xlabel('latency (ms)', size=12)
    ax.set_ylabel('average inconsistency value', size=12)
    ax.legend(bbox_to_anchor=(1.0, 1.05))
    plt.fill_between(x, ci_l, ci_u, alpha=0.1)
    plt.fill_between(x, ci_l_1, ci_u_1, alpha=0.1)
    plt.fill_between(x, ci_l_2, ci_u_2, alpha=0.1)
    #plt.fill_between(x, ci_l_3, ci_u_3, alpha=0.1)
    plt.ylim([-5, 90])
    plt.title("Redis state inconsistency on low latency")
    # Plot the confidence interval
    plt.show()

if __name__ == '__main__':
    # Set the files to input
    # compute_data_state("current_state_values5_redis.txt", "remote_state_values5_redis.txt")
    plot_data_states()

