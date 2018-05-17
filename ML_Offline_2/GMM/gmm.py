import inputgenerator
import numpy.random as rand
import matplotlib.pyplot as plt


def expectation():
    pass

def maximization():
    pass

def calculate_ll():
    return 0.0


def plot_image(filename, xs, ys, labels):
    fig = plt.figure()
    plt.scatter(xs, ys, c= labels)
    fig.savefig(filename)


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

    for i in range(k):
        mulist[i] = [rand.uniform(-10, 10) for j in range(2)]
        covlist[i] = [[rand.uniform(0, 10), 0], [0, rand.uniform(0, 10)]]
    
    improve = 599.0
    tolerance = 0.0001
    ll_old = -100.0

    labels = [1]*len(xs)

    print('Starting GMM')

    iters = 0
    while improve > tolerance:
        iters+=1
        expectation()
        maximization()
        ll = calculate_ll()
        improve = abs(ll - ll_old)
        ll_old = ll

        if iters%10 == 0:
            print('End of {}th iteration..., improvement: {}'.format(iters, improve))
            plot_image('iteration{}.png'.format(iters), xs, ys, labels)



    print('GMM ended')
    plot_image('final.png', xs, ys, labels)
    
    




if __name__ =='__main__':
    main()