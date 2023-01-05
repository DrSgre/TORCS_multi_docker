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
    data = []
    detract = 0
    for i in range(len(Lines_1)):
        line_string1 = Lines_1[i].strip()
        line_split1 = line_string1.split(" ")
        state1 = int(line_split1[1])
        value1 = int(line_split1[2])
        line_string2 = Lines_2[i-detract].strip()
        line_split2 = line_string2.split(" ")
        state2 = int(line_split2[1])
        value2 = int(line_split2[2])
        if state1 == state2:
            data.append(value2 - value1)
        else:
            detract += 1
    average_value = mean(data)
    max_value = max(data)
    sdeviation_value = stdev(data)
    print("Number of states: " + str(len(data)))
    print("State inconsistency - m: " + str(average_value) + " ns: " + str(sdeviation_value))
    print("Maximum inconsistency: " + str(max_value))
    print("")

if __name__ == '__main__':
    # Set the files to input
    compute_data_state("current_state.txt", "remote_state.txt")

