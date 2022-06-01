


def leSeries(data, offset, num):
    result = []
    idx = 0
    for i in range(0, num):
        # print("cat me")
        swap = []
        for i in range(0, len(data)):
            swap.append(-1)
        gotten = False
        while not gotten:
            idx += 1
            gotten = True
            for i in range(0, len(data)):
                f = data[i] - factorial(i + offset) / idx
                if f <= 0:
                    gotten = False
                    break
                else:
                    swap[i] = f
        result.append(idx)
        data = swap
        # print(data)
    print("......")
    for d in data:
        print(float(d))
    return result



