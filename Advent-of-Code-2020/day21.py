import os
import copy


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day21_data.txt')
    data = read_data(data_path)

    pairs = list()
    for line in data:
        f, a = line[:-1].split(' (contains ')
        pairs.append((set(f.split(' ')), set(a.split(', '))))
    # pairs.sort(key=lambda x: len(x[1]))
    pairs2 = copy.deepcopy(pairs)

    while True:
        inspected = list()
        alergen = None
        for ingredients, alergens in pairs:
            if len(inspected) == 0 and len(alergens) == 1:
                alergen = next(iter(alergens))
                break
        else:
            break
        for ingredients, alergens in pairs:
            if alergen is not None and alergen in alergens:
                inspected.append(ingredients)
        inter = set.intersection(*inspected)
        for ingredients, alergens in pairs:
            for val in inter:
                if val in ingredients:
                    ingredients.remove(val)
            try:
                alergens.remove(alergen)
            except KeyError:
                continue
    res = 0
    for ingredients, alergens in pairs:
        res += len(ingredients)
    print(res)

    food_dict = {}
    while True:
        inspected = list()
        alergen = None
        for ingredients, alergens in pairs2:
            if len(inspected) == 0 and len(alergens) == 1:
                alergen = next(iter(alergens))
                break
        else:
            break
        for ingredients, alergens in pairs2:
            if alergen in alergens:
                inspected.append(ingredients)
        inter = set.intersection(*inspected)
        food_dict[alergen] = inter
        for ingredients, alergens in pairs2:
            try:
                alergens.remove(alergen)
            except KeyError:
                continue
    done = list()
    while True:
        for k in food_dict.keys():
            if k in done:
                continue
            if len(food_dict[k]) == 1:
                for k_n in food_dict.keys():
                    if k_n == k:
                        continue
                    try:
                        food_dict[k_n].remove(next(iter(food_dict[k])))
                    except KeyError:
                        continue
                done.append(k)
                break
        else:
            break
    a = [(k, next(iter(v))) for k, v in food_dict.items()]
    a.sort(key=lambda x: x[0])
    res = ''.join(str(v) + ',' for k, v in a)
    print(res.rstrip(','))


if __name__ == '__main__':
    main()
