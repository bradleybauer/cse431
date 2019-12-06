import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

times = {'array':[],'binary_tree':[]}

with open('times.txt') as f:
    for line in f.readlines():
        x,y = list(map(float,line.split()))
        times['array'].append(x)
        times['binary_tree'].append(y)

df = pd.DataFrame(times)

df.plot()
plt.xlabel('Input size')
plt.ylabel('Time (seconds)')
plt.gcf().subplots_adjust(left=.2)
df.ewm(alpha=.2).mean().iloc[::-1].ewm(alpha=.2).mean().iloc[::-1].plot()
plt.xlabel('Input size')
plt.ylabel('Time (seconds)')
plt.gcf().subplots_adjust(left=.2)
plt.show()

df = df.iloc[3000:5000]
df.plot()
plt.gcf().subplots_adjust(left=.2)
plt.xlabel('Input size')
plt.ylabel('Time (seconds)')
df.ewm(alpha=.1).mean().iloc[::-1].ewm(alpha=.1).mean().iloc[::-1].plot()
plt.xlabel('Input size')
plt.ylabel('Time (seconds)')
plt.gcf().subplots_adjust(left=.2)
plt.show()
