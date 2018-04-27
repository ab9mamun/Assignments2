import csv
import math
import random

train_data = []
test_data = []

def br():
    print('=================================================')

def yncounter(dataset):
    y = 0
    n = 0
    for row in dataset:
        if row[1] == 'no':
            n+=1
        elif row[1] == 'yes':
            y+=1
    return y, n


def entropy(train_data, feature_index):
    pass

def next_attribute(train):
    pass

def decision_stump(train):

    pass

def init():
    #declare globals
    global train_data, test_data
    global rowlength
    global header
    #initialize random
    random.seed(1224)

    #read data file
    dataset = []
    br()
    print('Reading data file...')
    with open('bank-additional-full.csv', newline='') as csvfile:
        bankreader =  csv.reader(csvfile, delimiter=';', quotechar='|')
        for row in bankreader:
            dataset.append(row)
    csvfile.close()

    #process dataset
    print('Processing datapoints...')
    n = len(dataset)
    rowlength = len(dataset[0])
    #print(n, 'datapoints found')
    ydata = []
    ndata = []
    for row in dataset:
        for i  in range(rowlength):
            entry = row[i]
            if entry[0] == '"':
                row[i] = entry[1:len(entry)-1]
            else:
                row[i] = float(entry)
        if row[rowlength-1] == 'no':
            ndata.append((row[:rowlength-1], 'no'))
        elif row[rowlength-1] == 'yes':
            ydata.append((row[:rowlength-1], 'yes'))

    #print(len(ndata), len(ydata))
    header = dataset[0]

    #take same numbers of yes and no
    nlength = len(ndata)
    ylength = len(ydata)
    sel_ndata = []

    for i in range(ylength):
        j = random.randint(0, nlength-1)
        sel_ndata.append(ndata[j])
        ndata.pop(j)
        nlength -= 1

    train_data = ydata[:int(ylength*2/3)] + sel_ndata[:int(ylength*2/3)]
    test_data = ydata[int(ylength*2/3):] + sel_ndata[int(ylength * 2 / 3):]

    random.shuffle(train_data)
    random.shuffle(test_data)
    print('Dataset created. Number of datapoints:', len(train_data) + len(test_data))
    print('Train data: ', len(train_data), 'Test data:', len(test_data))
    print('Yes/No counter:', yncounter(train_data), yncounter(test_data))
    br()

def main():
    init()


if __name__ == "__main__":
    main()

