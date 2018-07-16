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
    np.random.seed(22)

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

    X_seq = X_seq.reshape((N,))
    print('X_seq:')
    print(X_seq, '\n')

    X_clusters = [list() for i in range(M)]
    p_prior = np.zeros((M,))
    p_trans = np.zeros((M,M))

    k = binToDec(I_seq[0])
    p_prior[k] +=1

    for i in range(1, N):
        k_1 = binToDec(I_seq[i-1])
        k = binToDec(I_seq[i])
        X_clusters[k].append(X_seq[i])
        p_trans[k_1][k] +=1
        p_prior[k] +=1

    p_prior /= np.sum(p_prior)  #divide by sum of all
    p_trans /= np.sum(p_trans, axis=1).reshape((M,1))  #divide by row-wise sum

    print('Prior probability:')
    print(p_prior, '\n')

    print('Transition probablility:')
    print(p_trans,'\n')
    #print(X_clusters)

    X_clusters_mean = [0]*M
    for i in range(M):
        X_clusters_mean[i] = np.average(X_clusters[i])

    print('X_Cluster means:')
    print(X_clusters_mean)

    



if __name__ == '__main__':
    main()