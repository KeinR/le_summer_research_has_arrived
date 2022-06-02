import copy
import math
import sys
import json

def places(i):
    if (i == 0):
        return 1
    r = 0
    while i >= 1:
        r += 1
        i /= 10
    return r

# Takes flattened arrays
def mkGrid(graph):
    a = list(set(copy.deepcopy(graph)))
    a.sort()
    width = 0
    widthMap = {}
    mostWidth = 0
    div = ""
    for v in a:
        w = places(v)
        width += w
        widthMap[v] = w
        if w > mostWidth:
            mostWidth = w
        div += "-" * (w + 2) + "+"
    div = "+" + "-" * (mostWidth + 2) + "+" + div
    body = ""
    body += div + "\n| " + " " * mostWidth + " |"
    for v in a:
        body += " %i |" % (v)
    body += "\n" + div + "\n"

    depDict = {}
    for v in a:
        depDict[v] = set()
    for i in range(0, int(len(graph) / 2)):
        m = i * 2
        f = graph[m]
        s = graph[m+1]
        if f > s:
            depDict[s].add(f)
        else:
            depDict[f].add(s)

    fmt = "| %%%ii |" % (mostWidth)
    for q in a:
        body += fmt % (q)
        for v in a:
            c = " "
            if v in depDict[q]:
                c = "X"
            elif v <= q:
                c = "#"
            body += " " + c*(widthMap[v]) + " |"
        body += "\n" + div + "\n"

    return body



if len(sys.argv) != 2:
    print("Invalid num arguments")
    exit(1)

graph = json.loads(sys.argv[1])

grid = mkGrid(graph)

print(grid)

# ]mkGrid([0, 1, 1, 2, 2, 30, 30, 4])


