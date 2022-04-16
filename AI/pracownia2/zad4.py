from collections import deque
from random import choice
from random import random
from random import randrange

arr = []
N = 0
M = 0
endPosition = set()

gameStates = {}

const0 = ord('0')
constA = ord('a')
def charToHex(x):
    if x <= '9':
        return ord(x)-const0
    else:
        return ord(x)-constA+10
def hexToChar(x):
    if x <= 9:
        return chr(const0 + x)
    else:
        return chr(constA + x - 10)

def isWin(state):
    positions = [state[i:i+2] for i in range(0,len(state),2)]
    for p in positions:
        if p not in endPosition:
            return False
    return True

def makeState(positions):
    positions.sort()
    res = positions[0]
    for i in range(1,len(positions)):
        if positions[i-1]!=positions[i]:#usuwam duplikaty
            res += positions[i]
    return res

def makeFirstState():
    global N
    N = len(arr)
    global M
    M = len(arr[0])

    startPos = []
    for i in range(N):
        for j in range(M):
            if arr[i][j] == 'S' or arr[i][j] == 'B':
                startPos.append(hexToChar(i)+hexToChar(j))
            if arr[i][j] == 'G' or arr[i][j] == 'B':
                endPosition.add(hexToChar(i)+hexToChar(j))
            if arr[i][j] != '#':
                arr[i][j] = ' '
    return makeState(startPos)

def moveHelper(pos,dir):
    position = pos
    i,j = charToHex(position[0]),charToHex(position[1])
    if dir == 'U':
        i-=1
    if dir == 'D':
        i+=1
    if dir == 'L':
        j-=1
    if dir == 'R':
        j+=1
    if arr[i][j]=='#':#wejscie w sciane = nie wykonanie ruchu
        return pos
    else:
        return hexToChar(i)+hexToChar(j)

def move(pos,dir):
    for d in dir:
        pos = moveHelper(pos,d)
    return pos

def debugArrPrint(state):
    positions = [state[i:i+2] for i in range(0,len(state),2)]
    for i in range(N):
        for j in range(M):
            tmp = arr[i][j]
            pos = (hexToChar(i)+hexToChar(j))
            isInPos = pos in positions
            isInEnd = pos in endPosition
            if isInPos:
                tmp = 'S'
            if isInEnd:
                tmp = 'G'
            if isInPos and isInEnd:
                tmp = 'B'
            print(tmp,end='')
        print()

def moveState(state,dir):
    positions = [state[i:i+2] for i in range(0,len(state),2)]
    return makeState([move(p,dir) for p in positions])

def combinePositions(firstState):
    desiredDifferentPositions = 2
    maxPathLength = 80
    state = firstState
    path = ''

    print('\n\n')
    print('combS',state,len(state)/2,path,len(path))
    # debugArrPrint(state)

    # bede wykonwal losowe, zachlanne ruchy
    # jesli nie zmniejsza ilosci roznych pozycji to powtarzam losowanie,
    # jest mala szansa, ze nawet kiedy nie zmniejszy sie ilosc dalej wykonam ruch
    moveTable = ['U','D','L','R']#,'UU','DD','LL','RR','UUU','DDD','LLL','RRR','UUUU','DDDD','LLLL','RRRR']#,'UUUUU','DDDDD','LLLLL','RRRRR']
    while True:
        while (len(state) > (desiredDifferentPositions*2)) and (len(path) < maxPathLength):
            m = (choice(moveTable)) * randrange(1,5) #kierunek * ilosc krokow
            newState = moveState(state,m)
            newPath = path+m
            if (len(newState) < len(state)) or (random() < 0.2):
                state = newState
                path = newPath
        if (len(state) <= (desiredDifferentPositions*2)):
            break
        print('restart',state,len(state)/2,path,len(path))


    print('combE',state,len(state)/2,path,len(path))
    debugArrPrint(state)
    return state,path

def BFS(firstState):
    que = deque()
    que.append(firstState)
    gameStates[firstState] = ''
    moveTable = ['U','D','L','R']
    while que:
        state = que.popleft()
        path = gameStates[state]
        if isWin(state):
            break
        positions = [state[i:i+2] for i in range(0,len(state),2)]
        for m in moveTable:
            newPositions = [move(p,m) for p in positions]
            newState = makeState(newPositions)
            newPath = path+m
            isIn = newState in gameStates
            if not isIn:# or (isIn and (len(gameStates[newState]) > len(newPath))):
                gameStates[newState] = newPath
                que.append(newState)
    return path

def komandos():
    firstState = makeFirstState()
    combinedStete,combinedPath = combinePositions(firstState)
    resultPath = BFS(combinedStete)
    print(combinedPath+resultPath)
    out.write(str(combinedPath)+str(resultPath)+'\n')

with open('zad_output.txt','w') as out:
    with open('zad_input.txt','r') as file:
        for line in file:
            arr.append([])
            for l in line:
                if l != '\n':
                    arr[-1].append(l)
        komandos()
