from copy import deepcopy

moves = 0

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
    return moveU(moveL(figure))
    
def moveUR(figure):
    return moveU(moveR(figure))

def moveDL(figure):
    return moveD(moveL(figure))    
    
def moveDR(figure):
    return moveD(moveR(figure))
    
def isValidMove(wKing,wRook,bKing):
    if (wKing[0] < 'a' or wKing[0] > 'h' or wKing[1] < '1' or wKing[1] > '8' or 
        wRook[0] < 'a' or wRook[0] > 'h' or wRook[1] < '1' or wRook[1] > '8' or 
        bKing[0] < 'a' or bKing[0] > 'h' or bKing[1] < '1' or bKing[1] > '8'):
        return False #wyjscie poza plansze
    if wKing == bKing or wRook == bKing or wRook == wKing:
        return False #wejscie na inna figure
    if (abs(ord(wKing[0]) - ord(bKing[0])) <= 1) and (abs(ord(wKing[1]) - ord(bKing[1])) <= 1):
        return False #wejscie krola w zakres krola
    if (bKing[0] == wRook[0]) or (bKing[1] == wRook[1]):
        return False #wejscie krola pod wieze
    return True

def isCheck(wKing,wRook,bKing):#czarny krol szachowany przez wieze
    return (bKing[0]==wRook[0]) or (bKing[1]==wRook[1])

def isPat(wKing,wRook,bKing):#czarny krol nie moze wykonac zadnego ruchu
    return not (isValidMove(wKing,wRook,moveL(bKing)) or
                isValidMove(wKing,wRook,moveR(bKing)) or
                isValidMove(wKing,wRook,moveD(bKing)) or
                isValidMove(wKing,wRook,moveU(bKing)) or
                isValidMove(wKing,wRook,moveUL(bKing)) or
                isValidMove(wKing,wRook,moveUR(bKing)) or
                isValidMove(wKing,wRook,moveDR(bKing)) or
                isValidMove(wKing,wRook,moveDL(bKing)))

def isWin(wKing,wRook,bKing):#krol szachowany i nie moze zrobic ruchu
    return isCheck(wKing,wRook,bKing) and isPat(wKing,wRook,bKing)

def setMoves(count):
    global moves
    moves = count

def round(turn,wKing,wRook,bKing,count,path):
    if not __debug__:
        path = deepcopy(path) #aby w kazdym wywolaniu funkcji nie bylo referencji do tego samego obiektu, tylko nowa lista
        path.append([wKing,wRook,bKing])
    if count > moves:
        return False #fail
    if isWin(wKing,wRook,bKing):
        setMoves(count)
        if not __debug__:
            # print(path)
            out.write(str(path)+'\n')
        return True #poniewaz w ValidMove sprawdzam czy jest pod wieza - wyklucza szach
    if not isValidMove(wKing,wRook,bKing):
        return False #niepoprawny ruch
    if turn == 'white' and isCheck(wKing,wRook,bKing):
        return False #czarny dobrowolnie wszedl pod wieze

    if turn == 'black' and isPat(wKing,wRook,bKing):
        return False

    if turn == 'black':
        return (round('white',wKing,wRook,moveL(bKing) ,count+1,path) or
                round('white',wKing,wRook,moveR(bKing) ,count+1,path) or
                round('white',wKing,wRook,moveD(bKing) ,count+1,path) or
                round('white',wKing,wRook,moveU(bKing) ,count+1,path) or
                round('white',wKing,wRook,moveDR(bKing),count+1,path) or
                round('white',wKing,wRook,moveDL(bKing),count+1,path) or
                round('white',wKing,wRook,moveUR(bKing),count+1,path) or
                round('white',wKing,wRook,moveUL(bKing),count+1,path))
    else: #white
        if (round('black',moveL(wKing) ,wRook,bKing,count+1,path) or
            round('black',moveR(wKing) ,wRook,bKing,count+1,path) or
            round('black',moveD(wKing) ,wRook,bKing,count+1,path) or
            round('black',moveU(wKing) ,wRook,bKing,count+1,path) or
            round('black',moveDR(wKing),wRook,bKing,count+1,path) or
            round('black',moveDL(wKing),wRook,bKing,count+1,path) or
            round('black',moveUR(wKing),wRook,bKing,count+1,path) or
            round('black',moveUL(wKing),wRook,bKing,count+1,path)):
            return True

        tmp = wRook
        for i in range(8):
            tmp = moveL(tmp)
            if tmp == bKing or tmp == wKing:
                break
            if round('black',wKing,tmp,bKing,count+1,path):
                return True
        tmp = wRook
        for i in range(8):
            tmp = moveD(tmp)
            if tmp == bKing or tmp == wKing:
                break
            if round('black',wKing,tmp,bKing,count+1,path):
                return True
        tmp = wRook
        for i in range(8):
            tmp = moveR(tmp)
            if tmp == bKing or tmp == wKing:
                break
            if round('black',wKing,tmp,bKing,count+1,path):
                return True
        tmp = wRook
        for i in range(8):
            tmp = moveU(tmp)
            if tmp == bKing or tmp == wKing:
                break
            if round('black',wKing,tmp,bKing,count+1,path):
                return True
    return False


with open('zad1_input.txt','r') as file:
    with open('zad1_output.txt','w') as out:
        for line in file:
            [turn,wKing,wRook,bKing] = line.split()
            found = False
            for i in range(50):
                moves = i+1
                if round(turn,wKing,wRook,bKing,0,[]):
                    found = True
                    # print('udalo sie w',moves,'ruchach!')
                    out.write(str(moves)+'\n')
                    break
            if not found:
                # print('INF')
                out.write('INF\n')

# black f2 h6 h1
# white f2 c6 h1
# white f3 c6 h2
# black c4 c8 h3
# white a1 b2 c3

# zwykly tryb:
# python3 zad1.py 
# tryb debug:
# python3 -O zad1.py 