import inputgenerator
import numpy.random as rand
import visual
import numpy as np
import math
import numpy.linalg as lg



def prob_normal(x, mu, cov):
    xtemp = np.matrix(x)
    mutemp = np.matrix(mu)
    covtemp = np.matrix(cov)

    sub = xtemp - mutemp
    det = lg.det(covtemp)
    invert = lg.inv(covtemp)
    #print(sub.shape, invert.shape, covtemp.shape)
    return math.exp(-0.5* (sub*invert)*sub.transpose()) / (2*math.pi*math.sqrt(det))


def expectation(xs, ys, mulist, covlist, weight, total, k):
    P = [[0 for j in range(k)] for i in range(total)]
   # print(len(P))
    labels = [0 for i in range(total)]

    for i in range(total):

        rowsum = 0
        for j in range(k):
            P[i][j] = weight[j]*prob_normal([xs[i], ys[i]], mulist[j], covlist[j])
            rowsum+= P[i][j]
        for j in range(k):
            P[i][j] /= rowsum

        labels[i] =  P[i].index(max(P[i]))

    # for i in range(total):
    #     print(P[i])
    return P, labels

def maximization(P,xs, ys,  total, k):
    mulist = [[0.0, 0.0] for j in range(k)]
    covlist = [[[0.0, 0.0], [0.0, 0.0]] for j in range(k)]
    weight = [1.0/k]*k
    sumpx = [[0.0, 0.0] for j in range(k)]
    sump = [0.0]*k
    for j in range(k):
        for i in range(total):
            sumpx[j][0]+= P[i][j]*xs[i]
            sumpx[j][1]+= P[i][j]*ys[i]
            sump[j] += P[i][j]

        mulist[j] = [sumpx[j][0] / sump[j], sumpx[j][1] / sump[j]]
        weight[j] = sump[j] / total


    for j in range(k):
        covtemp = np.matrix(covlist[j])
        mutemp = np.matrix(mulist[j])
        for i in range(total):
            xtemp = np.matrix([xs[i], ys[i]])
            sub = xtemp - mutemp
            covtemp+= P[i][j]* sub.transpose()*sub

        covtemp /= sump[j]
        covlist[j] = np.array(covtemp)

    return mulist, covlist, weight

def calculate_ll(xs, ys, mulist, covlist, weight, total, k):
    ll = 0
    for i in range(total):
        summ = 0
        for j in range(k):
            summ+= weight[j]*prob_normal([xs[i], ys[i]], mulist[j], covlist[j])
        ll += math.log(summ)
    return ll


def main():
    SEED = 17
    rand.seed(SEED)
    print('Program started')
    k = 3
    xs, ys = inputgenerator.generate_input()

    #initial guesses----
    print('Guessing the parameters')
    mulist = [None]*k
    covlist = [None]*k
    total = len(xs)

    for i in range(k):
        mulist[i] = [rand.uniform(-10, 10) for j in range(2)]
        covlist[i] = [[rand.uniform(0, 10), 0], [0, rand.uniform(0, 10)]]
    
    improve = 599.0
    tolerance = 0.0001
    ll_old = -100.0
    weight = [1.0/k]*k
    max_iter = 25

    labels = [1]*len(xs)

    print('Starting GMM.. \nTerminating condition: improvement < {} or iters > {}\n'.format(tolerance, max_iter))

    iters = 0
    while improve > tolerance and iters < max_iter:
        iters+=1
        P, labels = expectation(xs, ys, mulist, covlist, weight, total, k)
        mulist, covlist, weight = maximization(P,xs, ys, total, k)
        ll = calculate_ll(xs, ys, mulist, covlist, weight, total, k)
        improve = abs(ll - ll_old)
        ll_old = ll

        print('End of {}th iteration..., improvement: {}'.format(iters, improve))
        if iters%5 == 0:
            visual.plot_image('iteration{}.png'.format(iters), xs, ys, labels, mulist, covlist, k)



    print('GMM ended after {} iterations'.format(iters))
    print('Plotting the final image')
    visual.plot_image('final.png', xs, ys, labels, mulist, covlist, k)
    
    




if __name__ =='__main__':
    main()