import numpy as np
from collections import Counter
import copy

# Tree, from vertex, current vertex, count map,
# current recorded distance (including this one)
def walk(t, f, r, c, d):
    # print("Checking f=%i,r=%i,d=%i" % (f,r,d))
    if c[r] > 1:
        vertices = set()
        for i in range(0, int(len(t) / 2)):
            m = i * 2
            if t[m] == r and t[m+1] != f:
                vertices.add(t[m+1])
            elif t[m+1] == r and t[m] != f:
                vertices.add(t[m])
        result = []
        for v in vertices:
            result += walk(t, r, v, c, d + 1)
        return result
    elif c[r] == 1:
        return [d]
    else:
        # Impossible!!!
        assert(False)

def treeCodeList(a):
    c = Counter(a)
    results = []
    for k,v in c.items():
        if v == 1:
            i = a.index(k)
            q = -1
            if i % 2 == 0:
                q = a[i+1]
            else:
                q = a[i-1]
            results.append(walk(a, k, q, c, 0))
    return np.array(results).flatten()

def treeCode(a):
    l = treeCodeList(a)
    c = Counter(l)
    # https://oeis.org/A000040
    primes = [2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,
             61,67,71,73,79,83,89,97,101,103,107,109,113,127,
             131,137,139,149,151,157,163,167,173,179,181,191,
             193,197,199,211,223,227,229,233,239,241,251,257,
             263,269,271]
    result = 0
    for k,v in c.items():
        result += primes[k] ** v

    return result 


# Preconditions:
# - Must be a tree - not a forest
# - Input is flattened
def checkIsometric(a, b):
    if len(a) % 2 != 0:
        # Malformed input
        return False
    if len(a) != len(b):
        return False


    ca = treeCode(a)
    cb = treeCode(b)
    # print(ca)
    return ca == cb

def isConnected(g):
    identity = {}
    c = Counter(g)
    for k in c:
        identity[k] = -1
    gi = 0
    for i in range(0, int(len(g) / 2)):
        m = i * 2
        f = g[m]
        s = g[m+1]
        fi = identity[f]
        si = identity[s]
        if fi == -1 and si == -1:
            identity[f] = gi
            identity[s] = gi
            gi += 1
        elif fi != si:
            if si == -1:
                identity[s] = identity[f]
            elif fi == -1:
                identity[f] = identity[s]
            else:
                target = identity[s]
                goal = identity[f]
                for k,v in identity.items():
                    if v == target:
                        identity[k] = goal
    c = -1
    for k,v in identity.items():
        if c == -1:
            c = v
        elif c != v:
            return False
    return True

def isTree(graph):
    return isConnected(graph) and len(set(graph)) - 1 == int(len(graph) / 2)


# print(checkIsometric([0, 1, 1, 2, 2, 3, 1, 7], [4, 3, 3, 6, 6, 1, 3, 9]))

# print(isTree([0, 1, 1, 2, 2, 3, 1, 7]))

def testV(v):
    length = (v - 1) * 2
    series = []
    nonIso = []
    for i in range(length):
        series.append(0)
    running = True
    while running:
        if isTree(series):
            if len(nonIso) == 0:
                print("Appending: " + str(series))
                nonIso.append(copy.deepcopy(series))
            else:
                good = True
                for n in nonIso:
                    if checkIsometric(n, series):
                        good = False
                        break
                if good:
                    print("Appending: " + str(series))
                    nonIso.append(copy.deepcopy(series))
        series[0] += 1
        for i in range(len(series)):
            if series[i] >= v:
                if i+1 < len(series):
                    series[i] = 0
                    series[i+1] += 1
                else:
                    running = False
                    break
            else:
                break
    return nonIso


print(testV(6))

# print(isTree([0, 0, 0, 0, 0, 4]))

# print(isTree([1, 0, 1, 3, 2, 1]))
# print(isTree([2, 3, 0, 2, 2, 1]))

# print(checkIsometric([2, 3, 0, 2, 2, 1], [1, 0, 1, 3, 2, 1]))




