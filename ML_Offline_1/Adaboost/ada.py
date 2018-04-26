import csv
import math
import random

def br():
    print('=================================================')

def decision_stump():
    pass

def init():
    #declare globals
    global dataset
    global rowlength
    global datasize
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
    dataset = ydata[:]

    #take same numbers of yes and no
    nlength = len(ndata)
    ylength = len(ydata)

    for i in range(ylength):
        j = random.randint(0, nlength-1)
        dataset.append(ndata[j])
        ndata.pop(j)
        nlength -= 1

    datasize = len(dataset)
    random.shuffle(dataset)
    print('Dataset created. Number of datapoints:', datasize)
    br()

def main():
    init()


if __name__ == "__main__":
    main()

