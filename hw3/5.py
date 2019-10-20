def ans(memo,Bs,K):
    if K in memo:
        return memo[K]
    if K == 0 or len(Bs) == 0:
        return 0
    best = 0
    for i in range(K//Bs[-1]+1):
        best = max(best, i*Bs[-1] + ans(memo, Bs[:-1], K-i*Bs[-1]))
    memo[K] = best
    return best

T = int(input())
for t in range(T):
    N,K = map(int, input().split())
    Bs = set(map(int, input().split()))
    memo = {}
    print(ans(memo,list(Bs),K))

