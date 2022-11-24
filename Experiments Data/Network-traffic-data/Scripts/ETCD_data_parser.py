from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')


def byte_conversion(value):
    if "kB" in value:
        value = float(value.split("kB")[0]) * 1000
        return value
    if "MB" in value:
        value = float(value.split("MB")[0]) * 1000000
        return value
    if "GB" in value:
        value = float(value.split("GB")[0]) * 1000000000
        return value
    if "B" in value:
        value = float(value.split("B")[0])
        return value


# Function used to compute the data related to the system network traffic.
def compute_data_docker(n, file):
    file = open(file, 'r')
    Lines = file.readlines()
    count = 0
    data = []
    plot_data = [[], [], [], []]

    for line in Lines:
        if count < n:
            if line != "\n":
                line_string = line.strip()
                line_split = line_string.split("-")
                if (line_split[0] == "app"):
                    temp_split = []
                    temp_split.append(line_split[0] + line_split[1])
                    temp_split.append(line_split[2])
                    temp_split.append(line_split[3])
                    line_split = temp_split
                name = line_split[0]
                net_split = line_split[1].split(" / ")
                net_input = net_split[0]
                net_output = net_split[1]
                block_split = line_split[2].split(" / ")
                block_input = block_split[0]
                block_output = block_split[1]
                net_input = byte_conversion(net_input)
                net_output = byte_conversion(net_output)
                block_input = byte_conversion(block_input)
                block_output = byte_conversion(block_output)
                if name == 'appetcd_Etcd_1':
                    plot_data[0].append(net_input)
                    plot_data[1].append(net_output)
                    plot_data[2].append(block_input)
                    plot_data[3].append(block_output)
                present = False
                for i in range(len(data)):
                    if data[i][0][0] == name:
                        data[i][1].append(net_input - data[i][0][1])
                        data[i][0][1] = net_input
                        data[i][2].append(net_output - data[i][0][2])
                        data[i][0][2] = net_output
                        data[i][3].append(block_input - data[i][0][3])
                        data[i][0][3] = block_input
                        data[i][4].append(block_output - data[i][0][4])
                        data[i][0][4] = block_output
                        present = True
                if not present:
                    data.append([[name]])
                    data[len(data) - 1][0].append(net_input)
                    data[len(data) - 1].append([net_input])
                    data[len(data) - 1][0].append(net_output)
                    data[len(data) - 1].append([net_output])
                    data[len(data) - 1][0].append(block_input)
                    data[len(data) - 1].append([block_input])
                    data[len(data) - 1][0].append(block_output)
                    data[len(data) - 1].append([block_output])
            else:
                count += 2
        else:
            break
    y1 = plot_data[0]
    y2 = plot_data[1]
    y3 = plot_data[2]
    y4 = plot_data[3]
    x = np.linspace(0, 90, num=45)
    # Plot the sinus function
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    # Also change the name of the labels to match the container you are plotting.
    ax.plot(x, y1, 'o-', markersize=2, label='ETCD Network Input')
    ax.plot(x, y2, 'o-', markersize=2, label='ETCD Network Output')
    ax.plot(x, y3, 'o-', markersize=2, label='ETCD Block Input')
    ax.plot(x, y4, 'o-', markersize=2, label='ETCD Block Output')
    # Set the preferred scale and change the axis limits to match.
    ax.set_yscale('log')
    plt.ylim(bottom=1e4, top=1e10)
    #ax.set_yscale('symlog')
    #plt.ylim(top=1e10)
    #plt.ylim(top=2e9)

    ax.set_xlabel('time (s)', size=12)
    ax.set_ylabel('network traffic (B)', size=12)
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0)
    # Also change the name of graph to match the container you are plotting.
    plt.title("ETCD container data")
    plt.xlim([-5, 95])
    plt.show()
    for i in range(len(data)):
        net_input_average = mean(data[i][1])
        net_output_average = mean(data[i][2])
        block_input_average = mean(data[i][3])
        block_output_average = mean(data[i][4])
        net_input_sdeviation = stdev(data[i][1])
        net_output_sdeviation = stdev(data[i][2])
        block_input_sdeviation = stdev(data[i][3])
        block_output_sdeviation = stdev(data[i][4])
        print(data[i][0][0] + " Network Input - m: " + str(net_input_average) + " s: " + str(net_input_sdeviation))
        print(data[i][0][0] + " Network Output - m: " + str(net_output_average) + " s: " + str(net_output_sdeviation))
        print(data[i][0][0] + " Block Input - m: " + str(block_input_average) + " s: " + str(block_input_sdeviation))
        print(data[i][0][0] + " Block Output - m: " + str(block_output_average) + " s: " + str(block_output_sdeviation))
        print(data[i][0][0] + " Maximum Network Input: " + str(data[i][0][1]))
        print(data[i][0][0] + " Maximum Network Output: " + str(data[i][0][2]))
        print(data[i][0][0] + " Maximum Network Input: " + str(data[i][0][3]))
        print(data[i][0][0] + " Maximum Block Output: " + str(data[i][0][4]))
        print("")

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
    # Set the number of seconds and the file to input
    compute_data_docker(90, "output_docker_etcd.txt")
    compute_data_RTT(90, "output_RTT.txt")

