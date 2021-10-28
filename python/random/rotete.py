import csv
import math

def rotate(theta, A):
    Ylen = len(A)
    Xlen = len(A[0])
    Yhalf = Ylen/2.0
    Xhalf = Xlen/2.0
    # print(Xlen,Ylen,Xhalf,Yhalf)
    B = []
    for y in range(Ylen):
        B.append([])
        for x in range(Xlen):
            B[y].append(0)
    # print(B)
    # B = [[0.0] * Xlen] * Ylen
    for y in range(Ylen):
        for x in range(Xlen):
            Ynew = int(round((y-Yhalf)*math.cos(theta) - (x-Xhalf)*math.sin(theta) + Yhalf))
            Xnew = int(round((x-Xhalf)*math.cos(theta) + (y-Yhalf)*math.sin(theta) + Xhalf))
            # print(x,y,Xnew,Ynew)
            if Ynew >= 0 and Xnew >= 0 and Ynew < Ylen and Xnew < Xlen:
                B[Ynew][Xnew] = A[y][x]
                # print(x,y,Xnew,Ynew, A[y][x], B[Ynew][Xnew])

    foundZero = False
    for y in range(Ylen):
        for x in range(Xlen):
            if float(B[y][x]) == 0.0:
                sum = 0.0
                div = 0.0
                if y-1 >= 0:
                    if float(B[y-1][x]) != 0.0:
                        div += 1.0
                        sum += float(B[y-1][x])
                if x-1 >= 0:
                    if float(B[y][x-1]) != 0.0:
                        div += 1.0
                        sum += float(B[y][x-1])
                if y+1 < Ylen:
                    if float(B[y+1][x]) != 0.0:
                        div += 1.0
                        sum += float(B[y+1][x])
                if x+1 < Xlen:
                    if float(B[y][x+1]) != 0.0:
                        div += 1.0
                        sum += float(B[y][x+1])
                if div != 0.0:
                    B[y][x] = str(sum / div)
                if float(B[y][x]) == 0.0:
                    foundZero = True

    while foundZero:
        foundZero = False
        for y in range(Ylen):
            for x in range(Xlen):
                if float(B[y][x]) == 0.0:
                    sum = 0.0
                    div = 0.0
                    if y-1 >= 0:
                        if float(B[y-1][x]) != 0.0:
                            div += 1.0
                            sum += float(B[y-1][x])
                    if x-1 >= 0:
                        if float(B[y][x-1]) != 0.0:
                            div += 1.0
                            sum += float(B[y][x-1])
                    if y+1 < Ylen:
                        if float(B[y+1][x]) != 0.0:
                            div += 1.0
                            sum += float(B[y+1][x])
                    if x+1 < Xlen:
                        if float(B[y][x+1]) != 0.0:
                            div += 1.0
                            sum += float(B[y][x+1])
                    if div != 0.0:
                        B[y][x] = str(sum / div)
                    if float(B[y][x]) == 0.0:
                        foundZero = True

    return B

# for i in range(10):
#     name = str(i)
#     open(name + '.csv')

#     for j in range(10):
#         data2 = data2 = rotate((j+1)*math.pi/6.0, data)
#         with open('./' + name + 'r' + str((j+1)*30) + '.csv', 'w') as f:
#             writer = csv.writer(f)
#             for y in data2:
#                 writer.writerow(y)


with open('test.csv', newline='') as csvfile:
    data = list(csv.reader(csvfile))

# print(data)

# print(str(len(data)) + ',' + str(len(data[0])))

data2 = rotate(math.pi/6.0, data)


for y in data:
    print(y)
print()
for y in data2:
    print(y)


with open('./testr.csv', 'w') as f:
    writer = csv.writer(f)
    for y in data2:
        writer.writerow(y)


# open the file in the write mode
# with open('path/to/csv_file', 'w') as f:

# create the csv writer
# writer = csv.writer(f)

# write a row to the csv file
# writer.writerow(row)
