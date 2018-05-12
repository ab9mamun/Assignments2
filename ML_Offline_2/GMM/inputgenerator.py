import numpy
import numpy.random as rand
import matplotlib.pyplot as plt

def generate_input():
    SEED = 34
    rand.seed(SEED)
    k = rand.randint(3, 10)
    total = 1000 - 1000%k  + k #the next integer divisible by k
    xs = []
    ys = []
    labels = []

    for i in range (k):
        mu = [rand.uniform(-10, 10), rand.uniform(-10, 10)]
       # print(mu)
        cov = [ [rand.uniform(0, 10/k),0], [0, rand.uniform(0.1, 5/k)] ]
        #print(cov)
        x, y = numpy.random.multivariate_normal(mu, cov,total//k ).T
        xs = numpy.concatenate((xs, x))
        ys = numpy.concatenate((ys, y))
        labels = labels+[i]*(total//k)

    fig = plt.figure()
    plt.scatter(xs, ys, 20, c=labels)
    fig.savefig("true-map.png")

    with open('input.txt', 'w') as file:
        file.write(str(k)+'\n')
        file.write(str(total)+'\n')
        for i in range(total):
            file.write(str(xs[i])+' ' + str(ys[i])+'\n')
        file.close()