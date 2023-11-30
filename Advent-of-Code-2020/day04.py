import os


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def is_hexa(string):
    try:
        int(string, 16)
        return True
    except ValueError:
        return False


def is_dec(string):
    try:
        int(string)
        return True
    except ValueError:
        return False


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day04_data.txt')
    data = read_data(data_path)
    data.append('')

    rules = {
        'byr': lambda x: is_dec(x) and len(x) == 4 and (1920 <= int(x) <= 2002),
        'iyr': lambda x: is_dec(x) and len(x) == 4 and (2010 <= int(x) <= 2020),
        'eyr': lambda x: is_dec(x) and len(x) == 4 and (2020 <= int(x) <= 2030),
        'hgt': lambda x: len(x) >= 4 and (150 <= int(x[:-2]) <= 193 if x[-2:] == 'cm' else 59 <= int(x[:-2]) <= 76),
        'hcl': lambda x: x[0] == '#' and len(x[1:]) == 6 and is_hexa(x[1:]),
        'ecl': lambda x: x in ['amb', 'blu', 'brn', 'gry', 'grn', 'hzl', 'oth'],
        'pid': lambda x: len(x) == 9 and is_dec(x),
        'cid': lambda x: True,
        }

    passport = set()
    valid = 0
    invalid = 0
    for line in data:
        if line == '':
            if ((len(passport) == 8) or ((len(passport) == 7) and ('cid' not in passport))):
                valid += 1
            else:
                invalid += 1
            passport = set()
            continue
        for value in line.split(' '):
            pair = value.split(':')
            if rules[pair[0]](pair[1]):
                passport.add(pair[0])
    print(valid, invalid)


if __name__ == '__main__':
    main()
