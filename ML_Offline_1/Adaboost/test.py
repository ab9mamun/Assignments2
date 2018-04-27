

for i in range(5):
    temp = x[:i*100//5] + x[(i+1)*100//5:]
    val = x[i*100//5:(i+1)*100//5]
    print(temp)
    print(val)
