N = int(input())
cash = {}
for x in input().split():
    x = int(x)
    cash[x] = cash.get(x,0) + 1
cash = sorted(cash.items())
for x,n in cash:
    print(N)
    N-=n
