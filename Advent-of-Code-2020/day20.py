import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def compatible(piece1, piece2):
    return (np.all(piece1[:, -1] == piece2[:, 0])) or (np.all(piece1[:, 0] == piece2[:, -1])) or \
           (np.all(piece1[-1, :] == piece2[0, :])) or (np.all(piece1[0, :] == piece2[-1, :]))


def compatible_side(piece1, piece2):
    if np.all(piece1[:, -1] == piece2[:, 0]): return 'l'
    if np.all(piece1[:, 0] == piece2[:, -1]): return 'r'
    if np.all(piece1[-1, :] == piece2[0, :]): return 't'
    if np.all(piece1[0, :] == piece2[-1, :]): return 'b'
    return None


def eligible(piece, board, x, y, side, tile_size):
    iy = tile_size*y
    ix = tile_size*x
    return (True if x == 0 or np.isnan(board[iy, ix-1]) else np.all(board[iy:iy+tile_size, ix-1] == piece[:, 0])) and \
           (True if y == 0 or np.isnan(board[iy-1, ix]) else np.all(board[iy-1, ix:ix+tile_size] == piece[0, :])) and \
           (True if x == side-1 or np.isnan(board[iy, ix+tile_size+1]) else np.all(board[iy:iy+tile_size, ix+tile_size+1] == piece[:, 0])) and \
           (True if y == side-1 or np.isnan(board[iy+tile_size+1, ix]) else np.all(board[iy+tile_size+1, ix:ix+tile_size] == piece[0, :]))


def solve(board, ids, tiles, x, y, side, tile_size, rotate, flip):
    if side*side == np.sum(ids != 0):
        return True
    for key, tile in tiles.items():
        if key in ids:
            continue
        for rot in rotate:
            for fl in flip:
                tformed = fl(rot(tile))
                if eligible(tformed, board, x, y, side, tile_size):
                    iy = tile_size*y
                    ix = tile_size*x
                    board[iy:iy+tile_size, ix:ix+tile_size] = tformed
                    ids[y, x] = key
                    x = (x+1) % side
                    y = y + (x == 0)
                    if solve(board, ids, tiles, x, y, side, tile_size, rotate, flip):
                        return True
                    else:
                        x = (x-1) % side
                        y = y - (x == side-1)
                        board[iy:iy+tile_size, ix:ix+tile_size] = np.nan
                        ids[y, x] = 0
                        continue
    else:
        return False


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day20_data.txt')
    data = read_data(data_path)
    data.append('')

    tiles = {}
    for line in data:
        if line == '':
            tiles[key] = np.array(tile)
        if line.startswith('Tile'):
            key = int(line[:-1].split(' ')[1])
            tile = list()
        else:
            tile.append([0 if char == '.' else 1 for char in line])
    # tiles = dict(sorted(tiles.items(), key=lambda x: x[0]))
    side = int(np.sqrt(len(tiles.keys())))
    tile_size = next(iter(tiles.values())).shape[0]
    board = np.full((side*tile_size, side*tile_size), np.nan)
    ids = np.zeros((side, side), dtype=np.uint16)

    rotate = [
        lambda x: x,
        lambda x: np.rot90(x, 1),
        lambda x: np.rot90(x, 2),
        lambda x: np.rot90(x, -1),
    ]
    flip = [
        lambda x: x,
        lambda x: x[:, ::-1],
        lambda x: x[::-1, :],
    ]

    res = list()
    for key1, tile1 in tiles.items():
        nums = 0
        for key2, tile2 in tiles.items():
            if key1 == key2:
                continue
            br = False
            for r in rotate:
                for f in flip:
                    tformed = f(r(tile2))
                    if compatible(tile1, tformed):
                        nums += 1
                        br = True
                        break
                if br:
                    break
        if nums == 2:
            res.append(key1)
            k = ''
            for key2, tile2 in tiles.items():
                if key1 == key2:
                    continue
                br = False
                for r in rotate:
                    for f in flip:
                        tformed = f(r(tile2))
                        ret = compatible_side(tile1, tformed)
                        if ret is not None:
                            k += ret
                            br = True
                            break
                    if br:
                        break
    print(res[0]*res[1]*res[2]*res[3])
    
    board[0:tile_size, 0:tile_size] = tiles[res[1]]
    ids[0, 0] = res[1]

    solved = solve(board, ids, tiles, 1, 0, side, tile_size, rotate, flip)
    ids = ids.astype(np.uint64)
    print(ids)

    stripped_board = np.zeros((side*(tile_size-2), side*(tile_size-2)))
    for i in range(side):
        for j in range(side):
            stripped_board[i*(tile_size-2):i*(tile_size-2)+(tile_size-2),
                           j*(tile_size-2):j*(tile_size-2)+(tile_size-2)] = board[i*tile_size+1:i*tile_size+tile_size-1,
                                                                                  j*tile_size+1:j*tile_size+tile_size-1]
    board = stripped_board.astype(np.bool)

    raw_monster = '                  # \n#    ##    ##    ###\n #  #  #  #  #  #   '
    monster = np.array([[0 if char == ' ' else 1 for char in line] for line in raw_monster.split('\n')], dtype=np.bool)
    ones = int(np.sum(monster))
    
    for rot in rotate:
        for fl in flip:
            tformed = fl(rot(board))
            found = 0
            for i in range(tformed.shape[0]):
                for j in range(tformed.shape[1]):
                    area = tformed[i:i+monster.shape[0], j:j+monster.shape[1]]
                    if area.shape != monster.shape:
                        continue
                    if int(np.sum(np.logical_and(area, monster))) == ones:
                        found += 1
                        area[:, :] = np.logical_and(area, np.logical_not(monster))
            if found != 0:
                print(np.sum(tformed))
                return


if __name__ == '__main__':
    main()
