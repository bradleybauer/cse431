N = int(input())
s=0
x=0
for i in range(N):
    r,c = map(int,input().split())
    s += r-c
    if s < 0:
        x = i + 1
        s = 0
print(x % N)
