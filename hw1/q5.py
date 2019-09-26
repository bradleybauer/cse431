for i in range(int(input())):
    x,y,n = map(int,input().split())
    print(*sorted(set((n-j)*x+j*y for j in range(n+1))))
