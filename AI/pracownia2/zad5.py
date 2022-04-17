from collections import deque
import numpy as np
import queue

arr = []
distances = []
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
    global distances
    distances = [[2*N*M for j in range(M)] for i in range(N)]

    startPos = []
    for i in range(N):
        for j in range(M):
            if arr[i][j] == 'S' or arr[i][j] == 'B':
                startPos.append(hexToChar(i)+hexToChar(j))
            if arr[i][j] == 'G' or arr[i][j] == 'B':
                endPosition.add(hexToChar(i)+hexToChar(j))
                distances[i][j] = 0
            if arr[i][j] != '#':
                arr[i][j] = ' '

    for q in range(N*M):
        for i in range(1,N-1):
            for j in range(1,M-1):
                if arr[i][j] != '#':
                    distances[i][j] = min([distances[i][j],distances[i+1][j]+1,distances[i-1][j]+1,distances[i][j+1]+1,distances[i][j-1]+1])

    return makeState(startPos)

def maxDistance(state):
    positions = [state[i:i+2] for i in range(0,len(state),2)]
    return max([distances[charToHex(p[0])][charToHex(p[1])] for p in positions])

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

def AStar(firstState):
    # bfs gdzie zamiast zwyklego deque mamy priority queue
    # sortowanie po (max(min odlegosc kazdej z pozycji stanu od punktu koncowego) + dlugosc sciezki do tego stanu)
    que = queue.PriorityQueue()
    que.put((maxDistance(firstState),firstState))
    gameStates[firstState] = ''
    moveTable = ['U','D','L','R']
    while que:
        state = que.get()[1]
        path = gameStates[state]
        if isWin(state):
            break
        positions = [state[i:i+2] for i in range(0,len(state),2)]
        for m in moveTable:
            newPositions = [move(p,m) for p in positions]
            newState = makeState(newPositions)
            newPath = path+m
            isIn = newState in gameStates
            if not isIn or (isIn and (len(newPath) < len(gameStates[newState]))):
                gameStates[newState] = newPath
                que.put((maxDistance(newState)+len(newPath),newState))
    return path

def komandos():
    firstState = makeFirstState()
    resultPath = AStar(firstState)
    print(resultPath)
    out.write(str(resultPath)+'\n')

with open('zad_output.txt','w') as out:
    with open('zad_input.txt','r') as file:
        for line in file:
            arr.append([])
            for l in line:
                if l != '\n':
                    arr[-1].append(l)
        komandos()
