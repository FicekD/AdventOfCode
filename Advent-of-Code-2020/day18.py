import os


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def basic_expression(left, operator, right):
    if len(left) != 1:
        left = [basic_expression(left[:-2], left[-2], left[-1])]
    # print(f'{left} {operator} {right}')
    if operator == '*':
        return int(left[0]) * int(right)
    else:
        return int(left[0]) + int(right)


def solve_braces(data, idx=0, advanced=False):
    j = None
    for i in range(idx, len(data)):
        if data[i] == '(' and j is None:
            j = i
        elif data[i] == '(' and j is not None:
            return solve_braces(data, i, advanced)
        elif data[i] == ')':
            if not advanced:
                solution = basic_expression(data[j+1:i-2], data[i-2], data[i-1])
            else:
                solution = advanced_expression(data[j+1:i])
            data[j:i+1] = [solution]    
            return True
    return False


def advanced_expression(expr):
    i, length = 0, len(expr)
    while i < length:
        if expr[i] == '+':
            expr[i-1:i+2] = [int(expr[i-1]) + int(expr[i+1])]
            length -= 2
            i -= 1
        i += 1
    solution = 1
    for val in expr:
        if val == '*':
            continue
        solution *= int(val)
    return solution


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day18_data.txt')
    data = read_data(data_path)

    data = [[val for val in list(line) if val != ' '] for line in data]

    solutions = list()
    for expression in data:
        while solve_braces(expression):
            pass
        solutions.append(basic_expression(expression[:-2], expression[-2], expression[-1]))
    print(sum(solutions))

    data = read_data(data_path)
    data = [[val for val in list(line) if val != ' '] for line in data]
    solutions = list()
    for expression in data:
        while solve_braces(expression, advanced=True):
            pass
        solutions.append(advanced_expression(expression))
    print(sum(solutions))


if __name__ == '__main__':
    main()
