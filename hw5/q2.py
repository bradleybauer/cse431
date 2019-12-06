N,M = tuple(map(int,input().split()))
adj = {}
for i in range(M):
    x,y = tuple(map(int,input().split()))
    adj[x] = adj.get(x,set()) | set([y])
    adj[y] = adj.get(y,set()) | set([x])

# for x,s in adj.items():
#     print(x,':',s)

m = 0
for i in range(N):
    for j in range(i+1,N):
        if j in adj[i]:
            x = len(adj[i])
            y = len(adj[j]) - len(adj[i] & adj[j]) - 1
            pairs = x*(x+1)//2 + y*(y+1)//2 + x*y
            m = max(m,pairs)
print(m)
