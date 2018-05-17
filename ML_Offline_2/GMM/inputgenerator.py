import numpy
import numpy.random as rand
import matplotlib.pyplot as plt
import sklearn.datasets as sk

def normalize(arr):
    n = len(arr)
    summ = sum(arr)
    for i in range(n):
        arr[i] /= summ

def generate_input():
    k = 3
    print('Initializing Parameters')
    total = 1000
    xs = []
    ys = []
    labels = []
    mulist = [[5.0, -2.0], [-7.1, 8.2], [-8.5, -7.2]]
    covlist = [[[7.0, 0], [0, 3.2]],[ [2.8, 0],[0, 6.3]], [[6.2, 0], [0, 10.1]] ]

    weightlist = [1.0/k for i in range(k)]
    dists = [i for i in range(k)]

    print('Generating datapoints')
    for i in range (total):
        
        z = rand.choice(dists, p = weightlist)
        x, y = numpy.random.multivariate_normal(mulist[z], covlist[z],1).T
        xs = numpy.concatenate((xs, x))
        ys = numpy.concatenate((ys, y))
        labels.append(z)

    fig = plt.figure()
    plt.scatter(xs, ys, 20, c=labels)
    print('Creating true image')
    fig.savefig("true-map.png")

    with open('input.txt', 'w') as file:
        file.write(str(k)+'\n')
        file.write(str(total)+'\n')
        for i in range(total):
            file.write(str(xs[i])+' ' + str(ys[i])+'\n')
        file.close()

    print('Forgetting parameters. Returning datapoints')
    return xs, ys, k