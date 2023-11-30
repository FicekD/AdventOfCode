import os


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day05_data.txt')
    data = read_data(data_path)

    # rows = [int(line[:7].replace('F', '0').replace('B', '1'), 2) for line in data]
    # cols = [int(line[7:].replace('L', '0').replace('R', '1'), 2) for line in data]
    # seats = [8*row + col for row, col in zip(rows, cols)]
    seats = list()
    for line in data:
        encoded = line.replace('F', '0').replace('B', '1').replace('L', '0').replace('R', '1')
        row = int(encoded[:7], 2)
        col = int(encoded[7:], 2)
        seat = 8*row + col
        seats.append(seat)
    print(max(seats))
    seats.sort()
    for i, seat in enumerate(seats[1:-1]):
        if seat - seats[i] == 2:
            print(seat - 1)



if __name__ == '__main__':
    main()
