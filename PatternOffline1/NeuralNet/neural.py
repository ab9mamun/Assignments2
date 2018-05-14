import numpy
import numpy.random as rand

#supporitng functions ---
def br():
    print('----------------------------------------------')



class Neuron:
    def __init__(self, w_length):
        self.weight = [rand.uniform(0, 1)]*w_length
        self.delta = 0
        self.output = 0

    def print(self, header):
        print(header,  self.weight)


class Layer:
    def __init__(self, num_neuron, w_length):  #w_length = num_neuron_prev + 1 for bias
        self.num_neuron = num_neuron
        self.w_length = w_length
        self.neurons = [None]*num_neuron
        for i in range(num_neuron):
            self.neurons[i] = Neuron(w_length)

    def print(self, header):
        print(header)
        br()
        print('Number of neurons: ', self.num_neuron)
        for j in range(self.num_neuron):
            self.neurons[j].print('Neuron '+ str(j) + ':')


class Network:
    def __init__(self, num_attributes, num_classes, num_Hlayer, num_neurons):
        self.Hlayers = [None]*num_Hlayer
        self.outputLayer = None
        self.num_attributes = num_attributes
        self.num_classes = num_classes

        self.Hlayers[0] = Layer(num_neurons[0], num_attributes+1)
        for i in range(1, num_Hlayer):
            self.Hlayers[i] = Layer(num_neurons[i], num_neurons[i-1]+1)

        self.outputLayer = Layer(num_classes, num_neurons[num_Hlayer-1]+1)

        self.num_Hlayer = num_Hlayer
        self.num_neurons = num_neurons.copy()

    def print(self):
        br()
        print("Printing Neural Network")
        br()
        print('Number of hidden layers: ', self.num_Hlayer)
        for i in range(self.num_Hlayer):
            self.Hlayers[i].print('Hidden layer '+str(i)+":")

        self.outputLayer.print('Output layer:')
        br()


    def train(self, data):
        pass





def main():
    rand.seed(1112)

    input_ = numpy.loadtxt('input.txt', dtype=int)
    #print(input_)
    dataset = numpy.loadtxt('data.txt', dtype=int)
    #print(data)
    num_attributes = input_[0]
    num_classes = input_[1]
    num_Hlayer = input_[2]
    num_neurons = [0]*num_Hlayer
    for i in range(num_Hlayer):
        num_neurons[i] = input_[i+3]

    print('Number of attributes:', num_attributes)
    print('Number of classes:', num_classes)
    print('Number of hidden layers:', num_Hlayer)
    print('Number of neurons in each hidden layer:', num_neurons)

    network = Network(num_attributes, num_classes, num_Hlayer, num_neurons)
    network.train(dataset)
    network.print()


if __name__ == '__main__':
    main()
