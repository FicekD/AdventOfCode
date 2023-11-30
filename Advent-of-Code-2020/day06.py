import os


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day06_data.txt')
    data = read_data(data_path)
    data.append('')

    groups = list()
    group = set()
    for line in data:
        if line == '':
            groups.append(len(group))
            group = set()
            continue
        for value in line:
            group.add(value)
    print(sum(groups))

    groups = list()
    group = ''
    group_size = 0
    for line in data:
        if line == '':
            num = 0
            for _ in range(len(group)):
                try:
                    char = group[0]
                except IndexError:
                    break
                if group.count(char) == group_size:
                    num += 1
                group = group.replace(char, '')
            groups.append(num)
            group = ''
            group_size = 0
            continue
        group_size += 1
        for value in line:
            group += value
    print(sum(groups))


if __name__ == '__main__':
    main()
