# This code reduces a given minimum vertex cover instance to a maximum clique instance

def splitLine():
    return tuple(map(int,input().split()))

N,M,K = splitLine()

adj = {}
for i in range(M):
    v0,v1 = splitLine()
    adj[v0] = adj.get(v0,set()) | {v1}
    adj[v1] = adj.get(v1,set()) | {v0}

universe = set(range(N))
for v in range(N):
    adj[v] = universe - adj[v] - {v}

M = N*(N-1)//2 - M
K = N - K

print(N, M, K)
for v0 in sorted(adj):
    for v1 in sorted(adj[v0]):
        if v1 > v0:
            print(v0,v1)
