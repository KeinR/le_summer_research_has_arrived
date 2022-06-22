import json
import copy
import sys

name = sys.argv[1]
graphStr = sys.argv[2]

if len(name) <= 0:
    print("Must provide name")
    exit(1)

if len(graphStr) <= 0:
    print("Must provide graph")
    exit(1)

file = open('%s.json' % (name), 'r')
output = open('reports/%s.csv' % (name), 'w')
outputSummary = open('reports/%s.txt' % (name), 'w')
data = json.load(file)
counterSwp = {}
counterSwpLabel = {}
counterRes = {}

graph = json.loads(graphStr)
adjList = {}

for p in graph:
    if not p[0] in adjList:
        adjList[p[0]] = set()
    if not p[1] in adjList:
        adjList[p[1]] = set()
    adjList[p[0]].add(p[1])
    adjList[p[1]].add(p[0])

output.write("swapped labels,edges lost,edges gained,result,label mapping\n")
passed = 0

for labelling in data:
    for a in range(len(labelling)):
        for b in range(a+1, len(labelling)):
            lostA = []
            gainedA = []
            av = labelling[a]
            bv = labelling[b]
            for e in adjList[a]:
                if e != b:
                    lostA.append(abs(av - labelling[e]))
                    gainedA.append(abs(bv - labelling[e]))
            for e in adjList[b]:
                if e != a:
                    lostA.append(abs(bv - labelling[e]))
                    gainedA.append(abs(av - labelling[e]))
            lostA.sort()
            gainedA.sort()
            lost = set(lostA)
            gained = set(gainedA)
            
            p = "FAIL"

            #if swp in data:
            if lost == gained:
                swp = copy.deepcopy(labelling)
                tmp = swp[a]
                swp[a] = swp[b]
                swp[b] = tmp

                p = "PASS"
                passed += 1
                l = 0
                h = 0
                if a > b:
                    h = a
                    l = b
                else:
                    h = b
                    l = a
                f = "(%i,%i)" % (l,h)
                if not f in counterSwp:
                    counterSwp[f] = 0
                counterSwp[f] += 1
                l = 0
                h = 0
                if av > bv:
                    h = av
                    l = bv
                else:
                    h = bv
                    l = av
                f = "(%i,%i)" % (l,h)
                if not f in counterSwp:
                    counterSwpLabel[f] = 0
                counterSwpLabel[f] += 1

                ss = str(swp)
                if not ss in counterRes:
                    counterRes[ss] = 0
                counterRes[ss] += 1
            
            ls = ""
            gs = ""
            for g in gainedA:
                gs += "%i," % (g)
            for l in lostA:
                ls += "%i," % (l)
            ls = ls[:len(ls)-1]
            gs = gs[:len(gs)-1]

            output.write('"%i,%i","%s","%s","%s","%s"\n' % (av,bv, ls, gs, p, str(labelling)))


outputSummary.write("----------------------------\n")
outputSummary.write("Name: %s\n" % (name))
outputSummary.write("----------------------------\n")
outputSummary.write("Graph: %s\n" % (graph))
outputSummary.write("----------------------------\n")
outputSummary.write("Passed (swapped): %i\n" % (passed))
outputSummary.write("----------------------------\n")
outputSummary.write("Swapped pair frequencies (indices): %s\n" % (str(counterSwp)))
outputSummary.write("----------------------------\n")
outputSummary.write("Swapped pair frequencies (labels): %s\n" % (str(counterSwpLabel)))
outputSummary.write("----------------------------\n")
outputSummary.write("Resultant tree instances: %s\n" % (str(counterRes)))
outputSummary.write("----------------------------\n")

output.close()
outputSummary.close()
file.close()


