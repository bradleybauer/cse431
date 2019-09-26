import numpy as np

# looks like O(n^2)
def monolithRotation(matrix, m, n, r):
    nrings = min(m,n)//2
    A=np.array(matrix)
    if m>n:
        A=A.T # A.T is a view, O(1)
        r*=-1
    rings=[np.array([A[0,:], # top
                     A[1:-1,-1], # right
                     A[-1,:][::-1], # bottom
                     A[1:-1,0][::-1]])] # slicing makes a view, O(1)
    for i in range(1,nrings): # O(n)
        rings+=[[A[i,i:-i],A[i+1:-i-1,-i-1],A[-i-1,i:-i][::-1],A[i+1:-i-1,i][::-1]]] # O(1)
    rolls=[np.roll(np.concatenate(c),-r) for c in rings] # concatenate copies, O(n^2)
    for ring,roll in zip(rings,rolls): # O(n)
        for part in ring: # O(1)
            np.copyto(part,roll[:len(part)]) # O(n)
            roll=roll[len(part):] # update view O(1)
    if m>n:
        A=A.T
    for r in A:
        for c in r:
            print(c,end=' ')
        print()

if __name__ == '__main__':
    mnr = input().rstrip().split()

    m = int(mnr[0]) # Number of rows

    n = int(mnr[1]) # Number of columns

    r = int(mnr[2]) # Number of rotations

    matrix = []

    for _ in range(m):
        matrix.append(list(map(int, input().rstrip().split())))

    monolithRotation(matrix, m, n, r)
