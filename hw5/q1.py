N,M = tuple(map(int,input().split()))
deg = {}
mdeg = 0
for i in range(M):
    x,y = tuple(map(int,input().split()))
    deg[x] = deg.get(x,0) + 1
    deg[y] = deg.get(y,0) + 1
    mdeg = max(mdeg,deg[x],deg[y])
print(mdeg*(mdeg+1)//2)
