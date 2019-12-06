N,M = list(map(int,input().split()))
parents = {i:i for i in range(N)}
def find(x):
    if parents[x] != x:
        return find(parents[x])
    return x
def union(x,y):
    px = find(x)
    py = find(y)
    parents[px] = py
for i in range(M):
    x,y = list(map(int,input().split()))
    union(x,y)
count = 0
for i in range(N): # there is one component for each representative
    if find(i) == i:
        count += 1
print(count)
