# dla ciagu 0/1 dlugosci n i wczytanej wartosci m
# w czasie O(n*m) sprawdzam wszystkie kombinacje
# jesli jest 1 to zmniejszam sume pozostalych 1,
# jesli jest 0 to zwiekszam ilosc zmian
# wynik to ilosc zmian + ilosc pozostalych 1

def nonogram(row, n):
    oneCount = sum([r=='1' for r in row])
    res = len(row)
    for i in range(len(row)-n+1):
        changes = 0
        oneCopy = oneCount
        for j in range(n):
            if row[i+j] == '0':
                changes+=1
            else:
                oneCopy-=1
        changes+=oneCopy
        res = min(res,changes)
    return res

with open('zad4_output.txt','w') as out:
    with open('zad4_input.txt','r') as file:
        for line in file:
            [row,n] = line.split()
            n = int(n)
            out.write(str(nonogram(row,n))+'\n')
            # print(nonogram(row,n))

