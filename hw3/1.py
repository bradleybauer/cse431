N,K = map(int,input().split())
energies = sorted(map(int, input().split()))[::-1]
s = 0
for i in range(N):
    s += energies[i] * (i//K + 1)
print(s)
