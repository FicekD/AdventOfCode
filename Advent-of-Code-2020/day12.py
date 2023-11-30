import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def rotate(x, y, rot):
    if rot == 180:
        return -x, -y
    elif rot == 90:
        return -y, x
    elif rot == 270:
        return y, -x
    else:
        return x, y



def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day12_data.txt')
    data = read_data(data_path)

    angles = {
        0: (1, 0), 
        180: (-1, 0),
        90: (0, 1),
        270: (0, -1)
    }

    instructions = {
        'N': lambda num, x, y, angle: (x, y + num, angle),
        'S': lambda num, x, y, angle: (x, y - num, angle),
        'E': lambda num, x, y, angle: (x + num, y, angle),
        'W': lambda num, x, y, angle: (x - num, y, angle),
        'L': lambda num, x, y, angle: (x, y, (angle + num) % 360),
        'R': lambda num, x, y, angle: (x, y, (angle - num) % 360),
        'F': lambda num, x, y, angle: (x + num * angles[angle][0], y + num * angles[angle][1], angle),
    }

    x, y, angle = 0, 0, 0
    for inst in data:
        x, y, angle = instructions[inst[0]](int(inst[1:]), x, y, angle)
    print(abs(x) + abs(y))

    instructions = {
        'N': lambda num, x, y: (x, y + num),
        'S': lambda num, x, y: (x, y - num),
        'E': lambda num, x, y: (x + num, y),
        'W': lambda num, x, y: (x - num, y),
        'L': lambda num, x, y: (rotate(x, y, num % 360)),
        'R': lambda num, x, y: (rotate(x, y, -num % 360)),
    }

    x_ship, y_ship, x_wp, y_wp = 0, 0, 10, 1
    for inst in data:
        if inst[0] == 'F':
            x_ship += int(inst[1:]) * x_wp
            y_ship += int(inst[1:]) * y_wp
        else:
            x_wp, y_wp = instructions[inst[0]](int(inst[1:]), x_wp, y_wp)
    print(abs(x_ship) + abs(y_ship))


if __name__ == '__main__':
    main()
