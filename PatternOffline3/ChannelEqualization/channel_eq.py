import numpy as np
import math


def binToDec(arr):
    n = len(arr)
    d = 0
    for i in range(n):
        d |= (arr[i] << i)
    return d


def main():
    print('Hello world')

    with open('config.txt') as file:
        n, _ = file.readline().split()
        n = int(n)
        weights = list(map(float, file.readline().split()))
        noise_variance = float(file.readline())

    with open('train.txt') as file:
        train_bits = list(map(int, file.readline()))
    with open('test.txt') as file:
        test_bits = list(map(int, file.readline()))


    ##---------------end of reading inputs------------------------
    M = 2**n   ##number of clusters
    N = len(train_bits) - n +1  #number of training samples---
    sequences = []  #I sequences
    for i in range(N):
        sequences.append(train_bits[i:i+n])

    noises = np.random.normal(loc=0.0, scale=math.sqrt(noise_variance), size=(N,1))

    I_seq = np.array(sequences).reshape((N, n))
    W = np.array(weights).reshape((n,1))
    X_seq = np.dot(I_seq, W) + noises

    print(X_seq)

    #X_clusters = [[] for i in range()]
    ##X_clusters_mean = np.zeros()
    #for i in range(N):
     #   X_clusters[binToDec(I_seq[i])].append(X_seq[i][0])

    #for i in range(M):

if __name__ == '__main__':
    main()