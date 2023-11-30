import os
import numpy as np
from scipy.signal import correlate


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def transform(number, loop_size):
    val = 1
    for _ in range(loop_size):
        val = (val * number) % 20201227
    return val


def find_loopsize(number, public_key):
    val = 1
    i = 1
    while True:
        val = (val * number) % 20201227
        if val == public_key:
            break
        i += 1
    return i


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day25_data.txt')
    data = read_data(data_path)

    card_public = int(data[0])
    door_public = int(data[1])

    loopsize_card = find_loopsize(7, card_public)
    loopsize_door = find_loopsize(7, door_public)

    print(transform(card_public, loopsize_door))


if __name__ == '__main__':
    main()
