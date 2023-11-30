import os
import numpy as np


def read_data(path):
    with open(path, 'r') as file:
        lines = file.readlines()
    lines = [line.rstrip('\n') for line in lines]
    return lines


def check_dims(states):
    if len(states.shape) == 3:
        x, y, z = [0, 0], [0, 0], [0, 0]
        if np.any(states[0, :, :] > 0):
            z[0] = 1
        if np.any(states[-1, :, :] > 0):
            z[1] = 1
        if np.any(states[:, 0, :] > 0):
            y[0] = 1
        if np.any(states[:, -1, :] > 0):
            y[1] = 1
        if np.any(states[:, :, 0] > 0):
            x[0] = 1
        if np.any(states[:, :, -1] > 0):
            x[1] = 1
        size = states.shape
        new_states = np.zeros(np.add(size, [sum(z), sum(y), sum(x)]), dtype=states.dtype)
        new_states[z[0]:size[0]+z[0], y[0]:size[1]+y[0], x[0]:size[2]+x[0]] = states
    else:
        x, y, z, w = [0, 0], [0, 0], [0, 0], [0, 0]
        if np.any(states[0, :, :, :] > 0):
            w[0] = 1
        if np.any(states[-1, :, :, :] > 0):
            w[1] = 1
        if np.any(states[:, 0, :, :] > 0):
            z[0] = 1
        if np.any(states[:, -1, :, :] > 0):
            z[1] = 1
        if np.any(states[:, :, 0, :] > 0):
            y[0] = 1
        if np.any(states[:, :, -1, :] > 0):
            y[1] = 1
        if np.any(states[:, :, :, 0] > 0):
            x[0] = 1
        if np.any(states[:, :, :, -1] > 0):
            x[1] = 1
        size = states.shape
        new_states = np.zeros(np.add(size, [sum(w), sum(z), sum(y), sum(x)]), dtype=states.dtype)
        new_states[w[0]:size[0]+w[0], z[0]:size[1]+z[0], y[0]:size[2]+y[0], x[0]:size[3]+x[0]] = states
    return new_states


def main():
    base_path = os.path.dirname(os.path.realpath(__file__))
    data_path = os.path.join(base_path, 'data', 'day17_data.txt')
    data = read_data(data_path)
    
    states = list()
    for line in data:
        states.append([0 if char == '.' else 1 for char in line])
    states = np.array(states)
    states_p2 = states.copy()
    states = states.reshape((1, states.shape[0], states.shape[1]))

    cycles = 6
    for _ in range(cycles):
        states = check_dims(states)
        print(f'{states.shape} : {np.sum(states)}')
        new_states = states.copy()
        for z in range(states.shape[0]):
            for row in range(states.shape[1]):
                for col in range(states.shape[2]):
                    area = states[z-(z>0):z+(2*(z<states.shape[0])),
                                  row-(row>0):row+(2*(row<states.shape[1])), 
                                  col-(col>0):col+(2*(col<states.shape[2]))]
                    state = states[z, row, col]
                    active = np.sum(area) - state
                    if state == 1 and not (2 <= active <= 3):
                        new_states[z, row, col] = 0
                    elif state == 0 and active == 3:
                        new_states[z, row, col] = 1
        states = new_states
    print(np.sum(states))

    states = states_p2.reshape((1, 1, states_p2.shape[0], states_p2.shape[1]))
    cycles = 6
    for _ in range(cycles):
        states = check_dims(states)
        print(f'{states.shape} : {np.sum(states)}')
        new_states = states.copy()
        for w in range(states.shape[0]):
            for z in range(states.shape[1]):
                for row in range(states.shape[2]):
                    for col in range(states.shape[3]):
                        area = states[w-(w>0):w+(2*(w<states.shape[0])),
                                      z-(z>0):z+(2*(z<states.shape[1])),
                                      row-(row>0):row+(2*(row<states.shape[2])), 
                                      col-(col>0):col+(2*(col<states.shape[3]))]
                        state = states[w, z, row, col]
                        active = np.sum(area) - state
                        if state == 1 and not (2 <= active <= 3):
                            new_states[w, z, row, col] = 0
                        elif state == 0 and active == 3:
                            new_states[w, z, row, col] = 1
        states = new_states
    print(np.sum(states))


if __name__ == '__main__':
    main()
