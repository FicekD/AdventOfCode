import os


def fetch_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n').split(' ') for line in lines]
    
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data = fetch_data(os.path.join(base_path, 'data', 'day02_data.txt'))

    ranges = [(int(line[0].split('-')[0]), int(line[0].split('-')[1])) for line in data]
    letters = [line[1][0] for line in data]
    pwrds = [line[2] for line in data]

    valid_a = 0
    valid_b = 0
    for rng, letter, pwrd in zip(ranges, letters, pwrds):
        num = pwrd.count(letter)
        if num >= rng[0] and num <= rng[1]:
            valid_a += 1
        if (pwrd[rng[0]-1] == letter or pwrd[rng[1]-1] == letter) and pwrd[rng[0]-1] != pwrd[rng[1]-1]:
            valid_b += 1
    print(valid_a)
    print(valid_b)


if __name__ == '__main__':
    main()
