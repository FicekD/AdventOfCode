import os
import copy


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


class Node:
    def __init__(self, x, aprev, anext):
        self.x = x
        self.prev = aprev
        self.next = anext
    def __eq__(self, rside):
        return self.x == rside


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day23_data.txt')
    data = read_data(data_path)

    cups = [int(i) for i in data[0]]
    idx = 0
    for _ in range(100):
        idx_val = cups[idx]
        start = (idx+1) % len(cups)
        stop = (idx+4) % len(cups)
        pickup = cups[start:stop] if stop > start else cups[start:] + cups[:stop]
        dest = cups[idx] - 1
        while dest not in cups or dest in pickup:
            dest = (dest - 1) % (max(cups) + 1)
        for val in pickup:
            cups.remove(val)
        i = cups.index(dest)
        for val in reversed(pickup):
            cups.insert(i+1, val)
        idx = (cups.index(idx_val) + 1) % len(cups)
    idx = cups.index(1)
    print(''.join(str(val) for val in cups[idx+1:] + cups[:idx]))

    cups = [int(i) for i in data[0]]
    max_c = 1000000
    cups = cups + list(range(len(cups)+1, max_c+1))
    
    head = Node(cups[0], None, None)
    prev = head
    for val in cups[1:]:
        node = Node(val, prev, None)
        prev.next = node
        prev = node
    prev.next = head
    head.prev = prev

    idx_map = {}
    curr = head
    while True:
        idx_map[curr.x] = curr
        curr = curr.next
        if curr == head:
            break

    curr = head
    for lol in range(10000000):
        first = curr.next
        second = first.next
        third = second.next

        curr.next = third.next
        third.next.prev = curr

        x = (curr.x - 1) % (max_c + 1)
        while x == 0 or x in [first, second, third]:
            x = (x - 1) % (max_c + 1)

        dest = idx_map[x]
        foo = dest.next
        dest.next = first
        first.prev = dest
        third.next = foo
        foo.prev = third

        curr = curr.next
    print(idx_map[1].next.x * idx_map[1].next.next.x)


if __name__ == '__main__':
    main()
