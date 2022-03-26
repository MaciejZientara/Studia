
import random

n = 0
m = 0
rows = []
columns = []

arr = []

def correct(idx):
    val = (rows[idx] if idx<n else columns[idx-n])

    rowColumn = True #True - row, False - column
    if idx >= n:
        idx-=n
        rowColumn = False

    if rowColumn:#row
        count = sum([1 for i in range(n) if arr[idx][i]==1])#czy ilosc 1 jest rowna val
        if count != val:
            return False

        for i in range(n-val+1):#czy jeden spojny ciag o dlugosci >= val
            if arr[idx][i] == 1:
                for j in range(val):
                    if arr[idx][i+j] != 1:
                        return False
                return True
        return False
    else:#column
        count = sum([1 for i in range(m) if arr[i][idx]==1])#czy ilosc 1 jest rowna val
        if count != val:
            return False

        for i in range(m-val+1):#czy jeden spojny ciag o dlugosci >= val
            if arr[i][idx] == 1:
                for j in range(val):
                    if arr[i+j][idx] != 1:
                        return False
                return True
        return False

def solved():
    for i in range(n+m):#pierwsze n wartosci to indeksy rows, kolejne m to indeksy columns
        if correct(i) == False:
            return False

    return True

def randomFill():
    for a in arr:
        for i in range(len(a)):
            a[i] = (1 if random.random()>=0.5 else 0)
    return

def zad4(idx):
    val = (rows[idx] if idx<n else columns[idx-n])
    rowColumn = True #True - row, False - column
    if idx >= n:
        idx-=n
        rowColumn = False

    res = n*m
    if rowColumn:#row
        oneCount = sum([arr[idx][i]==1 for i in range(n)])
        for i in range(n-val+1):
            changes = 0
            oneCopy = oneCount
            for j in range(val):
                if arr[idx][i+j] == 0:
                    changes+=1
                else:
                    oneCopy-=1
            changes+=oneCopy
            res = min(res,changes)
        return res
    else:#column
        oneCount = sum([arr[i][idx]==1 for i in range(n)])
        for i in range(m-val+1):
            changes = 0
            oneCopy = oneCount
            for j in range(val):
                if arr[i+j][idx] == 0:
                    changes+=1
                else:
                    oneCopy-=1
            changes+=oneCopy
            res = min(res,changes)
        return res


def switchArr(i,j):
    arr[i][j] = 1 if arr[i][j] == 0 else 0

def changePixel(idx):
    rowColumn = True #True - row, False - column
    if idx >= n:
        idx-=n
        rowColumn = False

    minVal, storedIdx = n*m,-1
    if rowColumn:#row
        for i in range(m):
            switchArr(idx,i)
            tmp = zad4(i) + zad4(idx)
            if tmp < minVal:
                minVal = tmp
                storedIdx = i
            switchArr(idx,i)
        switchArr(idx,storedIdx)    
    else:#column
        for i in range(n):
            switchArr(i,idx)
            tmp = zad4(i) + zad4(idx)
            if tmp < minVal:
                minVal = tmp
                storedIdx = i
            switchArr(i,idx)
        switchArr(storedIdx,idx)
    return


def tryToSolve():
    counter = 2*n*m #po jakims czasie zacznij od poczatku
    while counter>0 and not solved():
        counter-=1
        idx = random.randrange(0,n+m)
        #pierwsze n wartosci to indeksy rows, kolejne m to indeksy columns
        while correct(idx):#szukamy zlej kolumny/wiersza
            if random.random() < 0.05: #mala szansa na popsucie dobrego ustawienia
                break
            else:
                idx = random.randrange(0,n+m)
        #mamy index kolumny/wiersza do zamienienia jednego pixela
        changePixel(idx)

def nonogram():
    for i in range(n):#przygotowanie tablicy rozmiaru n x m
        arr.append([0 for j in range(m)])

    randCount = 0
    while not solved():
        randCount+=1
        randomFill()
        tryToSolve()
    print('ilosc randomFill:',randCount)

    printArr()
    return

def printArr():
    for a in arr:
        for b in a:
            print(('.' if b==0 else '#'),end='')
            out.write(('.' if b==0 else '#'))
        print()
        out.write('\n')
    

with open('zad5_output.txt','w') as out:
    first = True
    copyN = 0
    with open('zad5_input.txt','r') as file:
        for line in file:
            if first:
                [n,m] = line.split()
                [n,m] = [int(n),int(m)]
                copyN = n
                first = False
            else:
                if copyN > 0:
                    copyN-=1
                    rows.append(int(line))
                else:
                    columns.append(int(line))
        nonogram()

# 4 4
# 1
# 2
# 3
# 4
# 4
# 3
# 2
# 1
