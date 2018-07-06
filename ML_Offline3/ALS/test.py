import pandas as pd


l = [(1,2), (4, 44), (6, 22)]
print(l)
l.sort(key=lambda x:x[1], reverse=True)

print(l)