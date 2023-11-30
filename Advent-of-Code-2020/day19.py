import os
import re


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


# algorithm by /u/MichalMarsalek on reddit
def check_rule(rules, text, num):
    if len(text) == 0:
        return []
    if rules[num] == 'a' or rules[num] == 'b':
        if text[0] == rules[num]:
            return [1]
        else:
            return []
    length0 = list()
    for ruleset in rules[num]:
        length = [0]
        for rule in ruleset:
            length2 = []
            for l in length:
                for c in check_rule(rules, text[l:], rule):                        
                    length2.append(l+c)
            length = length2
        length0.extend(length)         
    return length0


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day19_data.txt')
    data = read_data(data_path)

    rules = {}
    for i, line in enumerate(data):
        if line == '':
            break
        key, stripped = line.split(': ')
        stripped = stripped.replace('\"', '')
        if stripped != 'a' and stripped != 'b':
            split = stripped.split('|')
            rule = [[int(val) for val in ruleset.split(' ') if val != ''] for ruleset in split]
            rules[int(key)] = rule
        else:
            rules[int(key)] = stripped
    messages = data[i+1:]
    print(sum(len(message) in check_rule(rules, message, 0) for message in messages))
    rules[8] = [[42], [42, 8]]
    rules[11] = [[42, 31], [42, 11, 31]]
    print(sum(len(message) in check_rule(rules, message, 0) for message in messages))


if __name__ == '__main__':
    main()
