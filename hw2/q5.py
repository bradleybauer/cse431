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

import math as m
nc = int(m.log2((H-1)//K + 1))
print(H-K*(2**nc-1))
