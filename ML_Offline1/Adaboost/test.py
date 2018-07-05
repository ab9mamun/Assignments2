import random
x = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
w = [1, 2, 3, 4, 4, 5, 3, 1, 7, 2]
z = []
for i in range(10):
    z+= random.choices(x, w)
print(z)