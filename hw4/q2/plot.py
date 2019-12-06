import matplotlib.pyplot as plt
import pandas as pd

hybridsort_times = {}
quicksort_times = {}
insertionsort_times = {}

with open('times.txt') as f:
    for line in f.readlines():
        k,x,y,z = line.split()
        k,x,y,z = int(k),float(x),float(y),float(z)
        if k not in hybridsort_times:
            hybridsort_times[k] = []
            quicksort_times[k] = []
            insertionsort_times[k] = []
        hybridsort_times[k].append(x)
        quicksort_times[k].append(y)
        insertionsort_times[k].append(z)

df = pd.DataFrame(hybridsort_times)

sf = df.mean(axis=0)
sf.plot()
plt.xlabel('k')
plt.ylabel('Time (seconds)')
plt.gcf().subplots_adjust(left=.2)
plt.show()


wf = sf.iloc[20:100]
wf.plot(color="#e9e9e9")
wf = wf.ewm(alpha=.2).mean() # shifts to the right a bit
wf = wf.iloc[::-1].ewm(alpha=.2).mean().iloc[::-1] # try to undo that shift
wf.plot(color="#ff0000")

# plt.title('k v.s. average time')
plt.xlabel('k')
plt.ylabel('Time (seconds)')
plt.gcf().subplots_adjust(left=.2)
plt.show()

mink = wf.idxmin()
print('min k value =',mink)

hbf = df.iloc[:,mink]
qkf = pd.DataFrame(quicksort_times).iloc[:,mink]
inf = pd.DataFrame(insertionsort_times).iloc[:,mink]

hbf.plot() # blue
qkf.plot() # orange
inf.plot() # green
plt.xlabel('Input size')
plt.ylabel('Time (seconds)')
plt.gcf().subplots_adjust(left=.2)
plt.legend(['hybridsort', 'quicksort','insertionsort'])
plt.show()

