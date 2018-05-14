# Backprop on the Seeds Dataset
from random import seed
from random import randrange
from random import random
from csv import reader
from math import exp
from random import shuffle


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

# Calculate accuracy percentage
def accuracy_metric(actual, predicted):
    correct = 0
    for i in range(len(actual)):
        if actual[i] == predicted[i]:
            correct += 1
    return correct / float(len(actual)) * 100.0



# Calculate neuron activation for an input
def activate(weights, inputs):
    activation = weights[-1]
    for i in range(len(weights) - 1):
        activation += weights[i] * inputs[i]
    return activation


# Transfer neuron activation
def transfer(activation):
    return 1.0 / (1.0 + exp(-activation))


# Forward propagate input to a network output
def forward_propagate(network, row):
    inputs = row
    for layer in network:
        new_inputs = []
        for neuron in layer:
            activation = activate(neuron['weights'], inputs)
            neuron['output'] = transfer(activation)
            new_inputs.append(neuron['output'])
        inputs = new_inputs
    return inputs


# Calculate the derivative of an neuron output
def transfer_derivative(output):
    return output * (1.0 - output)


# Backpropagate error and store in neurons
def backward_propagate_error(network, expected):
    for i in reversed(range(len(network))):
        layer = network[i]
        errors = list()

        #for hidden layers
        if i != len(network) - 1:
            for j in range(len(layer)):
                error = 0.0
                for neuron in network[i + 1]:
                    error += (neuron['weights'][j] * neuron['delta'])
                errors.append(error)
        else:
            for j in range(len(layer)):
                neuron = layer[j]
                errors.append(expected[j] - neuron['output'])
        for j in range(len(layer)):
            neuron = layer[j]
            neuron['delta'] = errors[j] * transfer_derivative(neuron['output'])


# Update network weights with error
def update_weights(network, row, l_rate):
    for i in range(len(network)):
        inputs = row[:-1]
        if i != 0:
            inputs = [neuron['output'] for neuron in network[i - 1]]
        for neuron in network[i]:
            for j in range(len(inputs)):
                neuron['weights'][j] += l_rate * neuron['delta'] * inputs[j]
            neuron['weights'][-1] += l_rate * neuron['delta']


# Train a network for a fixed number of epochs
def train_network(network, train, l_rate, n_epoch, n_outputs):
    print('Training Network')
    for epoch in range(n_epoch):
        for row in train:
            outputs = forward_propagate(network, row)
            expected = [0 for i in range(n_outputs)]
            expected[row[-1]] = 1
            backward_propagate_error(network, expected)
            update_weights(network, row, l_rate)

        if (epoch+1)%(n_epoch//10) == 0:
            print('Training...', (epoch+1)*100/n_epoch, '% complete')


# Initialize a network
def initialize_network(n_inputs, n_hidden_layers, n_hidden, n_outputs):
    print('Building Network...')
    network = list()
    size = n_inputs
    for r in range(n_hidden_layers):
        hidden_layer = [{'weights': [random() for i in range(size + 1)]} for i in range(n_hidden[r])]
        network.append(hidden_layer)
        size = n_hidden[r]

    output_layer = [{'weights': [random() for i in range(n_hidden[-1] + 1)]} for i in range(n_outputs)]
    network.append(output_layer)
    return network


# Make a prediction with a network
def predict(network, row):
    outputs = forward_propagate(network, row)
    return outputs.index(max(outputs))  #returns the class with highest output value


# Backpropagation Algorithm With Stochastic Gradient Descent
def back_propagation(train, l_rate, n_epoch, n_hidden_layers, n_hidden):
    n_inputs = len(train[0]) - 1
    n_outputs = len(set([row[-1] for row in train]))
    network = initialize_network(n_inputs, n_hidden_layers, n_hidden, n_outputs)
    train_network(network, train, l_rate, n_epoch, n_outputs)
    return network

def test_network(network, test):
    predictions = list()
    for row in test:
        prediction = predict(network, row)
        predictions.append(prediction)
    return (predictions)

def evaluate(network, fold):
    print('Testing...')
    test_set = list()
    for row in fold:
        row_copy = list(row)
        test_set.append(row_copy)
        row_copy[-1] = None

    predicted = test_network(network, test_set)
    actual = [row[-1] for row in fold]
    accuracy = accuracy_metric(actual, predicted)
    return accuracy

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

def main():
    # Test Backprop on Seeds dataset
    seed(11)
    # load and prepare data
    filename = 'data.csv'
    dataset = load_csv(filename)
    for i in range(len(dataset[0]) - 1):
        str_column_to_float(dataset, i)
    # convert class column to integers
    str_column_to_int(dataset, len(dataset[0]) - 1)


    # initialize the hyperparameters
    l_rate = 0.3
    n_epoch = 500
    n_hidden_layers = 2
    n_hidden = [4, 3]
    shuffle(dataset)
    size = len(dataset)

    print('Epoch:', n_epoch, 'Learning rate:', l_rate)
    print('Number of hidden layers:', n_hidden_layers)
    print('Number of Neurons in each hidden layer:', n_hidden)


    network =  back_propagation(dataset[:size*4//5], l_rate, n_epoch, n_hidden_layers, n_hidden)
    #print_network(network)
    score = evaluate(network, dataset[size*4//5:])
    print('Accuracy: %.3f%%' % (score))

if __name__ == '__main__':
    main()