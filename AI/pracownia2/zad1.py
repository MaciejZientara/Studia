n = 0
m = 0
# pary (dlugosc,czy skonczony blok)
rows = []
columns = []
# 3 stany: -1 (blocked, tutaj nie mozna pokolorowac), 0 (empty, mozna pomalowac), 1 (filled, pokolorowane)
arr = []

# zwraca bool czy dany rzad/kolumna rozwiazana
def correct(idx):
    rowColumn = True #True - row, False - column
    if idx >= n:
        idx-=n
        rowColumn = False
    values = (rows[idx] if rowColumn==True else columns[idx])

    ln = n if (rowColumn == False) else m
    block = [0] * ln
    for i in range(ln):
        block[i] = (arr[idx][i] if (rowColumn == True) else arr[i][idx])

    count = sum([block[i] for i in range(ln)])
    if count != sum([v[0] for v in values]):
        return False
    
    onesInRow = 0 #jedynki pod rzad
    valIndex = 0
    for b in block:
        if b!=1:
            if onesInRow!=0:
                if onesInRow!=values[valIndex][0]:
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
        if correct(i) == False:
            return False
    return True

#wypelnia caly blok: kolor lub blokada
def obviousFill(block,values,ln,valLen):
    if not values or values[0][0] == 0:#pusty blok
        values[0][1] = True
        for i in range(ln):
            block[i] = -1

    suma = valLen-1
    for i in range(valLen):
        suma+=values[i][0]
    if suma == (ln-2):#wypelnia caly blok
        idx = 1
        for v in range(valLen):
            values[v] = (values[v][0],True)
            for i in range(values[v][0]):
                block[idx] = 1
                idx+=1
            block[idx] = -1
            idx+=1

# niewazne jak ustawie segment pomiedzy blokadami, pewne pixele musza byc zamalowane
def sureColor(block,values,ln,valLen):
    if not values or values[0][0] == 0:#pusty blok
        return
    suma = valLen-1
    for i in range(valLen):
        suma+=values[i][0]
    if suma > int((ln-2)/2):#wypelnia co najmniej polowe bloku
        # znalazlem sposob tylko kiedy jest 1 lub 2 segmenty
        if valLen == 1:
            for j in range(ln-values[0][0]-1,values[0][0]+1):
                block[j] = 1
        if valLen == 2:
            if values[0][0] > values[1][0]:
                for j in range(ln-values[0][0]-values[1][0]-2,values[0][0]+1):
                    block[j] = 1
            else:
                for j in range(ln-values[0][0]-1,values[0][0]+values[1][0]+2):
                    block[j] = 1
        # if valLen == 3:#                                                                  !!! TODO !!!

# przed pierwszym i po ostatnim segmencie blokada + jesli wszystkie segmenty reszta pól to blokady
def blockWhenCompleted(block,values,ln):
    if values[0][1]:#przed pierwszym
        idx = 0
        while block[idx] != 1:
            block[idx] = -1
            idx += 1
    if values[-1][1]:#za ostatnim
        idx = -1
        while block[idx] != 1:
            block[idx] = -1
            idx -= 1
    if all([v[1] for v in values]):#cały block zakonczony - zablokowac pozostale
        for i in range(ln):
            if block[i] != 1:
                block[i] =- 1

