import os
import re


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def can_contain_gold(bag, rules):
    for rule in rules[bag]:
        if rule == 'no other':
            return False
        if 'shiny gold' in rule:
            return True
        if can_contain_gold(rule[2:], rules):
            return True
    return False


def how_many_in(bag, rules):
    result = 0
    for rule in rules[bag]:
        print(rule)
        if rule == 'no other':
            return 0
        result += (int(rule[0]) + int(rule[0])*how_many_in(rule[2:], rules))
    return result


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day07_data.txt')
    data = read_data(data_path)

    rules = dict()
    for line in data:
        spaced = line.split(' ')
        key = spaced[0] + ' ' + spaced[1]
        line = line.replace(' bags', '').replace(' bag', '')
        values = re.findall(r'contain (.+?)\.', line)[-1].split(', ')
        rules[key] = values
    
    result = 0
    for key in rules:
        result += int(can_contain_gold(key, rules))
    print(result)

    print(how_many_in('shiny gold', rules))


if __name__ == '__main__':
    main()
