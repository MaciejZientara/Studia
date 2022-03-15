# dla ciagu 0/1 dlugosci n i wczytanej wartosci m
# w czasie O(n*m) sprawdzam wszystkie kombinacje
# jesli jest 1 to zmniejszam sume pozostalych 1,
# jesli jest 0 to zwiekszam ilosc zmian
# wynik to ilosc zmian + ilosc pozostalych 1

def nonogram(row, n):
    switch = sum([r=='1' for r in row])
    res = switch + sum([1 if row[i]=='0' else -1 for i in range(n)])
    tmp = res
    for i in range(len(row)-n):
        tmp+= (1 if row[i]=='1' else -1)
        tmp+= (-1 if row[i+n]=='1' else 1)
        res = min(res,tmp)
    return res

# najpierw sprawdzam pierwsze n pozycji ile tam jest zmian
# potem gąsiennicowo przesuwam się o jeden, usuwam zmiane wykonaną na poprzedniej pozycji i dodaje ilość zmian na następnej pozycji
# czyli najpierw [0,n], [1,n+1], [2,n+2], ... i sprawdzam liniowo całość, a potem biorę minimum z tych ramek

with open('zad4_output.txt','w') as out:
    with open('zad4_input.txt','r') as file:
        for line in file:
            [row,n] = line.split()
            n = int(n)
            out.write(str(nonogram(row,n))+'\n')
            # print(nonogram(row,n))

