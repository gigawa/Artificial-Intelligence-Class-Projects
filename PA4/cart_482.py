#!/usr/bin/python

import argparse
import logging
import sys

import numpy as np

import gym
import gym.scoreboard.scoring
from gym import wrappers

def discretize_state( x, xdot, theta, thetadot ):
    one_degree = 0.0174532
    six_degrees = 0.1047192
    twelve_degrees = 0.2094384
    fifty_degrees = 0.87266

    box = 0
    if x < -2.4 or x > 2.4 or theta < -twelve_degrees or theta > twelve_degrees:
        return -1

    if x < -0.08:
        box = 0
    elif x < 0.08:
        box = 1
    else:
        box = 2

    if xdot < -0.5:
        box += 0
    elif xdot < 0.5:
        box +=3
    else:
        box +=6

    if theta < -six_degrees:
        box += 0
    if theta < -one_degree:
        box += 9
    elif theta < 0:
        box +=18
    elif theta < one_degree:
        box += 27
    elif theta < six_degrees:
        box += 36
    else:
        box += 45

    if thetadot < -fifty_degrees:
        box += 0
    elif thetadot < fifty_degrees:
        box += 54
    else:
        box += 108

    return box

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=None)
    parser.add_argument('env_id', nargs='?', default='CartPole-v0', help='Select the environment to run')
    args = parser.parse_args()

    # Call `undo_logger_setup` if you want to undo Gym's logger setup
    # and configure things manually. (The default should be fine most
    # of the time.)
    gym.undo_logger_setup()
    logger = logging.getLogger()
    formatter = logging.Formatter('[%(asctime)s] %(message)s')
    handler = logging.StreamHandler(sys.stderr)
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    # You can set the level to logging.DEBUG or logging.WARN if you
    # want to change the amount of output.
    logger.setLevel(logging.INFO)

    env = gym.make(args.env_id)
    outdir = '/tmp/' + 'qagent' + '-results'
    env = wrappers.Monitor(env, outdir, write_upon_reset=True, force=True)

    env.seed(0)
    Q = np.zeros([161, 2])

    alpha = 0.8
    gamma = 0.75

    n_episodes = 50001
    for episode in range(n_episodes):
        reward = 0
        done = False
        state = env.reset()
        s = discretize_state(state[0], state[1], state[2], state[3])
        while done != True:
            if Q[s][1] >= Q[s][0]:
                action = 1
            else:
                action = 0
            state2, reward, done, info = env.step(action)
            sprime = discretize_state(state2[0], state2[1], state2[2], state2[3])
            predicted_value = np.max(Q[sprime])
            if sprime < 0:
                predicted_value = 0
                reward = -5

            Q[s,action] += alpha * (reward + (gamma * predicted_value) - Q[s,action])

            s = sprime
            if episode % 1000 == 0:
                alpha *= .996

            #env.render()

        print gym.scoreboard.scoring.score_from_local(outdir)
