# This code is a certificate verifier for the hamiltonian cycle problem

def splitLine():
    return map(int,input().split())

N,M = splitLine()

edges = set()
for i in range(M):
    v0,v1 = splitLine()
    edges |= {(v0,v1),(v1,v0)}

T = int(input())
for i in range(T):
    cycleNodes = list(splitLine())
    cycleEdges = {(cycleNodes[0],cycleNodes[-1])}
    for v0,v1 in zip(cycleNodes,cycleNodes[1:]):
        cycleEdges |= {(v0,v1)}
    isPermutationOfNodes = len(set(cycleNodes))==N and len(cycleNodes)==N
    if isPermutationOfNodes and cycleEdges.issubset(edges):
        print('YES')
    else:
        print('NO')
