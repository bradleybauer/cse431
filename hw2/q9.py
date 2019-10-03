from heapq import heappush, heappop
N = int(input())
jobs = [tuple(map(int,input().split()))[::-1] for i in range(N)]
jobs.sort(key=lambda x:x[1])
known_jobs = [jobs[0]]
current_time = jobs[0][1]
time_spent = 0
i = 1
while len(known_jobs):
    duration,time = heappop(known_jobs)
    current_time += duration
    time_spent += current_time - time
    while i < N and current_time >= jobs[i][1]:
        heappush(known_jobs,jobs[i])
        i += 1
    if i < N and len(known_jobs) == 0:
        known_jobs = [jobs[i]]
        current_time = jobs[i][1]
        i += 1
print(time_spent // N)
