import pandas as pd
import numpy as np

np.random.seed(1)
V = np.random.randn(100, 1)
choice = {'V':V}
np.savetxt('save.txt', choice['V'])
#]) #choice['V'])

Vagain = np.loadtxt('save.txt')
print(Vagain)
print(np.sum(V - Vagain))