import os


def fetch_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [int(line.rstrip('\n')) for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data = fetch_data(os.path.join(base_path, 'data', 'day01_data.txt'))
    data.sort()

    solution = 2020
    for val1 in reversed(data):
        for val2 in data:
            if val1 == val2:
                continue
            for val3 in data:
                if val1 == val3 or val2 == val3:
                    continue
                elif val1 + val2 + val3 == solution:
                    print(val1, val2, val3)
                    print(val1 * val2 * val3)
                    return
                if (val1 + val2 + val3) > solution:
                    break


if __name__ == '__main__':
    main()