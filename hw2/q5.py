K,H=2,1212

s=0
c=K
for h in range(1,H+1):
    s += 1
    print(h,':',s)
    if (s == c):
        s = 0
        c *= 2
        print()

# bb you didn't submit this solution
def co(x):
    c = 0
    while x:
        x &= x-1
        c += 1
    return c
def nlpo2(x):
    x |= (x >> 1)
    x |= (x >> 2)
    x |= (x >> 4)
    x |= (x >> 8)
    x |= (x >> 16)
    return co(x>>1)

import math as m
# nc = int(m.log2((H-1)//K + 1))
nc = nlpo2((H-1)//K)
print(H-K*(2**nc-1))
