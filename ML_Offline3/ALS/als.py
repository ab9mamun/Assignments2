import pandas as pd
import numpy as np
import time

def get_data(filename):
    df = pd.read_csv(filename)
    df = df.pivot_table(columns=['item'], index=['user'], values='rating')
    df = df.fillna(0)
    return df.values



def loss(X, U, V, lamU, lamV):
    l = 0
    for n in range(X.shape[0]):
        for m in range(X.shape[1]):
            if X[n,m] !=0:
                l += ( X[n,m] - np.dot(U[n].T, V[m]) )**2

    for n in range(X.shape[0]):
        l+= np.sum(U[n]**2) ** 0.5
    for m in range(X.shape[1]):
        l+= np.sum(V[m]**2) ** 0.5

    return l

def trainUV(X):
    k = 10
    lamU = 0.1
    lamV = 0.1

    V = np.random.randn(X.shape[1], k)
    U = np.random.randn(X.shape[0], k)
    l = loss(X, U, V, lamU, lamV)
    print(l)

    return U, V


def main():
    start = time.time()
    np.random.seed(1)
    train = get_data('train.csv')
    validate = get_data('validation.csv')
    test = get_data('test.csv')
    print(train.shape, validate.shape, test.shape)

    trainUV(train)

    end = time.time()
    print('Time taken: {} seconds'.format(end-start))



if __name__=='__main__':
    main()