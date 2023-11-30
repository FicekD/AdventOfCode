import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day15_data.txt')
    data = read_data(data_path)
    
    nums = [int(val) for val in data[0].split(',')]

    turns = 30000000
    history = [0] * turns

    i = 0
    while i < len(nums):
        history[nums[i - 1]] = i
        i += 1
    last_num = nums[-1]
    while i < turns:
        num = last_num
        last_num = 0 if history[last_num] == 0 else i - history[last_num]
        history[num] = i
        i += 1
    print(last_num)


if __name__ == '__main__':
    main()
