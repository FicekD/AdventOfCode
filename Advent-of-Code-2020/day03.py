import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [[0 if char == '.' else 1 for char in line.rstrip('\n')] for line in lines]
    return np.array(lines)


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day03_data.txt')
    data = read_data(data_path)
    
    down = 1
    right = 3

    indices_down = list(range(0, data.shape[0], down))
    indices_right = [i % data.shape[1] for i in range(0, len(indices_down)*right, right)]

    result = np.sum(data[indices_down, indices_right])
    print(result)

    down = [1, 1, 1, 1, 2]
    right = [1, 3, 5, 7, 1]
    result = np.array([1], dtype=np.uint64)
    for d, r in zip(down, right):
        indices_down = list(range(0, data.shape[0], d))
        indices_right = [i % data.shape[1] for i in range(0, len(indices_down)*r, r)]
        result[0] *= np.sum(data[indices_down, indices_right])
    print(result[0])


if __name__ == '__main__':
    main()
