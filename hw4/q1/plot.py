import matplotlib.pyplot as plt
import pandas as pd

quicksort_times = []
insertionsort_times = []

with open('times.txt') as f:
    for line in f.readlines():
        x,y = list(map(float,line.split()))
        insertionsort_times.append(x)
        quicksort_times.append(y)

plt.plot(quicksort_times, label='quicksort')
plt.plot(insertionsort_times, label='insertionsort')
plt.gca().legend()
plt.ylabel('Time (seconds)')
plt.xlabel('Input Size')
plt.gcf().subplots_adjust(left=.2)
plt.show()

qf = pd.DataFrame(quicksort_times[:200])
qf = qf.ewm(alpha=.2).mean().iloc[::-1].ewm(alpha=.2).mean().iloc[::-1]
inf = pd.DataFrame(insertionsort_times[:200])
inf = inf.ewm(alpha=.2).mean().iloc[::-1].ewm(alpha=.2).mean().iloc[::-1]
conc = pd.concat([qf,inf],axis=1)
conc['diff'] = qf-inf
conc.plot()

plt.gca().legend()
plt.ylabel('Time')
plt.xlabel('Input Size')
plt.gcf().subplots_adjust(left=.2)
plt.legend(['quicksort','insertionsort','perf diff'])
plt.show()

print('perf diff max = ',conc['diff'].idxmax())
