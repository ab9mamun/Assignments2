import pandas as pd
import numpy as np
import time

def get_data(filename, size):
    df = pd.read_csv(filename)[:size]
    df = df.pivot_table(columns=['item'], index=['user'], values='rating')
    df = df.fillna(0)
    return df.values




#equivalent iterative version (slower)-------------
# def loss2(X,U,V,lamU,lamV):
#     l = 0
#     for n in range(X.shape[0]):
#       for m in range(X.shape[1]):
#            if X[n, m] != 0:
#                l += (X[n, m] - np.dot(U[n].T, V[m])) ** 2
#     for n in range(X.shape[0]):
#         l+= lamU*np.sum(U[n]**2) ** 0.5
#     for m in range(X.shape[1]):
#         l+= lamV*np.sum(V[m]**2) ** 0.5
#     return l

def loss(X, U, V, lamU, lamV):
    l = np.sum( (X - (X !=0)*np.dot(U, V.T) )**2)
    return l+ lamU*np.sum(np.sum(U**2, axis=1) ** 0.5) + lamV*np.sum(np.sum(V**2, axis=1) ** 0.5)


def als(X, U, V, lamU, lamV):

    for n in range(X.shape[0]):
        s = np.zeros((U.shape[1], U.shape[1]))
        s2 = np.zeros((U.shape[1],1))
        for m in range(X.shape[1]):
            if X[n,m] !=0:
                temp = V[m].T
                temp = temp.reshape((V.shape[1], 1))
                s += np.dot(temp, temp.T)
                #print(s2.shape, temp.shape)
               # print(temp)
                s2 = s2 + X[n,m]* temp
        #s2.reshape((s2.shape[0], 1))
        matToInv = s + lamU * np.identity(U.shape[1])
       # print(matToInv.shape, s.shape, s2.shape)
        U[n] = np.dot(np.linalg.inv(matToInv), s2).T

    for m in range(X.shape[1]):
        s = np.zeros((U.shape[1], U.shape[1]))
        s2 = np.zeros((U.shape[1], 1))
        for n in range(X.shape[0]):
            if X[n, m] != 0:
                temp = V[m].T
                temp = temp.reshape((V.shape[1], 1 ))
                s += np.dot(temp, temp.T)
               # print(s2.shape, temp.shape)
                s2 = s2 + X[n, m] * temp
        #s2.reshape((s2.shape[0], 1))
        matToInv = s + lamV * np.identity(V.shape[1])
        V[m] = np.dot(np.linalg.inv(matToInv), s2).T

    #print(X.shape, U.shape, V.shape)
    return U, V


def trainUV(X):
    k = 10
    lamU = 0.1
    lamV = 0.1
    thresh = 0.1
    l = 100
    prev = 50
    V = np.random.randn(X.shape[1], k)
    U = np.zeros((X.shape[0], k))
    iters = 0
    start = time.time()
    while abs(l - prev) >thresh and iters < 200:
        prev = l
        iters+=1
        U, V = als(X, U, V, lamU, lamV)
        l = loss(X, U, V, lamU, lamV)

        if iters%10==0:
            end = time.time()
            print('Iter: {} Loss:{} Time {} secs'.format(iters, l, end - start))
            #start = end

    return U, V


def main():

    np.random.seed(1)
    train = get_data('train.csv', 60)
    validate = get_data('validation.csv', 20)
    test = get_data('test.csv', 20)
    print(train.shape, validate.shape, test.shape)

    start = time.time()
    trainUV(train)
    end = time.time()
    print('Time taken: {} seconds'.format(end-start))



if __name__=='__main__':
    main()