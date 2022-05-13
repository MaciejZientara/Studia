
def B(i,j):
    return 'B_%d_%d' % (i,j)
    
# dziedziny B (0/1)
def domains(Bs):
    return [b + " in 0..1" for b in Bs]

# pixele podane jako input
def known_pixels(triples):
    return [(B(i,j) + " #= " + str(val)) for (i,j,val) in triples]

# sumy zamalowanych pixeli w wierszach i kolumnach zgadzaja sie z oczekiwanymi
def correct_sums(R, rows, C, cols):
    result = []
    # rows
    for i in range(R):
        tmp = ""
        for j in range(C):
            tmp += B(i,j)
            if j!= C-1:
                tmp+= " + "
        tmp += " #= " + str(rows[i])
        result.append(tmp)
    # columns
    for j in range(C):
        tmp = ""
        for i in range(R):
            tmp += B(i,j)
            if i!= R-1:
                tmp+= " + "
        tmp += " #= " + str(cols[j])
        result.append(tmp)
    return result

# na podstawie slajdu 44/48 z wykladu 5
# mozna wyodrebnic 3 przypadki do rozpatrzenia
# ulozenie pixeli: 3x1, 1x3, 2x2

# idea wzieta ze slajdu 46/48 z wykladu 5
# B => (A+C > 0)
def oneXthree(R,C):
    result = []
    # rows
    for j in range(C):
        for i in range(R - 2):
            result.append(
                B(i + 1, j) + " #= 1 #==> " + B(i, j) + " + " + B(i + 2, j) + " #> 0"
            )
    # columns
    for i in range(R):
        for j in range(C - 2):
            result.append(
                B(i, j + 1) + " #= 1 #==> " + B(i, j) + " + " + B(i, j + 2) + " #> 0"
            )
    return result

# analizujac slajd 44/48 i rozpisujac sobie
# wszystkie poprawne i niepoprawne ustawienia
# 0/1 w kwadracie 2x2 mozna zauwazyc zaleznosc:
# Wszystkie przypadki gdzie nie ma zamalowanych 2 po skosie
# sa dobre, natomiast jedyny przypadek kiedy sa 2 zamalowane
# po skosie jest wtedy gdy pozostale 2 tez sa zamalowane 
# (wszystkie 4 pixele zamalowane) 
def twoXtwo(R,C):
    result = []
    for i in range(R - 1):
        for j in range(C - 1):
            result.append(
                B(i, j) + " + " + B(i + 1, j + 1) + " #= 2 #<==> "
                + B(i + 1, j) + " + " + B(i, j + 1) + " #= 2"
            )
    return result

def storms(rows, cols, triples):
    writeln(':- use_module(library(clpfd)).')
    
    R = len(rows)
    C = len(cols)
    
    bs = [ B(i,j) for i in range(R) for j in range(C)]
    
    writeln('solve([' + ', '.join(bs) + ']) :- ')
    
    cs = (
        domains(bs)
        + correct_sums(R, rows, C, cols)
        + known_pixels(triples)
        + oneXthree(R, C)
        + twoXtwo(R, C)
    )

    print_constraints(cs, 4, 70)
    writeln('    labeling([ff], [' +  ', '.join(bs) + ']).' )
    writeln(":- tell('prolog_result.txt'), solve(X), write(X), nl, told.")

def print_constraints(Cs, indent, d):
    position = indent
    output = indent * " "# 'tabulacja'
    for c in Cs:
        output += (c + ', ')
        position += len(c)
        if position > d: # ograniczenie ilosci znakow w jednej linii
            writeln(output)
            position = indent
            output = indent * " "# 'tabulacja'
    if output != indent * " ": # wypisz jesli cos zostalo
        writeln(output)

def writeln(s):
    output.write(s + '\n')

txt = open('zad_input.txt').readlines()
output = open('zad_output.txt', 'w')

# map nie ma funkcji len() + potem latwiej sie iterowac po listach
rows = list(map(int, txt[0].split()))
cols = list(map(int, txt[1].split()))
triples = []

for i in range(2, len(txt)):
    if txt[i].strip():
        triples.append(map(int, txt[i].split()))

storms(rows, cols, triples)            
        

