import os


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day08_data.txt')
    data = read_data(data_path)

    done = list()
    acc = 0
    pc = 0
    while True:
        if pc in done:
            break
        done.append(pc)
        line = data[pc]
        ins = line[:3]
        arg = int(line[4:])
        if ins == 'acc':
            acc += arg
            pc += 1
        elif ins == 'jmp':
            pc += arg
        else:
            pc += 1
        if pc == len(data):
            print('Last instruction')
            break
    print(acc)

    for i in range(len(data)):
        if data[i][:3] == 'acc':
            continue
        done = list()
        acc = 0
        pc = 0
        while True:
            if pc in done:
                break
            done.append(pc)
            line = data[pc]
            ins = line[:3]
            arg = int(line[4:])
            if ins == 'acc':
                acc += arg
                pc += 1
            elif ins == 'jmp':
                if pc == i:
                    pc += 1
                else:
                    pc += arg
            else:
                if pc == i:
                    pc += arg
                else:
                    pc += 1
            if pc == len(data):
                print(acc)
                return


if __name__ == '__main__':
    main()
