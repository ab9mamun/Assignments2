import csv
import math
import random
import collections
import numpy.random as rand

train_data = []
test_data = []

class Row:
    #x: [] features
    #y: result
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Count:
    #total:
    #yes:
    def __init__(self, total, yes):
        self.total = total
        self.yes = yes

class IgTracker:
    #type:
    #count
    #value
    def __init__(self, _type):
        self.type = _type
        self.value = None
        self.count = None


class DecisionStump:
    #igs
    #best
    #feature_index
    def __init__(self, igs, best, feature_index):
        self.igs = igs
        self.best = best
        self.feature_index = feature_index

def br():
    print('=================================================')

def yncounterAll(dataset):
    y = 0
    n = 0
    for row in dataset:
        if row.y == 'no':
            n+=1
        elif row.y == 'yes':
            y+=1
    return y, n

def yncounter(train, feature_index):
    count = {}
    for row in train:
        entry = row.x[feature_index]
        y = row.y
        if entry in count:
            count[entry].total += 1
        else:
            count[entry] = Count(1, 0)
        if y == 'yes':
            count[entry].yes += 1

    return count

def getIgTrackerNum(train, feature_index):
    it = IgTracker(float)
    count = yncounter(train, feature_index)
    oc = collections.OrderedDict(sorted(count.items()))

    keys = []
    for key in oc.keys():
        keys.append(key)

    best_mid = None
    best_entropy = 9999
    best_count = None

    for i in range(len(keys) -1):
        mid = keys[i] + keys[i+1]
        lt = 0
        ly = 0
        gt = 0
        gy = 0
        for j in range(i+1):
            lt += oc[keys[j]].total
            ly += oc[keys[j]].yes
        for j in range(i+1, len(keys)):
            gt += oc[keys[j]].total
            gy += oc[keys[j]].yes

        #print(lt, ly, gt, gy)
        if ly == 0 or ly == lt:
            l_entropy = 0
        else:
            l_entropy = - ly/lt*math.log2(ly/lt) - (lt-ly)/lt*math.log2((lt-ly)/lt)
        if gy == 0 or gy == gt:
            g_entropy = 0
        else:
            g_entropy = - gy/gt*math.log2(gy/gt) - (gt-gy)/gt*math.log2((gt-gy)/gt)

        entropy = lt/(lt+gt)*l_entropy + gt/(lt+gt)*g_entropy

       #print(lt+gt)

        if entropy < best_entropy:
            best_entropy = entropy
            best_mid = mid
            best_count = {'less': Count(lt, ly), 'greater': Count(gt, gy)}

    it.value = best_mid
    it.count = best_count
    it.entropy = best_entropy
    ans1 = None
    ans2 = None
    if best_count['less'].yes >= best_count['less'].total/2:
        ans1 = 'yes'
    else:
        ans1 = 'no'
    if best_count['greater'].yes >= best_count['greater'].total/2:
        ans2 = 'yes'
    else:
        ans2 = 'no'

    it.ans = {'less': ans1, 'greater': ans2}

    return it


def getIgTrackerStr(train, feature_index):
    it = IgTracker(str)
    count = yncounter(train, feature_index)

    entropy = 0
    ans = {}
    for c in count:
        t = count[c].total
        y = count[c].yes
        if y == 0 or y == t:
            entropy_i = 0
        else:
            entropy_i = -y/t*math.log2(y/t) - (t-y)/t*math.log2((t-y)/t)

        entropy += t/len(train)*entropy_i

        if y >= t/2:
            ans[c] = 'yes'
        else:
            ans[c] = 'no'

    it.entropy = entropy
    it.ans = ans
    return it



def getIgTracker(train, feature_index):
    if type(train[0].x[feature_index]) == str:
        return getIgTrackerStr(train, feature_index)
    else:
        return getIgTrackerNum(train, feature_index)

def decision_stump(train):
    entropy = 22
    ig = None
    igs = []
    feature_index = -1
    for i in range(len(header) - 1):
        it_i = getIgTracker(train, i)
        igs.append(it_i)
        if it_i.entropy < entropy:
            ig = it_i
            entropy = it_i.entropy
            feature_index = i

    return DecisionStump(igs, ig, feature_index)


def resample(dataset, w):
    n = len(w)
    cum_w = [0]*n
    cum_w[0] = w[0]
    for i in range(1,n):
        cum_w[i] = cum_w[i-1]+w[i]
    tempdata = []
    for i in range(n):
        u = rand.uniform(0, 1)
        j = 0
        while j < n:
            if u < cum_w[j]:
                break
            else:
                j+=1
        tempdata.append(dataset[j])

    return tempdata

def check(dc, row):
    if dc.best.value is None:
        value = row.x[dc.feature_index]
        if dc.best.ans[value] == row.y:
            return True
        else:
            return False
    else:
        value = row.x[dc.feature_index]
        if value < dc.best.value:
            if dc.best.ans['less'] == row.y:
                return True
            else:
                return False
        else:
            if dc.best.ans['greater'] == row.y:
                return True
            else:
                return False




    return None

def normalize(w):
    n = len(w)
    sum = 0
    for i in range(n):
        sum+= w[i]
    for i in range(n):
        w[i] = w[i]/sum
    return w

def adaboost(dataset, K): #K is number of boost
    n = len(dataset)
    w = [1/n]*n
    h = [None]*K
    z = [None]*K

    for k in range(K):
        tempdata = resample(dataset, w)
        dc = decision_stump(tempdata)
        error = 0
        for j in range(n):
            if check(dc, dataset[j]) == False:
                error = error + w[j]
        for j in range(n):
            if check(dc, dataset[j]) == True:
                w[j] = w[j]* error/(1-error)

        w = normalize(w)






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
            ndata.append(Row(row[:rowlength-1], 'no'))
        elif row[rowlength-1] == 'yes':
            ydata.append(Row(row[:rowlength-1], 'yes'))

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

    train_data = ydata[:int(ylength*4/5)] + sel_ndata[:int(ylength*4/5)]
    test_data = ydata[int(ylength*4/5):] + sel_ndata[int(ylength * 4 / 5):]

    random.shuffle(train_data)
    # for row in train_data:
    #     if row.x[2] == 'married':
    #         row.y = 'yes'
    #     else:
    #         row.y = 'no'

    random.shuffle(test_data)
    print('Dataset created. Number of datapoints:', len(train_data) + len(test_data))
    print('Train data: ', len(train_data), 'Test data:', len(test_data))
    print('Yes/No counter:', yncounterAll(train_data), yncounterAll(test_data))
    br()

def main():
    init()
    dc = decision_stump(train_data)

if __name__ == "__main__":
    main()

