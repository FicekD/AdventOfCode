import os
import copy


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def decode_data(data):
    player1 = list()
    player2 = list()
    for line in data:
        if line == '':
            continue
        elif line == 'Player 1:':
            player = player1
        elif line == 'Player 2:':
            player = player2
        else:
            player.append(int(line))
    return player1, player2


def game(player1, player2):
    # print('New Game')
    player1_cache = list()
    player2_cache = list()
    while len(player1) != 0 and len(player2) != 0:
        result = play_round(player1, player2, player1_cache, player2_cache)
        if result == 'end':
            break
        elif result == 'p1':
            player1.append(player1[0])
            player1.append(player2[0])
        elif result == 'p2':
            player2.append(player2[0])
            player2.append(player1[0])
        player1.pop(0)
        player2.pop(0)
    return ('p1', player1) if result == 'end' else (result, (player1 if result == 'p1' else player2))


def play_round(player1, player2, player1_cache, player2_cache):
    # print(f'New round\nPlayer 1\'s deck: {player1}\nPlayer 2\'s deck: {player2}')
    if player1 in player1_cache or player2 in player2_cache:
        # print('Repeated sequence reached. Player 1 wins.')
        return 'end'
    player1_cache.append(copy.copy(player1))
    player2_cache.append(copy.copy(player2))
    if (len(player1) - 1) >= player1[0] and (len(player2) - 1) >= player2[0]:
        return game(copy.copy(player1[1:player1[0]+1]), copy.copy(player2[1:player2[0]+1]))[0]
    # print('Player 1 wins!' if player1[0] > player2[0] else 'Player 2 wins!')
    return 'p1' if player1[0] > player2[0] else 'p2'


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day22_data.txt')
    data = read_data(data_path)

    player1, player2 = decode_data(data)
    while len(player1) != 0 and len(player2) != 0:
        if player1[0] > player2[0]:
            player1.append(player1[0])
            player1.append(player2[0])
        else:
            player2.append(player2[0])
            player2.append(player1[0])
        player1.pop(0)
        player2.pop(0)

    print(sum([i*val for i, val in enumerate(reversed(player1), start=1)]))
    print(sum([i*val for i, val in enumerate(reversed(player2), start=1)]))

    player1, player2 = decode_data(data)
    w, p = game(player1, player2)
    print('Crab wins' if w == 'p1' else 'I defended my honor!')
    print(sum([i*val for i, val in enumerate(reversed(p), start=1)]))


if __name__ == '__main__':
    main()
