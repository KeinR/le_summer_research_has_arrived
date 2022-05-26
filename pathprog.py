import sys

def doCount(hist, d, vertices, stats):
    stats["degrees"][vertices - 1 - d] += 1
    if d == 0:
        return 1
    ret = 0
    for i in range(0, vertices - 1):
        first = i
        second = i + d

        # print("--- testing [%i, %i], diff: %i, first: %i, second: %i, nfs: %i, fns: %i" % (first, second, second < vertices, first in hist, second in hist, not first in hist, not second in hist))
        if second < vertices and ((not first in hist and second in hist) or (first in hist and not second in hist)):
            # print("New child: [%i, %i], d=%i" % (first, second, d))
            ret += doCount(hist + [first, second], d - 1, vertices, stats)
    return ret




if len(sys.argv) != 2:
    print("Invalid args")

vertices = int(sys.argv[1])

if vertices < 3:
    print("Invalid vertices count")

def run(vertices):

    print("Running %i vertices..." % (vertices))

    stats = {}

    stats["degrees"] = []

    for i in range(0, vertices):
        stats["degrees"].append(0)

    print(doCount([0, vertices - 1], vertices - 2, vertices, stats))
    print("Stats:")
    print(stats)


for i in range(2, 20):
    run(i)

