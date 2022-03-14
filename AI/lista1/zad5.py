
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

def changePixel(idx):
    pass

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

    while not solved():
        randomFill()
        tryToSolve()
    
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
