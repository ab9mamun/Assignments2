import random
print('Hello world')
def randf(low, high):
    return low + random.random()*(high - low)
while True:
    print(randf(-12,56))