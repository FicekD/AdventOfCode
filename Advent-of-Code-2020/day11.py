import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day11_data.txt')
    data = read_data(data_path)
    data_list = [[1 if char == 'L' else -1 for char in line] for line in data]
    data = np.zeros((len(data) + 2, len(data[0]) + 2)) - 1
    data[1:-1, 1:-1] = np.array(data_list)
    data[np.where(data == -1)] = np.nan
    iteration = 0
    while True:
        print(iteration)
        new_data = data.copy()
        changed = False
        for row in range(data.shape[0]):
            for col in range(data.shape[1]):
                if np.isnan(data[row, col]):
                    continue
                area = data[row-1:row+2, col-1:col+2].copy()
                for i in [-1, 0, 1]:
                    for j in [-1, 0, 1]:
                        if i == j and i == 0:
                            continue
                        if np.isnan(area[1+i, 1+j]):
                            i_c, j_c = i, j
                            while True:
                                try:
                                    if row+i_c < 0 or col+j_c < 0:
                                        break
                                    if np.isnan(data[row+i_c, col+j_c]):
                                        j_c += j
                                        i_c += i
                                    else:
                                        area[1+i, 1+j] = data[row+i_c, col+j_c]
                                        break
                                except IndexError:
                                    break
                seats = 8 - np.sum(np.isnan(area))
                empty = int(np.nansum(area) - data[row, col])
                if data[row, col] == 1 and empty == seats:
                    changed = True
                    new_data[row, col] = 0
                elif data[row, col] == 0 and seats - empty >= 5:
                    changed = True
                    new_data[row, col] = 1
        if not changed:
            break
        data[:, :] = new_data
        iteration += 1
    print(int(-np.nansum(data - 1)))


if __name__ == '__main__':
    main()