# oznacz zakonczone segmenty, jesli dwa pomalowane pixele nie moga nalezec do 2 roznych segmentow - polacz je ze soba
def tryToComplete(block,values,ln,valLen):
    # sums[i] = suma wszystkich values od 0 do i włącznie
    sums = [v[0] for v in values]
    for i in range(1,valLen):
        sums[i] += sums[i-1]

    iter = 0
    while block[iter]!=1:
        iter+=1
    if iter-1-sums[0] < 1:#czy pierwszy pokolorowany pixel nalezy do pierwszego segmentu
        ;


    # moge zrobic brute force:
    # chce poukladac wszystkie segmenty w zadanym blocku z uwzglednieniem blokad i juz pomalowanych pixeli
    # jezeli jakis pixel jest pomalowany w kazdym ustawieniu to trzeba go pomalowac, 
    # jezeli jest zablokowany to trzeba go zablokowac
    # a kiedy ustawie wszystkie segmenty to pozostale pola oznaczam jako zablokowane
    # 
    # ten brute force rozwiaze mi bardzo duzo problemow,
    # obviousFill, sureColor, blockWhenCompleted, firstLastDon'tFit
    # ale tez trudniejsze czyli dodanie blokad jesli jakis segment jest zakotwiczony
    # do jakiegos pokolorwanego pixela i nie ma mozliwosci dosiegnac do pewnych pol - te pola zablokowac
    # 
    # najpierw mimo wszystko wykonam pozostale funkcje - zmniejszy naklad pracy brute force
    # dodac tablice zapisujaca czy dany blok juz spelniony (wszystkie values spelnione) - szybsze sprawdzanie
    # czy warto puszczac ponownie te wszystkie funkcje czy nie ma sensu 







    # jezeli jest ograniczone blokada z jednej strony to moge rozszerzac dopoki nie bedzie mialo dlugosci 
    # oczekiwanej dla tego segmentu (skad wiem ktory to segment?)
    
    # jezeli jest ograniczone z jednej strony to moze poszukac gdzie jest nastepna blokada i rozpatrywac przedzialy
    # zamiast cale bloki... 

    # 
    # co jezeli nie ograniczone z obu stron ale ma dlugosc oczekiwana dla pewnego segmentu
    # 
    # co jezeli ograniczone z obu stron blokadami ale jest kilka segmentow tej samej dlugosci i nie da sie okreslic ktory zostal zakocznony... 


    # jeszcze tutaj dodac wypelnianie block pomiedzy zakonczonymi i oraz i+1 segmentem

    # jezeli znalazlem 1 i natrafilem na blokade to bede rozpatrywac kolejny val index
    # blok jest skonczony jezeli przynajmniej z jednej strony ma blokade
    # dodaje blokade z drugiej strony i zapisuje, ze skonczony - tutaj uzywam onesInRow
    
    onesInRow = 0 #jedynki pod rzad
    valIndex = 0
    for i in range(ln):#znajdz dokonczone segmenty
        if block[i]!=1:
            if onesInRow!=0:
                if onesInRow==values[valIndex][0]:#dokonczony segment
                    values[valIndex] = (values[valIndex][0],True)
                    block[i] = -1#blokada po segmencie
                    block[i-1-values[valIndex][0]] = -1#blokada przed segmentem
                if block[i]==-1:
                    valIndex+=1
            onesInRow = 0
        else:
            onesInRow+=1
    print(block,values)

# jesli pierwszy/ostatni segment jest dluzszy niz przestrzen miedzy blokadami - zablokuj cala ta przestrzen
def firstLastDontFit(block,values,ln):
    couldFit = True
    for i in range(1,values[0][0]+1):
        if block[i] == -1:
            couldFit = False
    if couldFit == False:
        for i in range(1,values[0][0]+1):
            block[i] == -1

    couldFit = True
    for i in range(ln-values[-1][0]-1,ln):
        if block[i] == -1:
            couldFit = False
    if couldFit == False:
        for i in range(ln-values[-1][0]-1,ln):
            block[i] == -1

# na wszystkich wierszach i kolumnach puszcza funkcje powyzej
def tryToSolve():
    for i in range(n+m):
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
        # tutaj operacje na block
        obviousFill(block,values,ln,valLen)
        sureColor(block,values,ln,valLen)
        tryToComplete(block,values,ln,valLen)
        blockWhenCompleted(block,values,ln)
        firstLastDontFit(block,values,ln)

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

def nonogram():
    #przygotowanie tablicy rozmiaru n x m
    global arr
    arr = [[0 for j in range(m+2)] for i in range(n+2)]

    # dla ulatwienia operacji, ograniczam caly obrazek blokada
    for i in range(n+2):
        arr[i][0] = -1
        arr[i][m+1] = -1
    for j in range(m+2):
        arr[0][j] = -1
        arr[n+1][j] = -1

    # while not solved():
    #     tryToSolve()

    tryToSolve()
    # solved()
    # tryToSolve()
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
                values = [(int(x),False) for x in line.split()]
                if copyOfN > 0:
                    copyOfN-=1
                    rows.append(values)
                else:
                    columns.append(values)
        nonogram()
