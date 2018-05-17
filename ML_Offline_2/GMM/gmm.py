import inputgenerator
import numpy.random as rand

def main():
    SEED = 17
    rand.seed(SEED)
    print('GMM starting')
    xs, ys, k = inputgenerator.generate_input()


if __name__ =='__main__':
    main()