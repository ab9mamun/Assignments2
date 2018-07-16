import numpy as np
import math
import scipy.stats

def sep():
    print('============================================')
def br():
    print()

def binToDec(arr):
    n = len(arr)
    d = 0
    for i in range(n):
        d |= (arr[i] << i)
    return d

def MSB(num, n):
    return int(np.binary_repr(num, width=n)[0])

def process_bits(bits, n, W, noise_stddev):
    N = len(bits) - n + 1  # number of test samples---
    sequences = []  # I sequences
    for i in range(N):
        sequences.append(bits[i:i + n])

    I = np.array(sequences).reshape((N, n))
    noises = np.random.normal(loc=0.0, scale=noise_stddev, size=(N, 1))
    X = np.dot(I, W) + noises
    return I, X.reshape((N,)), N


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
    W = np.array(weights).reshape((n, 1))
    noise_stddev = math.sqrt(noise_variance)

    I_seq, X_seq, N = process_bits(train_bits, n,W,  noise_stddev)
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

    ##-------------------now time for testing===================

    I_test, X_test, N_test = process_bits(test_bits, n, W, noise_stddev)

    #viterbi starts----
    #initialization------------
    MLE = np.zeros((N_test, M)) #stores the likelihood
    x = X_test[0]
    for j in range(M): #for each cluster---- find the cost for first bit
        #ll = p(wj)*p(x|wj)
        MLE[0][j] = math.log(p_prior[j]*scipy.stats.norm.pdf(x, X_clusters_mean[j], noise_stddev))

    parents = [[None]*M for i in range(N_test)]

    #main loop-------------
    for i in range(1, len(X_test)): #for length 1 to last--
        x = X_test[i]
        for j in range(M):
            p_x_given_wj = scipy.stats.norm.pdf(x, X_clusters_mean[j], noise_stddev)

            max_ll = -float('inf') #maximum log likelihood
            for k in range(M):
                if p_trans[k][j] == 0:
                    continue

                ll = MLE[i - 1][k] + math.log(p_x_given_wj * p_trans[k][j])
                if ll > max_ll:
                    max_ll= ll
                    parents[i][j] = k
                    MLE[i][j] = ll


    predicted_clusters = [0]*N_test
    j = np.argmax(MLE[-1])
    predicted_clusters[-1] = j

    for i in range(N_test-1).__reversed__():
        j = parents[i+1][j]  #parent of the previous one
        predicted_clusters[i] = j

    predicted_bits = []
    for cluster in predicted_clusters:
        predicted_bits.append(MSB(cluster, n)) #last bit of cluster


    actual_bits = np.array(test_bits[n-1:]).reshape((1,N_test))
    predicted_bits = np.array(predicted_bits).reshape((1, N_test))

    incorrect = np.sum(np.abs(actual_bits - predicted_bits))
    accuracy = (N_test - incorrect)/N_test

    br()
    br()
    print('Actual bits \t:\t', actual_bits.reshape(N_test,))
    print('Predicted bits\t:\t', predicted_bits.reshape(N_test,))

    sep()
    print('Accuracy\t\t:\t\t{}%'.format(accuracy*100))
    sep()

if __name__ == '__main__':
    main()