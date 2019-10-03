import heapq
N,K=map(int,input().split())
ls=[-int(input()) for i in range(N)]
heapq.heapify(ls)
for i in range(K):
    print(-heapq.heappop(ls))
