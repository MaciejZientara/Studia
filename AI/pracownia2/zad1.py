from copy import deepcopy

n = 0
m = 0
# dlugosci segmentow
rows = []
columns = []
# prawda/falsz czy kolumna/wiersz skonczone
completedRowCol = []
# 3 stany: -1 (blocked, tutaj nie mozna pokolorowac), 0 (empty, mozna pomalowac), 1 (filled, pokolorowane)
arr = []

# zwraca bool czy dany rzad/kolumna rozwiazana
def correct(block,values,ln):
    count = sum([block[i]==1 for i in range(ln)])
    if count != sum(values):
        return False
    
    onesInRow = 0 #jedynki pod rzad
    valIndex = 0
    for b in block:
        if b!=1:
            if onesInRow!=0:
                if onesInRow!=values[valIndex]:
                    return False
                else:
                    valIndex+=1
            onesInRow = 0
        else:
            onesInRow+=1
    return True

# zwraca bool czy calosc rozwiazana czy nie
def solved():
    #pierwsze n wartosci to indeksy rows, kolejne m to indeksy columns
    for i in range(n+m):
        if completedRowCol[i] == False:
            idx = i
            rowColumn = True #True - row, False - column
            if idx >= n:
                idx-=n
                rowColumn = False
            values = (rows[idx] if rowColumn==True else columns[idx])

            ln = n if (rowColumn == False) else m
            block = [0] * ln
            for j in range(ln):
                block[j] = (arr[idx+1][j+1] if (rowColumn == True) else arr[j+1][idx+1])
            if correct(block,values,ln) == False:
                return False
            else:
                completedRowCol[i] = True
    return True

# rekurencyjna funkcja pomocnicza dla brute force
def bruteRec(sameForAllBlock, block, iter,valiter ,values,ln,valLen,limits):
    if valiter >= valLen:
        if correct(block,values,ln):
            for i in range(ln):
                if sameForAllBlock[i]==0:
                    sameForAllBlock[i] = (block[i] if block[i]!=0 else -1)
                else:
                    if sameForAllBlock[i]!=(block[i] if block[i]!=0 else -1):
                        sameForAllBlock[i] = -2
        return
    if iter >= limits[valiter]:#ln:
        return
    blockCopy = deepcopy(block)
    while iter < limits[valiter]:#ln
        if blockCopy[iter]!=-1:
            if (iter+values[valiter]<=ln) and all([block[iter+i]!=-1 for i in range(values[valiter])]):
                for i in range(values[valiter]):
                    blockCopy[iter+i]=1
                bruteRec(sameForAllBlock, blockCopy, iter+values[valiter]+1,valiter+1 ,values,ln,valLen,limits)
        blockCopy[iter] = (-1 if block[iter]==0 else block[iter])
        iter += 1

# sprawdzam wszystkie mozliwe ustawienia z uwzglednieniem blokad i pomalowanych pixeli, jesli jakis pixel ma
# taka sama wartosc dla wszystkich ustawien - taka wartosc mu przypisuje (blokada/pomalowanie)
def bruteForce(block,values,ln,valLen,limits):
    result = [0 for i in range(ln)]
    bruteRec(result,block,0,0,values,ln,valLen,limits)
    for i in range(ln):
        if result[i]!=-2 and block[i]==0:
            block[i]=result[i]

# na wszystkich wierszach i kolumnach puszcza funkcje powyzej
def tryToSolve():
    for i in range(n+m):
        if completedRowCol[i] == True:
            continue
        idx = i
        
        rowColumn = True #True - row, False - column
        if idx >= n:
            idx-=n
            rowColumn = False
        values = (rows[idx] if rowColumn==True else columns[idx])

        ln = n if (rowColumn == False) else m
        ln+=2
        block = [0] * (ln)
        for i in range(ln):
            block[i] = (arr[idx+1][i] if (rowColumn == True) else arr[i][idx+1])

        valLen = len(values)

        limits = [0 for v in values]
        limits[0] = ln-(sum(values)+valLen-2)
        for i in range(1,valLen):
            limits[i] = limits[i-1]+values[i-1]+1

        # tutaj operacje na block
        bruteForce(block,values,ln,valLen,limits)

        # zapisz zmiany
        for i in range(ln):
            if rowColumn == True:
                arr[idx+1][i] = block[i]
            else: 
                arr[i][idx+1] = block[i]
        if rowColumn==True:
            rows[idx] = values
        else:
            columns[idx] = values

# przygotowuje plansze i stara sie ja rozwiazac
def nonogram():
    #przygotowanie tablicy rozmiaru n x m
    global arr
    global completedRowCol
    arr = [[0 for j in range(m+2)] for i in range(n+2)]
    completedRowCol = [False for i in range(n+m)]

    # dla ulatwienia operacji, ograniczam caly obrazek blokada
    for i in range(n+2):
        arr[i][0] = -1
        arr[i][m+1] = -1
    for j in range(m+2):
        arr[0][j] = -1
        arr[n+1][j] = -1

    while not solved():
        tryToSolve()

    printArr()
    return

# wypisuje cala plansze
def printArr():
    for a in arr[1:-1]:
        for b in a[1:-1]:
            print(('#' if b==1 else '.' if b==0 else 'x'),end='')
            out.write(('#' if b==1 else '.'))
        print()
        out.write('\n')

with open('zad_output.txt','w') as out:
    with open('zad_input.txt','r') as file:
        first = True
        copyOfN = 0
        for line in file:
            if first:
                [n,m] = line.split()
                [n,m] = [int(n),int(m)]
                copyOfN = n
                first = False
            else:
                values = [int(x) for x in line.split()]
                if copyOfN > 0:
                    copyOfN-=1
                    rows.append(values)
                else:
                    columns.append(values)
        nonogram()
