import os


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [int(line.rstrip('\n')) for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day10_data.txt')
    data = read_data(data_path)
    data.sort()
    data = [0] + data + [data[-1] + 3]

    diff = [0, 0, 0]
    prev = 0
    for num in data[1:]:
        diff[num-prev-1] += 1
        prev = num
    print(diff[0]*diff[2])

    varsiations = [1]
    for data_val in data[1:]:
        paths = 0
        for inputs, var_val in zip(varsiations, data):
            if data_val - var_val <= 3:
                paths += inputs
        varsiations.append(paths)
    print(varsiations[-1])


if __name__ == '__main__':
    main()
