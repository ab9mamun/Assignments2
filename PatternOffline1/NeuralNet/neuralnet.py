# Backprop on the Seeds Dataset
from random import seed
from random import random
from csv import reader
from math import exp
from random import shuffle
from numpy import dot
from numpy import loadtxt

# Load a CSV file
def load_csv(filename):
    dataset = list()
    with open(filename, 'r') as file:
        csv_reader = reader(file)
        for row in csv_reader:
            if not row:
                continue
            dataset.append(row)
    return dataset

# Convert string column to float
def str_column_to_float(dataset, column):
    for row in dataset:
        row[column] = float(row[column].strip())


# Convert string column to integer
def str_column_to_int(dataset, column):
    class_values = [row[column] for row in dataset]
    unique = set(class_values)
    lookup = dict()
    for i, value in enumerate(unique):
        lookup[value] = i
    for row in dataset:
        row[column] = lookup[row[column]]
    return lookup

def net_input(weights, features):
    temp = features.copy()
    temp.append(1)
    return dot(weights, temp)


def sigmoid(net):
    return 1.0 / (1.0 + exp(-net))

def forward_pass(network, row):
    #print(len(row))
    features = row
    for layer in network:
        next_features = []
        for neuron in layer:
            net = net_input(neuron['weights'], features)
            neuron['output'] = sigmoid(net)
            next_features.append(neuron['output'])
        features = next_features.copy()
    return features

# Backpropagate error and store in neurons
def backward_pass(network, target):
    for r_1 in reversed(range(len(network))):
        layer = network[r_1]
        weighted_delta_sums = list()

        #for hidden layers
        if r_1 != len(network) - 1:
            for j in range(len(layer)):
                weighted_delta_sum = 0.0
                for neuron in network[r_1 + 1]:
                    weighted_delta_sum += (neuron['weights'][j] * neuron['delta'])
                weighted_delta_sums.append(weighted_delta_sum)
        #for output layer
        else:
            for j in range(len(layer)):
                neuron = layer[j]
                weighted_delta_sums.append(neuron['output'] - target[j])
        for j in range(len(layer)):
            neuron = layer[j]
            output = neuron['output']
            neuron['delta'] = weighted_delta_sums[j] * output * (1.0 - output)

# Update network weights with error
def update_weights(network, row, l_rate):
    for r in range(len(network)):
        features = row[:-1]
        if r != 0:
            features = [neuron['output'] for neuron in network[r - 1]]
        for neuron in network[r]:
            for j in range(len(features)):
                neuron['weights'][j] -= l_rate * neuron['delta'] * features[j] #update the weights for features
            neuron['weights'][-1] -= l_rate * neuron['delta']   #update the weight for bias


# Train a network for a fixed number of epochs
def train_network(network, train, l_rate, n_epoch, n_outputs):
    print('Training Network')
    for epoch in range(n_epoch):
        for row in train:
            outputs = forward_pass(network, row[:-1])  #returns the ouputs of the output neurons

            target = [0 for i in range(n_outputs)] #update the target values
            target[row[-1]] = 1
            backward_pass(network, target)
            update_weights(network, row, l_rate)

        if (epoch+1)%(n_epoch//10) == 0:
            print('Training...', (epoch+1)*100/n_epoch, '% complete')


# Initialize a network
def init_network(n_inputs, n_hidden_layers, n_hidden, n_outputs):
    print('Building Network...')
    network = list()
    size = n_inputs
    for r in range(n_hidden_layers):
        hidden_layer = [{'weights': [random() for i in range(size + 1)]} for i in range(n_hidden[r])]
        network.append(hidden_layer)
        size = n_hidden[r]

    output_layer = [{'weights': [random() for i in range(size + 1)]} for i in range(n_outputs)]
    network.append(output_layer)
    return network


# Make a prediction with a network
def predict(network, row):
    outputs = forward_pass(network, row[:-1])
    return outputs.index(max(outputs))  #returns the class with highest output value


# Backpropagation Algorithm With Stochastic Gradient Descent
def back_propagation(train, l_rate, n_epoch, n_hidden_layers, n_hidden):
    n_inputs = len(train[0]) - 1  #number of features in a datapoint--
    n_outputs = len(set([row[-1] for row in train])) #number of different classes
    network = init_network(n_inputs, n_hidden_layers, n_hidden, n_outputs)  #build network structure
    train_network(network, train, l_rate, n_epoch, n_outputs)   #update weights of the neurons
    return network

def test_network(network, test):
    predictions = list()
    for row in test:
        prediction = predict(network, row)  #predict the class for each row
        predictions.append(prediction)
    return (predictions)

def evaluate(network, testset):
    print('Testing...')
    testset_locked = list()
    for row in testset:
        row_copy = list(row)
        testset_locked.append(row_copy)
        row_copy[-1] = None

    predicted = test_network(network, testset_locked)
    actual = [row[-1] for row in testset]

    correct = 0
    for i in range(len(actual)):
        if actual[i] == predicted[i]:
            correct += 1
    return len(actual), correct, correct*100 /float(len(actual))

def br():
    print('----------------------------------------------')

def print_network(network):
    br()
    print('Printing Network')
    br()
    for i in range(len(network)):
        if i == len(network) -1:
            print('Output layer:')
        else:
            print('Hidden layer:', i)
        br()
        j = 0
        for neuron in network[i]:
            print('Neuron', j, neuron['weights'])
            j+=1

        br()

def load(filename):

    tempdata = loadtxt(filename)
    dataset = list()
    for row in tempdata:
        temprow = list()
        for j in range(len(row) - 1):
            temprow.append(float(row[j]))
        temprow.append(int(row[-1]))
        dataset.append(temprow.copy())

    return dataset

def main():
    # Test Backprop on Seeds dataset
    seed(11)
    # load and prepare data
    train_set = load('data.txt')
    test_set = load('input.txt')

    # initialize the hyperparameters
    l_rate = 0.3
    n_epoch = 300
    n_hidden_layers = 2
    n_hidden = [3, 4]
    shuffle(train_set)
    size = len(train_set)

    #shift the classes to start from 0
    minimum = 89999
    for row in train_set:
        minimum = min(minimum, row[-1])
    for row in train_set:
        row[-1] -= minimum

    minimum = 89999
    for row in test_set:
        minimum = min(minimum, row[-1])
    for row in test_set:
        row[-1] -= minimum


    print('Epoch:', n_epoch, 'Learning rate:', l_rate)
    print('Number of hidden layers:', n_hidden_layers)
    print('Number of Neurons in each hidden layer:', n_hidden)

    network =  back_propagation(train_set, l_rate, n_epoch, n_hidden_layers, n_hidden)
    #print_network(network)
    total, correct, score = evaluate(network, test_set)
    print('Total:', total, 'Correct:', correct)
    print('Accuracy: %.3f%%' % (score))

if __name__ == '__main__':
    main()