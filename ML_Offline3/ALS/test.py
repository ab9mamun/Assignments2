import pandas as pd
import numpy as np

def get_data(filename):
    df = pd.read_csv(filename)
    df = df.pivot_table(columns=['item'], index=['user'], values='rating')
    df = df.fillna(0)
    return df

train = get_data('train.csv')
validate = get_data('validation.csv')
test = get_data('test.csv')

print(train.shape, validate.shape, test.shape)
#print(matrix)