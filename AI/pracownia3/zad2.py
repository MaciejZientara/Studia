from copy import deepcopy
from collections import deque
from random import randrange

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
def bruteRec(allCorect, block, iter,valiter ,values,ln,valLen,limits):
    if valiter >= valLen:
        if correct(block,values,ln):
            allCorect.append(deepcopy(block))
        return
    if iter >= limits[valiter]:
        return
    blockCopy = deepcopy(block)
    while iter < limits[valiter]:
        if blockCopy[iter]!=-1:
            if (iter+values[valiter]<=ln) and all([block[iter+i]!=-1 for i in range(values[valiter])]):
                for i in range(values[valiter]):
                    blockCopy[iter+i]=1
                bruteRec(allCorect, blockCopy, iter+values[valiter]+1,valiter+1 ,values,ln,valLen,limits)
        blockCopy[iter] = (-1 if block[iter]==0 else block[iter])
        iter += 1

# sprawdzam wszystkie mozliwe ustawienia z uwzglednieniem blokad i pomalowanych pixeli, jesli jakis pixel ma
# taka sama wartosc dla wszystkich ustawien - taka wartosc mu przypisuje (blokada/pomalowanie)
def bruteForce(block,values,ln,valLen,limits):
    result = []
    bruteRec(result,block,0,0,values,ln,valLen,limits)
    sameForAllBlock = [0 for i in range(ln)]
    for r in result:
        for i in range(ln):
            if sameForAllBlock[i]==0:
                sameForAllBlock[i] = (r[i] if r[i]!=0 else -1)
            else:
                if sameForAllBlock[i]!=(r[i] if r[i]!=0 else -1):
                    sameForAllBlock[i] = -2
    state = 0
    for i in range(ln):
        if sameForAllBlock[i]!=-2 and block[i]==0:
            block[i]=sameForAllBlock[i]
            state = 1
    if len(result)==0:
        state = 2
    return state

# na wszystkich wierszach i kolumnach puszcza funkcje powyzej
def tryToSolve():
    state = 0
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
        blockState = bruteForce(block,values,ln,valLen,limits)
        if blockState > state:
            state = blockState
        # 0 - nie nastapily zadne zmiany 
        # 1 - wystapily zmiany, wszystko w porzadku
        # 2 - nie mozna uzupelnic bloku, nalezy odtworzyc arr
        # jesli w chociaz jednym bloku wystapi przypadek o wyzszym priorytecie (wiekszej wartosci)
        # stan dla calej tablicy (return state) zostanie nadpisany

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
    return state

def randomChange():
    global arr
    i = randrange(n)
    j = randrange(m)
    while arr[i][j]!=0:
        i = randrange(n)
        j = randrange(m)
    arr[i][j]=1

def guessingTryToSolve():
    global arr
    que = deque()#przechowuje (kopie arr, counter)
    # kiedy martwy stan przywracam poprzedni zapisany i zmniejszam counter
    # kiedy counter == 0, zdejmuje ten stan ze stosu i przywracam poprzedni 
    que.append((deepcopy(arr),-1))#zapisuje stan poczatkowy, zeby nigdy nie probowac wczytac z pustej kolejki
    while not solved():
        printArr()
        print()
        state = tryToSolve()
        if state == 0:#brak zmian, trzeba zapisac stan arr w que i pomalowac losowy pixel
            que.append((deepcopy(arr),5))
            randomChange()
        if state == 2:#martwy stan, trzeba przywrocic poprzednie arr z que
            while True:
                if que[-1][1] > 0:
                    (ar,c) = que.pop()
                    c-=1
                    if c > 0:
                        que.append((ar,c))
                    else:
                        break
            arr = deepcopy(que[-1][0])

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

    guessingTryToSolve()

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
