from statistics import mean, stdev
import matplotlib
from matplotlib import pyplot as plt
import numpy as np

matplotlib.use('TkAgg')


def plot_data_benchmarks():
    #y = [54744.52, 48931.66, 46446.82, 45945.05, 44977.51, 40899.80, 36954.91, 32157.79, 28675.21, 27262.81, 24234.59]
    y = [94280.33, 88079.86, 87391.27, 80450.52, 67506.76, 55679.29, 47611.49, 41367.90, 36751.20, 32658.39, 29620.85]
    #y1 = [7560.70, 7248.32, 7156.25, 5861.52, 5776.90, 5619.94, 5563.37, 5153.00, 5135.07, 4951.06, 4567.56]
    y1 = [41640.97, 36074.40, 28986.20, 23625.16, 18784.41, 15538.75, 13278.67, 11639.65, 10389.08, 9547.55, 8730.23]
    x = [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20]
    # Plot the sinus function
    fig = plt.figure()
    ax = fig.add_axes([0.1, 0.1, 0.6, 0.75])
    ax.plot(x, y, 'o-', color='blue', label='Redis request rate')
    ax.plot(x, y1, 'o-', color='green', label='ETCD request rate')
    ax.set_xlabel('latency (ms)', size=12)
    ax.set_ylabel('throughput (req/s)', size=12)
    ax.set_xticks(np.arange(0, 22, 2))
    ax.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0)
    plt.ylim([-5, 100000])
    plt.title("ETCD-Redis read requests benchmark")
    # Plot the confidence interval
    plt.show()


if __name__ == '__main__':
    plot_data_benchmarks();

