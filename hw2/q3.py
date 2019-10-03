counts = {}
N = int(input())
max_count = 0
max_lab = 0
for i in range(N):
    lab = int(input())
    counts[lab] = counts.get(lab,0) + 1
    if counts[lab] >= max_count:
        if max_count == counts[lab]:
            max_lab = min(lab,max_lab)
        else:
            max_lab = lab
        max_count = counts[lab]
print(max_lab)
