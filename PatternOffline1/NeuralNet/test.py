import numpy.random as rand
import math
weight = [rand.uniform(0, 1) for i in range(10)]

def sigmoid(net):
    return 1/(1+math.exp(-net))
x = 1
print(sigmoid(x))

#print(weight)