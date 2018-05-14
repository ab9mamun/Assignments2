import math
import numpy
import numpy.random as rand
import copy

#supporitng functions ---
def br():
    print('----------------------------------------------')

def sigmoid(net):
    return 1/(1+math.exp(-net))

def loss(out, target):
    return 0.5*(target - out)**2




class Neuron:
    def __init__(self, w_length):
        self.weight = [rand.uniform(0, 1)-0.5 for i in range(w_length)]
        self.delta = 0
        self.output = 0
        self.target = 0
        self.tempweight = [0 for i in range(w_length)]

    def print(self, header):
        print(header,  self.weight)

    def setDelta(self, delta):
        self.delta = delta
    def setOutput(self, output):
        self.output = output
    def setTarget(self, target):
        self.target = target


class Layer:
    def __init__(self, num_neuron, w_length):  #w_length = num_neuron_prev + 1 for bias
        self.num_neuron = num_neuron
        self.w_length = w_length
        self.neurons = [None for i in range(num_neuron)]
        for i in range(num_neuron):
            self.neurons[i] = Neuron(w_length)

    def print(self, header):
        br()
        print(header)
        br()
        print('Number of neurons: ', self.num_neuron)
        for j in range(self.num_neuron):
            self.neurons[j].print('Neuron '+ str(j) + ':')


class Network:
    def __init__(self, num_attributes, num_classes, num_Hlayer, num_neurons):
        self.Hlayers = [None for i in range(num_Hlayer)]
        self.outputLayer = None
        self.num_attributes = num_attributes
        self.num_classes = num_classes

        if num_Hlayer >0 :
            self.Hlayers[0] = Layer(num_neurons[0], num_attributes+1)
            for i in range(1, num_Hlayer):
                self.Hlayers[i] = Layer(num_neurons[i], num_neurons[i-1]+1)

        if num_Hlayer >0:
            self.outputLayer = Layer(num_classes, num_neurons[num_Hlayer-1]+1)
        else:
            self.outputLayer = Layer(num_classes,num_attributes + 1)

        self.num_Hlayer = num_Hlayer
        self.num_neurons = copy.deepcopy(num_neurons)

    def print(self):
        br()
        print("Printing Neural Network")
        br()
        print('Number of hidden layers: ', self.num_Hlayer)
        for i in range(self.num_Hlayer):
            self.Hlayers[i].print('Hidden layer '+str(i)+":")

        self.outputLayer.print('Output layer:')
        br()


    def train(self, data, epoch, rate):
        for layer in self.Hlayers:
            for neuron in layer.neurons:
                neuron.tempweight = copy.deepcopy(neuron.weight)

        for neuron in self.outputLayer.neurons:
            neuron.tempweight = copy.deepcopy(neuron.weight)

        for i in range(epoch):
            for att in data:
                klass = int(att[-1])
                att[-1] = 1
                self.backpropagation(klass, att, rate)

            #br()
            #print('Printing all delta--')
            for layer in self.Hlayers:
                for neuron in layer.neurons:
                    neuron.weight = copy.deepcopy(neuron.tempweight)
            for neuron in self.outputLayer.neurons:
                neuron.weight = copy.deepcopy(neuron.tempweight)

            if (i+1)%(epoch//20) == 0:
                print('Training..', (i+1)*100/epoch, '% done')

    def backpropagation(self, klass, att, rate):
        self.init_targets(klass)
        self.forward_pass(att)
        self.backward_pass()
        self.updateweights(att, rate)
    # update target values--
    def init_targets(self, klass):
        for neuron in self.outputLayer.neurons:
            neuron.target = 0
        self.outputLayer.neurons[klass-1].target = 1

    def forward_pass(self, att):
        self.calculate_output(att)
        total_error = 0
        for neuron in self.outputLayer.neurons:
            error = loss(neuron.out, neuron.target)
            total_error += error

    def calculate_output(self, att):
        features = att
        #print('Printing features')
        #print(features)
        #print('Printing net input')
        for layer in self.Hlayers:
            temp_features = []
            for neuron in layer.neurons:
                net = numpy.dot(features, neuron.weight)

                neuron.out = sigmoid(net)
                #print(net, neuron.out)
                temp_features.append(neuron.out)

            temp_features.append(1)
            features = copy.deepcopy(temp_features)

        for neuron in self.outputLayer.neurons:
            net = numpy.dot(features, neuron.weight)
            neuron.out = sigmoid(net)

    def backward_pass(self):
        #update the deltas
        for neuron in self.outputLayer.neurons:
            neuron.delta = (neuron.out - neuron.target)*neuron.out*(1-neuron.out)

        if self.num_Hlayer >0:

            self.update_Hlayer_delta(self.num_Hlayer-1, self.outputLayer)
            for r_1 in range(self.num_Hlayer-1).__reversed__():
                r = r_1 +1
                self.update_Hlayer_delta(r_1, self.Hlayers[r])



    def update_Hlayer_delta(self, r_1, layer_i):
        for j in range(self.Hlayers[r_1].num_neuron):
            neuron_j = self.Hlayers[r_1].neurons[j]
            sum = 0
            for neuron_i in layer_i.neurons:
                sum += neuron_i.delta * neuron_i.weight[j]

            self.Hlayers[r_1].neurons[j].delta = sum * neuron_j.out * (1 - neuron_j.out)

    def updateweights(self, att, rate):

        features = att
        for layer in self.Hlayers:
            temp_features = []
            for neuron in layer.neurons:
                for j in range(len(features)):
                    neuron.tempweight[j] = neuron.tempweight[j] - rate*neuron.delta*features[j]

                temp_features.append(neuron.out)
            temp_features.append(1)
            features = copy.deepcopy(temp_features)

        for neuron in self.outputLayer.neurons:
            for j in range(len(features)):
                neuron.tempweight[j] = neuron.tempweight[j] - rate * neuron.delta * features[j]

    def test(self, data):
        for att in data:
            #klass = att[self.num_attributes]
            att[-1] = 1
            #self.init_targets(klass)
            self.calculate_output(att)
            output = []
            for neuron in self.outputLayer.neurons:
                output.append(neuron.out)
            print(output)


def main():
    rand.seed(1112)

    input_ = numpy.loadtxt('input.txt', dtype=int)
    #print(input_)
    dataset = numpy.loadtxt('data.txt')
    #print(dataset)
    #exit(0)

    rand.shuffle(dataset)
    #print(data)
    num_attributes = input_[0]
    num_classes = input_[1]
    num_Hlayer = input_[2]
    num_neurons = [0 for i in range(num_Hlayer)]
    for i in range(num_Hlayer):
        num_neurons[i] = input_[i+3]

    print('Number of attributes:', num_attributes)
    print('Number of classes:', num_classes)
    print('Number of hidden layers:', num_Hlayer)
    print('Number of neurons in each hidden layer:', num_neurons)

    network = Network(num_attributes, num_classes, num_Hlayer, num_neurons)
    #network.print()
    network.train(dataset, 300, rate = 0.5)
    #network.print()
    test = [[0, 0, 1], [1.6, 3.5, 1], [12.6, 17.9 ,2], [10.6, 14.2, 2]]
    network.test(test)


if __name__ == '__main__':
    main()
