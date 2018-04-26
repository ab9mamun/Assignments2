import csv
def init():
    with open('bank-additional-full.csv', newline='') as csvfile:
        bankreader =  csv.reader(csvfile, delimiter=' ', quotechar='|')
        i = 0
        for row in bankreader:
            if(i==3500):
                print(', '.join(row))
            i+=1


def main():
    print('Starting adaboost program')
    init()


if __name__ == "__main__":
    main()

