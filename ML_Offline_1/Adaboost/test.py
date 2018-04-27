import collections

d = {2:3, 1:89, 4:5, 3:0}

od = collections.OrderedDict(sorted(d.items()))
d = {}
for i in od:
    d[i] = od[i]
print(od.keys())

