import numpy as np
from bitstring import BitArray
from scipy import stats as sp


##
# Converts a BitArray to int
##
def bit2int(d):
    i = 0
    for bit in d:
        i = (i << 1) | bit
    return i

##
# Function for transmitting an input array
##
def transmit(input, w, sigma_square, n):
    padded_input = BitArray(n)

    # Padding bits at the beginning #
    padded_input.append(input)
    np.random.seed(1)

    # Getting random noise #
    ita = np.random.normal(0, np.sqrt(sigma_square), input.len)
    ret_x = []

    # The function f(I1,I2,...Ik) #
    for i in range(0, input.len):
        sum = 0
        for j in range(0, n):
            sum = sum + padded_input[i + j] * w[j]

        # Adding noise #
        ret_x.append(sum + ita[i])
    return ret_x


# Constant for eliminating 0 probility in case of underfitting
probability_calculation_constant = 1.0

print("# Reading config file.")
f = open("config.txt", 'r')

line = f.readline()
parts = line.split()

n = int(parts[0])
l = int(parts[1])

line = f.readline()
parts = line.split()
w = list(map(float, parts))

sigma_square = float(f.readline())
#sigma_square = 0.0
f.close()

print('n => {}'.format(n))
print('l => {}'.format(l))
print('w => {}'.format(w))
print('variance => {}'.format(sigma_square))


prior_probability = np.full((2**n), 1.0/2**n)
transition_probability = np.full((2**n, 2**n), 1.0/2**(n+1))

print("\n# Reading Train bits.")
f = open("train.txt", 'r')
line = f.readline()
f.close()
train = BitArray(bin=line)
train_states = train.len-2
prev_state = -1

##
# Calculating prior and transition probabilities
##
print(train_states)
pp = 0
for i in range(train_states):
        if(i!= 0 and train_states/i != pp):
            pp = train_states/i
            print(pp,end="\r")

        state = bit2int(train[i:i+n])
        prior_probability[state] = prior_probability[state] + 1.0/(train_states + probability_calculation_constant)
        if prev_state != -1: # If this is not the first state
            transition_probability[prev_state][state] = transition_probability[prev_state][state] + 1.0/(train_states - 1 + probability_calculation_constant)
        prev_state = state


print("\n# Prior probabilities :")
for j in prior_probability:
    print("%10.9f  " % j, end=" ")
print("\n")
print("\n# Transition probabilities :")
for i in range(2**n):
    for j in transition_probability[i]:
        print("%10.9f  "%j, end=" ")
    print()


print('\n# Train length {}'.format(train.len))

##
# Transmit train bits
##
train_x = transmit(train, w, sigma_square, n)

print("\n# Observations : ")
for i in range(0, train.len):
    print("%10.9f  " % train_x[i], end=" ")
print()

##
# Observation probability computation
##
observation_probability_data = []
observation_probability_distribution_mean = []
for i in range(0, 2**n):
    observation_probability_data.append([])
    observation_probability_distribution_mean.append(0.0)


padded_train = BitArray(n)
padded_train.append(train)

for i in range(0, train.len):
    (observation_probability_data[bit2int(padded_train[i:i+n])]).append(train_x[i])

for i in range(0, 2**n):
    mean = np.mean(observation_probability_data[i])
    observation_probability_distribution_mean[i] = mean

print("\n# Means of the observation probabilities : ")
for i in range(0, 2**n):
    print("%10.9f  " % observation_probability_distribution_mean[i], end=" ")
print("\n")


##
# read test
##
f = open("test.txt", 'r')
line = f.readline()
f.close()
test = BitArray(bin=line)
test_x = transmit(test, w, sigma_square, n)

viterby_array = np.full((2**n, test.len), 0.0)
parent_array = np.full((2**n, test.len), -1)
sigma = np.sqrt(sigma_square)
print("\n Starting Viterby\n")

##
# The Viterby Algorithm
##
k = 0
for i in range(0, 2**n):
    viterby_array[i][k] = np.log(prior_probability[i]) + np.log(sp.norm.pdf(test_x[k], observation_probability_distribution_mean[i], sigma))

for k in range(1, test.len):
    for i in range(0, 2 ** n):
        max_probable_i_prev = -1
        max_probability = -9999
        for i_prev in range(0, 2**n):
            probability = viterby_array[i_prev][k-1]+np.log(transition_probability[i_prev][i])
            if max_probability < probability:
                max_probability = probability
                max_probable_i_prev = i_prev
        viterby_array[i][k] = max_probability + np.log(sp.norm.pdf(test_x[k], observation_probability_distribution_mean[i], sigma))
        parent_array[i][k] = max_probable_i_prev


print("\n\nViterby array : ")
for i in range(0, 2**n):
    for k in range(0, test.len):
        print("%10.9f  " % viterby_array[i][k], end=" ")
    print()

##
#Time for back tracing
##
k = test.len-1
max = -99999
max_i = -1
for i in range(0, 2**n):
    if max < viterby_array[i][k]:
        max = viterby_array[i][k]
        max_i = i

trace = [max_i]
k = test.len-1
while parent_array[max_i][k] != -1:
    trace.append(parent_array[max_i][k])
    max_i = parent_array[max_i][k]
    k = k-1


final = []
sum = 0
for i in range(len(trace)-1, -1, -1):
    final.append(trace[i] & 1)
    sum = sum + ((trace[i] & 1) == test[i])

print("The final output : ", end="")
for i in range(0, test.len):
    print(final[i], end="")
print("\nThe given input  : ", end="")
print(test.bin)
print("\nAccuracy : ")
print('{} %'.format(sum*100.0/test.len))

    #0, 1, 1, 0, 1, 0, 0, 0, 1, 1  [0, 1, 1, 1, 0, 1, 0, 0, 1, 0
