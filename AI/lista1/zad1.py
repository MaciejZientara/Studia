from copy import deepcopy
from collections import deque
from math import sqrt

# plansza
# 8
# |
# 1
#  A - H

def moveU(figure):
    return figure[0] + chr(ord(figure[1])+1)

def moveD(figure):
    return figure[0] + chr(ord(figure[1])-1)

def moveR(figure):
    return chr(ord(figure[0])+1) + figure[1]

def moveL(figure):
    return chr(ord(figure[0])-1) + figure[1]

def moveUL(figure):
    return chr(ord(figure[0])-1) + chr(ord(figure[1])+1)
    
def moveUR(figure):
    return chr(ord(figure[0])+1) + chr(ord(figure[1])+1)

def moveDL(figure):
    return chr(ord(figure[0])-1) + chr(ord(figure[1])-1)
    
def moveDR(figure):
    return chr(ord(figure[0])+1) + chr(ord(figure[1])-1)

kingMoves = [moveL,moveD,moveR,moveU,moveUL,moveDL,moveDR,moveUR]
rookMoves = [moveL,moveD,moveR,moveU]

def areFiguresTooClose(figure1,figure2):
    return (abs(ord(figure1[0]) - ord(figure2[0])) <= 1) and (abs(ord(figure1[1]) - ord(figure2[1])) <= 1)

def isCheck(wKing,wRook,bKing):#czarny krol szachowany przez wieze i nie moze zbic tej wiezy!
    return ((bKing[0]==wRook[0]) or (bKing[1]==wRook[1]))# and not areFiguresTooClose(wRook,bKing)

def isOutsideBoard(figure):
    return figure[0] < 'a' or figure[0] > 'h' or figure[1] < '1' or figure[1] > '8'

def isPat(wKing,wRook,bKing):#czarny krol nie moze wykonac zadnego ruchu
    for move in kingMoves:
        tmpKing = move(bKing)
        if not (isOutsideBoard(tmpKing) or isCheck(wKing,wRook,tmpKing) or areFiguresTooClose(wKing,tmpKing)):
            return False
    return True
    #pat kiedy wszystkie ruchy sa niepoprawne, jesli ruch poprawny to return false

def isWrongDirection(bKing,old,new):#nie oplaca sie oddalac od czarnego krola!
    dist = lambda a,b : sqrt((ord(a[0])-ord(b[0]))**2 + (ord(a[1])-ord(b[1]))**2)
    return dist(bKing,old) + 1 < dist(bKing,new)
    #odleglosc zwiekszyla sie o wiecej niz 1

def isValidMove(turn,wKing,wRook,bKing):
    if wKing == bKing or wRook == bKing or wRook == wKing:
        return False #wejscie na inna figure
    if areFiguresTooClose(wKing,bKing):
        return False #wejscie krola w zakres krola
    if areFiguresTooClose(wRook,bKing):
        return False #zbicie wiezy = fail
    if turn == 'white' and isCheck(wKing,wRook,bKing):#czarny zrobil ruch, tura bialego
        return False #czarny dobrowolnie wszedl pod wieze
    if turn == 'black' and isPat(wKing,wRook,bKing) and not isCheck(wKing,wRook,bKing):
        return False #pat = fail, pat+check = win <- nie mozna zakazac takiego ruchu!
    return True

def isWin(wKing,wRook,bKing):#krol szachowany i nie moze zrobic ruchu
    return isCheck(wKing,wRook,bKing) and isPat(wKing,wRook,bKing)

def queueInsert(que,turn,wKing,wRook,bKing,count,path):
    if count > 30:
        return #nie udalo sie znalezc rozwiazania, czyszczenie listy

    if isValidMove(turn,wKing,wRook,bKing):
        if isWin(wKing,wRook,bKing):
            que.appendleft([turn,wKing,wRook,bKing,count,path])#znalezione zwyciestwo, w nastepnym while w round wyjscie z petli
        else:
            que.append([turn,wKing,wRook,bKing,count,path])
    return

def round(Turn,whiteKing,whiteRook,blackKing):
    que = deque()

    que.append([Turn,whiteKing,whiteRook,blackKing,0,[]])

    while que: # que not empty
        [turn,wKing,wRook,bKing,count,path] = que.popleft()

        # print(turn,wKing,wRook,bKing,count)

        if not __debug__:
            path = deepcopy(path) #aby w kazdym kroku nie bylo referencji do tego samego obiektu, tylko nowa lista
            path.append([wKing,wRook,bKing])

        if isWin(wKing,wRook,bKing):
            if not __debug__:
                # print(path)
                out.write(str(path)+'\n')
            return count

        if turn == 'black':
            for move in kingMoves:
                BKing = move(bKing)
                if not isOutsideBoard(BKing):
                    queueInsert(que,'white',wKing,wRook,BKing,count+1,path)
        else: #white
            for move in kingMoves:
                WKing = move(wKing)
                if isWrongDirection(bKing,wKing,WKing):
                    continue
                if not isOutsideBoard(WKing):
                    queueInsert(que,'black',WKing,wRook,bKing,count+1,path)
            for mv in rookMoves:
                tmp = wRook
                for i in range(8):
                    tmp = mv(tmp)
                    if isWrongDirection(bKing,wRook,tmp):
                        break
                    if tmp == bKing or tmp == wKing or isOutsideBoard(tmp): #wieza natrafila na przeszkode, nie moze dalej isc
                        break
                    queueInsert(que,'black',wKing,tmp,bKing,count+1,path)
    return 'INF'


with open('zad1_input.txt','r') as file:
    with open('zad1_output.txt','w') as out:
        for line in file:
            [turn,wKing,wRook,bKing] = line.split()
            found = False
            res = round(turn,wKing,wRook,bKing)
            # print(res)
            out.write(str(res)+'\n')

# black f2 h6 h1
# white f2 c6 h1
# white f3 c6 h2
# black c4 c8 h3
# white a1 b2 c3

# zwykly tryb:
# python3 zad1.py 
# tryb debug:
# python3 -O zad1.py 