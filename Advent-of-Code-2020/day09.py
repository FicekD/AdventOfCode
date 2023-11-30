import os


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [int(line.rstrip('\n')) for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day09_data.txt')
    data = read_data(data_path)

    nums = data[25:]
    for i, num in enumerate(nums):
        preamble = data[i:25+i]
        found = False
        for first in preamble:
            for second in preamble:
                if first == second:
                    continue
                if first + second == num:
                    found = True
                    break
            if found:
                break
        if not found:
            res = num
            break
    for i in range(len(data)):
        for j in range(len(data) - i - 1):
            if sum(data[i:i+j]) == res:
                print(max(data[i:i+j]) + min(data[i:i+j]))
                return
            elif sum(data[i:i+j]) > res:
                break


if __name__ == '__main__':
    main()
