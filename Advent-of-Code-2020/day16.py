import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day16_data.txt')
    data = read_data(data_path)
    
    outputs = [[], [], []]
    i = 0
    for line in data:
        if line == '':
            i += 1
            continue
        outputs[i].append(line)
    rules_raw, myticket, tickets = outputs[0], outputs[1][1:], outputs[2][1:]

    rules = {}
    for line in rules_raw:
        pair = line.split(': ')
        key = pair[0]
        vals = pair[1].split(' or ')
        vals = tuple([tuple(map(int, pair.split('-'))) for pair in vals])
        rules[key] = vals

    myticket = tuple([int(val) for val in myticket[0].split(',')])
    tickets = np.array([[int(val) for val in line.split(',')] for line in tickets])

    inrange = lambda nums, ranges: np.logical_or(np.logical_and(ranges[0][0] <= nums, nums <= ranges[0][1]),
                                                 np.logical_and(ranges[1][0] <= nums, nums <= ranges[1][1]))
    sum_invalid = 0
    valid = list()
    for ticket in tickets:
        mask = np.zeros(ticket.size, dtype=np.bool)
        for rule in rules.values():
            mask += inrange(ticket, rule)
        if np.all(mask):
            valid.append(ticket)
        sum_invalid += np.sum(~mask * ticket)
    print(sum_invalid)

    tickets = np.array(valid, dtype=np.int32)
    cols = list()
    for rule in rules.values():
        mask = inrange(tickets, rule)
        valid = np.all(mask, axis=0)
        cols.append(np.where(valid == True))

    matches = len(cols) * [None]
    indices = list()
    changed = True
    while changed:
        changed = False
        for match in matches:
            if match is not None:
                continue
            for i, col in enumerate(cols):
                col = col[0].tolist()
                for idx in indices:
                    try:
                        col.remove(idx)
                    except ValueError:
                        continue
                if len(col) == 1:
                    indices.append(col[0])
                    matches[i] = col[0]
                    changed = True
                    break
    result = 1
    for idx, rule in zip(matches, rules):
        if not 'departure' in rule:
            continue
        result *= myticket[idx]
    print(result)


if __name__ == '__main__':
    main()
