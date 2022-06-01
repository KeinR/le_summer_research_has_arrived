import sys

def looping(graph):
    sgraphs = []
    for i in range(0, int(len(graph) / 2)):
        first = -1
        second = -1
        f = graph[i*2]
        s = graph[i*2 + 1]
        for g in sgraphs:
            if f in g:
                first = g
            if s in g:
                second = g
        if first == -1 and second == -1:
            sgraphs.append([f, s])
        elif first == -1 and second != -1:
            second.append(f)
        elif first != -1 and second == -1:
            first.append(s)
        elif first != second:
            first = first + second
            sgraphs.remove(second)
        else:
            # They're both in the same graph
            return True
    return False



def doCount(hist, d, vertices, stats):
    stats["degrees"][vertices - 2 - d] += 1
    if d == 0:
        return 1
    ret = 0
    for i in range(0, vertices - 1):
        first = i
        second = i + d

        # print("--- testing [%i, %i], diff: %i, first: %i, second: %i, nfs: %i, fns: %i" % (first, second, second < vertices, first in hist, second in hist, not first in hist, not second in hist))
        if second < vertices and not looping(hist + [first, second]):
            # print("New child: [%i, %i], d=%i" % (first, second, d))
            ret += doCount(hist + [first, second], d - 1, vertices, stats)
    return ret




# if len(sys.argv) != 2:
    # print("Invalid args")
    # exit(1)

# vertices = int(sys.argv[1])

# if vertices < 3:
    # print("Invalid vertices count")
    # exit(1)

def run(vertices):

    print("Running %i vertices..." % (vertices))

    stats = {}

    stats["degrees"] = []

    for i in range(0, vertices-1):
        stats["degrees"].append(0)

    print(doCount([0, vertices - 1], vertices - 2, vertices, stats))
    print("Stats:")
    print(stats)

# run(vertices)

for i in range(2, 14):
    run(i)

