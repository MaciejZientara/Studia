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
# lista list wszystkich poprawnych ustawien blokow (3 zagniezdzone listy wliczajac zapisane block - tez listy)
allCorrectForAll = []
# lista numerow kolumn/wierszy dajaca lepsze wyniki losowego uzupelniania
betterOrder = []

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

# zwraca bool czy calosc rozwiazana czy nie + update completedRowCol
def solved():
    global rows
    global columns
    #pierwsze n wartosci to indeksy rows, kolejne m to indeksy columns
    for i in range(n+m):
        idx = i
        rowColumn = True #True - row, False - column
        if idx >= n:
            idx-=n
            rowColumn = False
        values = (rows[idx] if rowColumn==True else columns[idx])

        ln = n if (rowColumn == False) else m
        ln+=2
        block = [0] * ln
        for j in range(ln):
            block[j] = (arr[idx+1][j] if (rowColumn == True) else arr[j][idx+1])
        completedRowCol[i] = correct(block,values,ln)
    return all(completedRowCol)

# zwraca bool czy dany block z allCorrectForAll[i] jest dalej poprawny w stosunku do aktualnej zawartosci arr
def isStillCorrect(block,i):
    idx = i
    rowColumn = True #True - row, False - column
    if idx >= n:
        idx-=n
        rowColumn = False
    ln = n if (rowColumn == False) else m
    ln+=2
    for i in range(ln):
        B = block[i]
        A = (arr[idx+1][i] if rowColumn else arr[i][idx+1])
        if (A!=0) and (A!=B):
            return False
    return True

# zwraca liste wszystkich dajej poprawnych blokow z allCorrectForAll[i]
def findAllStillCorrect(idx):#                                                    TODO binarne przeszukiwanie, zamiast liniowego
    return [a for a in allCorrectForAll[idx] if isStillCorrect(a,idx)]

# sprawdzam wszystkie mozliwe i StillCorrect ustawienia
# jesli jakis pixel ma taka sama wartosc dla wszystkich ustawien, to taka wartosc mu przypisuje (blokada/pomalowanie)
def deduction(block,i,ln):
    result = findAllStillCorrect(i)
    sameForAllBlock = [0 for i in range(ln)]
    for r in result:
        for i in range(ln):
            if sameForAllBlock[i]==0:
                sameForAllBlock[i] = (r[i] if r[i]!=0 else -1)
            else:
                if sameForAllBlock[i]!=(r[i] if r[i]!=0 else -1):
                    sameForAllBlock[i] = -2
    state = 0
    if len(result)==0:
        state = 2
    else:
        for i in range(ln):
            if sameForAllBlock[i]!=-2 and block[i]==0:
                block[i]=sameForAllBlock[i]
                state = 1
    return state

# na wszystkich wierszach i kolumnach puszcza funkcje deduction
def tryToDeduce():
    state = 0
    for i in range(n+m):
        idx = i
        
        rowColumn = True #True - row, False - column
        if idx >= n:
            idx-=n
            rowColumn = False

        ln = n if (rowColumn == False) else m
        ln+=2
        block = [0] * (ln)
        for j in range(ln):
            block[j] = (arr[idx+1][j] if (rowColumn == True) else arr[j][idx+1])

        #jezeli jest zakonczony i poprawny, continue
        #jezeli jest zakonczony i bledny, zwroc stan martwy - nalezy odtworzyc arr
        #jezeli nie zakonczony to sprobuj dedukcji
        if completedRowCol[i] == True:
            if correct(block,(rows[idx] if rowColumn==True else columns[idx]),ln) == False:
                return 2
            else:
                continue

        # tutaj operacje na block
        blockState = deduction(block,i,ln)
        # print("block",block)
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
    return state

progressCounter = 0
def tryToSolve():
    global arr
    global completedRowCol
    global progressCounter
    global betterOrder
    global allCorrectForAll

    while not solved():
        #progressCounter sluzy tylko do obserwacji postepu pracy programu
        print(progressCounter,end='\r')
        progressCounter+=1
        # printArr()
        # print()
        state = tryToDeduce()
        if state == 0:#brak zmian, trzeba zapisac stan arr 
            iter = 0
            while completedRowCol[betterOrder[iter]] == True:
                iter+=1
                if iter==(n+m):
                    return False
            idx = betterOrder[iter]
            resultBlock = findAllStillCorrect(idx)
            completedRowCol[idx] = True
            rowColumn = True #True - row, False - column
            if idx >= n:
                idx-=n
                rowColumn = False
            ln = n if (rowColumn == False) else m # dlugosc wiersza = ilosc kolumn
            ln+=2
            arrCopy = deepcopy(arr)
            for res in resultBlock:
                for i in range(ln):
                    if rowColumn:
                        arr[idx+1][i] = res[i]
                    else:
                        arr[i][idx+1] = res[i]
                if not tryToSolve():
                    arr = deepcopy(arrCopy)
                    solved()#update completedRowCol
                else:
                    return True
            completedRowCol[(idx if rowColumn else idx+n)] = False
            return False
        if state == 2:#martwy stan, trzeba przywrocic poprzednie arr
            return False
    return True

# rekurencyjna funkcja pomocnicza dla generateCorect
def findAllCorect(allCorect, block, iter,valiter ,values,ln,valLen):
    if valiter >= valLen:
        if correct(block,values,ln):
            allCorect.append(deepcopy(block))
        return
    limit = ln-values[valiter]#tego juz nie rozpatrywac! 
    if iter >= limit:
        return
    while iter < limit:
        for i in range(values[valiter]):
            block[iter+i] = 1
        findAllCorect(allCorect, block, iter+values[valiter]+1,valiter+1 ,values,ln,valLen)
        for i in range(values[valiter]):
            block[iter+i] = -1
        iter+=1

def generateCorect(idx,rowColumn):
    values = (rows[idx] if rowColumn==True else columns[idx])
    ln = n if (rowColumn == False) else m
    ln+=2
    block = [-1] * (ln)
    valLen = len(values)
    result = []
    findAllCorect(result,block,1,0,values,ln,valLen)
    return result

def preprocessing():
    #przygotowanie tablicy rozmiaru n x m
    global arr
    global completedRowCol
    global allCorrectForAll
    global betterOrder

    # for i in range(min(n,m)):
    #     betterOrder.append(i)
    #     betterOrder.append(i+n)
    # for i in range(min(n,m),max(n,m)):
    #     betterOrder.append(i)
    
    arr = [[0 for j in range(m+2)] for i in range(n+2)]
    completedRowCol = [False for i in range(n+m)]

    # dla ulatwienia operacji, ograniczam caly obrazek blokada
    for i in range(n+2):
        arr[i][0] = -1
        arr[i][m+1] = -1
    for j in range(m+2):
        arr[0][j] = -1
        arr[n+1][j] = -1
    
    for i in range(n+m):
        idx = i
        rowColumn = True #True - row, False - column
        if idx >= n:
            idx-=n
            rowColumn = False
        allCorrectForAll.append(generateCorect(idx,rowColumn))

    orderHelper = [(len(allCorrectForAll[i]),i) for i in range(n+m)]
    for o in sorted(orderHelper):
        betterOrder.append(o[1])


# przygotowuje plansze i stara sie ja rozwiazac
def nonogram():
    preprocessing()
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
