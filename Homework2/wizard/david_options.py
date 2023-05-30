xs = [0,0,0,0,0,0,0,0,0,0,0,0]
options = []
for xs[6] in range(1,13):
    for xs[8] in range(1,13):
        if xs[8] == xs[6]:
            continue
        for xs[9] in range(1,13):
            if xs[9] == xs[8] or xs[9] == xs[6]:
                continue
            for xs[10] in range(1,13):
                if xs[10] == xs[9] or xs[10] == xs[8] or xs[10] == xs[6]:
                    continue
                for xs[11] in range(1,13):
                    if xs[11] == xs[10] or xs[11] == xs[9] or xs[11] == xs[8] or xs[11] == xs[6]:
                        continue
                    xs[0] = -13 + xs[8] + xs[9]
                    xs[1] = -13 + xs[6] + xs[9]
                    xs[2] = -26 + xs[6] + xs[8] + xs[9] + xs[10] + xs[11]
                    xs[3] = 39 - xs[6] - xs[8] - xs[9] - xs[10]
                    xs[4] = 26 - xs[6] - xs[9] - xs[11]
                    xs[5] = 39 - xs[6] - xs[8] - xs[9] - xs[11]
                    xs[7] = 26 - xs[8] - xs[9] - xs[10]
                    
                    if all([(x==int(x)) for x in xs]) and all([((g>=1) and (g<=12)) for g in xs]):
                        # print(xs)
                        for p,l in enumerate(xs):
                            if l in xs[:p] or l in xs[p+1:]:
                                # print("double")
                                continue
                        options.append(xs.copy())

convert = [10,5,11,4,8,1,3,0,6,7,2,9]
# print(options)
for opt in options:
    np=0
    # print(opt)
    converted = [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]
    for k,y in enumerate(opt):
        if convert[int(y-1)] in converted:
            np = 1
            break
        converted[k] = convert[int(y-1)]
    if not np:
        print(converted)
