# kf.py updated file for robot estimation assignment
# pset[3]
# (C) 2017 David Feil-Seifer


import numpy as np
import math
from scipy import stats
import scipy.stats

# kf_update: update state estimate [u, sigma] with new control [xdot] and measurement [z]
# 	parameters:
#			u : 2x1 vector with state estimate (x) at time t-1 and control (xdot) at time t
#			sigma: 2x2 matrix with covariance at time t-1
#			z (int): observed (uncertain) measurement of state (x) at time t
#	returns: [u sigma] updated state with estimate at time t

def kf_update(u, sigma, z):
	deltaT = 1
	F = np.matrix([[1, deltaT], [0, 1]])
	#print(sigma)
	#sigmaX = np.matrix([[100, 0], [0, 100]])
	sigmaX = sigma
	sigmaZ = np.matrix([[5, 0], [0, 5]])
	H = np.matrix([[1, 0], [0,0]])
	FsigmaFTS = np.dot(np.dot(F, sigma), F.T) + sigmaX
	inverse = np.linalg.inv((np.dot((np.dot(H, FsigmaFTS)), H.T)) + sigmaZ)
	first = np.dot(FsigmaFTS, H.T)
	K = np.dot(first, inverse)
	Fu = np.dot(F, u)
	HFu = np.dot(H, Fu)
	error = z - HFu
	u2 = Fu + np.dot(K, error)
	I = np.identity(2)
	sigma2 = np.dot((I - np.dot(K, H)), FsigmaFTS)
	return [u2, sigma2]


# door_update: update estimate of door locations
# 	parameters:
#			u : 2x1 vector with state estimate (x) at time t-1 and control (xdot) at time t-1
#			sigma: 2x2 matrix with covariance at time t-1
#			d (binary): door sensor at time t-1
#			door_dist (array of size 10): probability (0..1) that a door exists at each location (0..9)
#	returns: [door_dist] updated door distribution

def door_update(u, sigma, d, door_dist):

	x = int (u[0]/100)
	p = door_dist[x]
	if d:
		p += (p * 0.6);
		if p > 1:
			p = 0.99
	else:
		p -= (p * 0.2);
		if p < 0:
			p = 0.01

	door_dist[x] = p
	return door_dist
