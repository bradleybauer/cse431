import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

times = {'binary_tree':[],'hash_table':[]}
xticks = []

with open('times.txt') as f:
    for line in f.readlines():
        n,x,y = list(map(float,line.split()))
        xticks.append(int(n))
        times['binary_tree'].append(x)
        times['hash_table'].append(y)

df = pd.DataFrame(times)
df_log = df.apply(np.log,axis=0)

df.plot()
plt.xlabel('Input size')
plt.ylabel('Time (seconds)')
# plt.yticks([])
plt.xticks(np.arange(len(xticks)), xticks, rotation='vertical')
plt.gcf().subplots_adjust(bottom=.2)

df_log.plot()
plt.xlabel('Input size')
plt.ylabel('Time log(Seconds)')
# plt.yticks([])
plt.xticks(np.arange(len(xticks)), xticks, rotation='vertical')
plt.gcf().subplots_adjust(bottom=.2)

plt.show()
