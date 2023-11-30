import os
import numpy as np
from scipy.signal import correlate


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day24_data.txt')
    data = read_data(data_path)

    coords = list()
    for line in data:
        line = list(line.replace('sw', 's').replace('nw', 'n').replace('ne', 'm').replace('se', 'z'))
        line = [char.replace('s', 'sw').replace('n', 'nw').replace('m', 'ne').replace('z', 'se') for char in line]
        coords.append(line)

    dirs = {
        'e': lambda x, y: (x+1, y),
        'w': lambda x, y: (x-1, y),
        'sw': lambda x, y: (x, y+1),
        'nw': lambda x, y: (x-1, y-1),
        'se': lambda x, y: (x+1, y+1),
        'ne': lambda x, y: (x, y-1),
    }

    grid = np.full((201, 201), 0, dtype=np.bool)
    for seq in coords:
        x, y = 100, 100
        for direction in seq:
            x, y = dirs[direction](x, y)
        grid[y, x] = not grid[y, x]
    print(np.sum(grid == 1))

    kernel = np.array([[1, 1, 0], [1, 0, 1], [0, 1, 1]])
    for lol in range(100):
        new_grid = grid.copy()
        num_black = correlate(grid, kernel, 'same')
        for y in range(1, grid.shape[0]-1):
            for x in range(1, grid.shape[1]-1):
                if grid[y, x] == 1 and (num_black[y, x] == 0 or num_black[y, x] > 2):
                    new_grid[y, x] = 0
                elif grid[y, x] == 0 and num_black[y, x] == 2:
                    new_grid[y, x] = 1
        grid = new_grid
    print(np.sum(grid == 1))


if __name__ == '__main__':
    main()
