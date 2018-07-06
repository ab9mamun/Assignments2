import pandas as pd
import numpy as np
import time

def get_data():
    train = pd.read_excel("ratings_train.xlsx", header=None)
    validate = pd.read_excel('ratings_validate.xlsx', header=None)
    #test = pd.read_excel('ratings_test.xlsx', header=None)
    return train.values, validate.values, None #test.values




#RMSE loss-------------
def RMSE(X,U,V):
    l = 0
    count = 0
    for n in range(X.shape[0]):
      for m in range(X.shape[1]):
           if X[n, m] != -1:
               l += (X[n, m] - np.dot(U[n].T, V[m])) ** 2
               count+=1

    return (l/count)**0.5


def loss(X,U,V,lamU,lamV):
    l = 0
    for n in range(X.shape[0]):
      for m in range(X.shape[1]):
           if X[n, m] != -1:
               l += (X[n, m] - np.dot(U[n].T, V[m])) ** 2

    for n in range(X.shape[0]):
        l+= lamU*np.sum(U[n]**2) ** 0.5
    for m in range(X.shape[1]):
        l+= lamV*np.sum(V[m]**2) ** 0.5

    return l

#def loss(X, U, V, lamU, lamV):
#    l = np.sum( (X - (X !=0)*np.dot(U, V.T) )**2)
#    return l+ lamU*np.sum(np.sum(U**2, axis=1) ** 0.5) + lamV*np.sum(np.sum(V**2, axis=1) ** 0.5)

def alsU(X, U, V, lamU):
    if U is None:
        U = np.zeros((X.shape[0], V.shape[1]))
    for n in range(X.shape[0]):
        s = np.zeros((U.shape[1], U.shape[1]))
        s2 = np.zeros((U.shape[1],1))
        for m in range(X.shape[1]):
            if X[n,m] !=-1:
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
        return U

def als(X, U, V, lamU, lamV):
    U = alsU(X, U, V, lamU)
    for m in range(X.shape[1]):
        s = np.zeros((U.shape[1], U.shape[1]))
        s2 = np.zeros((U.shape[1], 1))
        for n in range(X.shape[0]):
            if X[n, m] != -1:
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


def trainUV(X, k, lamU, lamV):
    thresh = 0.1
    l = 100
    prev = 50
    V = np.random.randn(X.shape[1], k)
    U = np.zeros((X.shape[0], k))
    iters = 0
    start = time.time()
    while abs(l - prev) >thresh and iters < 50:
        prev = l
        iters+=1
        U, V = als(X, U, V, lamU, lamV)
        l = loss(X, U, V, lamU, lamV)

        if iters%10==0:
            #end = time.time()
            print('Still training...')
            #start = end

    return U, V


def main():
    start = time.time()
    np.random.seed(1)

    train, validate, test = get_data() #'train.csv', 'validation.csv', 'test.csv')
    #validate = get_data('validation.csv', 20)
    #test = get_data('test.csv', 20)
    print(train.shape, validate.shape)

    #start = time.time()
    l = float('inf')
    choice = {}
    results = {}


    for lam in [0.01, 0.1, 1, 10]:
        for k in [10, 20, 40]:
            print('Training for lam: {} k: {}'.format(lam, k))
            U, V = trainUV(train, k, lam, lam)
            templ = RMSE(train, U, V)
            results['train_'+str(lam)+' '+str(k)] = templ

            U_val = alsU(validate, None, V, lam)
            templ = RMSE(validate, U_val, V)
            results['validate_' + str(lam) + ' ' + str(k)] = templ
            if templ < l:
                l = templ
                choice = {'U': U, 'V': V, 'k':k, 'lam': lam}


    print('Choice: ', choice['lam'], choice['k'])
    print(results)

    end = time.time()
    print('Time taken: {} seconds'.format(end-start))




if __name__=='__main__':
    main()