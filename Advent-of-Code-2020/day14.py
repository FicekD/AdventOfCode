import os
import numpy as np
import re


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def decode_adress(add):
    output = list()
    size = add.count('x')
    parts = add.split('x')
    for i in range(2 ** size):
        vals = list(('{:0' + str(size) + 'b}').format(i))
        addr = ''
        for part, val in zip(parts, vals):
            addr += part
            addr += val
        addr += parts[-1]
        output.append(addr)
    return output


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day14_data.txt')
    data = read_data(data_path)

    addresses = {}

    masking = {
        '0': lambda x: '0',
        '1': lambda x: '1',
        'X': lambda x: x
    }

    mask = None
    for line in data:
        if line[:4] == 'mask':
            mask = line[7:]
            continue
        address = int(re.findall(r'\[(\d+)\]', line)[0])
        value = '{:036b}'.format(int(re.findall(r'= (\d+)$', line)[0]))
        masked = ''
        for mbit, vbit in zip(mask, value):
            masked += masking[mbit](vbit)
        masked = int(masked, 2)
        addresses[address] = masked
    print(sum(addresses.values()))

    addresses = {}

    masking = {
        '0': lambda x: x,
        '1': lambda x: '1',
        'X': lambda x: 'x'
    }

    mask = None
    for line in data:
        if line[:4] == 'mask':
            mask = line[7:]
            continue
        address = '{:036b}'.format(int(re.findall(r'\[(\d+)\]', line)[0]))
        value = int(re.findall(r'= (\d+)$', line)[0])
        masked = ''
        for mbit, abit in zip(mask, address):
            masked += masking[mbit](abit)
        masked_adresses = decode_adress(masked)
        for address in masked_adresses:
            addresses[int(address)] = value
    print(sum(addresses.values()))


if __name__ == '__main__':
    main()
