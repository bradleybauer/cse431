import random

K = 700
max_skills_per_person = 90
N = 1000
possible_numbers = range(K)
U = set(range(K))
subsets = []
control = set()
for i in range(N-1):
        sub = set(random.sample(U, random.randint(2,max_skills_per_person)))
        subsets += [sub]
        control |= sub

rest = U - control
for i,x  in enumerate(rest):
    subsets[i%len(subsets)] |= {x}

print(len(subsets),K)
for s in subsets:
    if not s:
        continue
    print(len(s))
    for i in s:
        print(i,end=' ')
    print()

