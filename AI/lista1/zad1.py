from copy import deepcopy

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

def isCheck(wKing,wRook,bKing):#czarny krol szachowany przez wieze
    return (bKing[0]==wRook[0]) or (bKing[1]==wRook[1])

def isOutsideBoard(figure):
    return figure[0] < 'a' or figure[0] > 'h' or figure[1] < '1' or figure[1] > '8'

def areKingsTooClose(king1,king2):
    return (abs(ord(king1[0]) - ord(king2[0])) <= 1) and (abs(ord(king1[1]) - ord(king2[1])) <= 1)

def isPat(wKing,wRook,bKing):#czarny krol nie moze wykonac zadnego ruchu
    for move in kingMoves:
        tmpKing = move(bKing)
        if not (isOutsideBoard(tmpKing) or isCheck(wKing,wRook,tmpKing) or areKingsTooClose(wKing,tmpKing)):
            return False
    return True
    #pat kiedy wszystkie ruchy sa niepoprawne, jesli ruch poprawny to return false
    
def isValidMove(turn,wKing,wRook,bKing):
    if wKing == bKing or wRook == bKing or wRook == wKing:
        return False #wejscie na inna figure
    if areKingsTooClose(wKing,bKing):
        return False #wejscie krola w zakres krola
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
        que.append([turn,wKing,wRook,bKing,count,path])
        if isWin(wKing,wRook,bKing):
            que[0],que[-1] = que[-1],que[0] #znalezione zwyciestwo, w nastepnym while w round wyjscie z petli
            # print('win',que[0], isWin(que[0][1],que[0][2],que[0][3]))
    return

def round(Turn,whiteKing,whiteRook,blackKing):
    que = []

    que.append([Turn,whiteKing,whiteRook,blackKing,0,[]])

    while que: # que not empty
        [turn,wKing,wRook,bKing,count,path] = que.pop(0)

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
                if not isOutsideBoard(WKing):
                    queueInsert(que,'black',WKing,wRook,bKing,count+1,path)
            for mv in rookMoves:
                tmp = wRook
                for i in range(8):
                    tmp = mv(tmp)
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