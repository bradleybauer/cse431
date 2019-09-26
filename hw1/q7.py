import math as m
for i in range(int(input())):
    x,y=map(int,input().split())
    d = y//12-(x-1)//12
    crtx = m.ceil(m.sqrt(x))
    frty = m.floor(m.sqrt(y))
    s = max(0,frty - (crtx - 1))
    b = frty // 6 - (crtx-1) // 6
    print(d, s, b)
