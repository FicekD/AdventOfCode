import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    lines[0] = int(lines[0])
    lines[1] = [(int(val), i) for i, val in enumerate(lines[1].split(',')) if val != 'x']
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day13_data.txt')
    data = read_data(data_path)

    intervals = list()
    for bus in data[1]:
        intervals.append((bus[0] - data[0] % bus[0], bus[0]))
    
    bus = min(intervals, key=lambda x: x[0])
    print(bus[0] * bus[1])

    t = 0
    lcm = 1
    for i in range(len(data[1])-1):
        lcm *= data[1][i][0]
        while (t + data[1][i+1][1]) % data[1][i+1][0] != 0:
            t += lcm
    print(t)


if __name__ == '__main__':
    main()
