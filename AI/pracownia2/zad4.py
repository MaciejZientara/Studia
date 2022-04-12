from collections import deque


arr = []
N = 0
M = 0
endPosition = []

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
    return all([state[i:i+2] in endPosition for i in range(0,len(state),2)])

def makeState(positions):
    sorted(positions)
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
                endPosition.append(hexToChar(i)+hexToChar(j))
            if arr[i][j] != '#':
                arr[i][j] = ' '
    return makeState(startPos)

def move(pos,dir):
    position = pos
    if dir == 'U':
        position = hexToChar(charToHex(position[0])-1) + position[1]
    if dir == 'D':
        position = hexToChar(charToHex(position[0])+1) + position[1]
    if dir == 'L':
        position = position[0] + hexToChar(charToHex(position[1])-1)
    if dir == 'R':
        position = position[0] + hexToChar(charToHex(position[1])+1)
    if arr[charToHex(position[0])][charToHex(position[1])] == '#':#wejscie w sciane = nie wykonanie ruchu
        return pos
    else:
        return position

def combinePositions(firstState):
    accaptableDifferentPositions = 3
    state = ''
    path = ''
    # w kolejce przechowuje pary (stan,path)
    que = deque()
    que.append((firstState,''))
    moveTable = ['U','D','L','R']
    while que:
        state,path = que.popleft()
        if (len(state)/2) <= accaptableDifferentPositions:
            break
        positions = [state[i:i+2] for i in range(0,len(state),2)]
        for m in moveTable:
            newPositions = []
            for p in positions:
                newPositions.append(move(p,m))
            que.append((makeState(newPositions),(path+m)))
    return state,path

#zakladam, ze state ma tylko kilka roznych pozycji (accaptableDifferentPositions w combinePositions)
def BFS(firstState):
    path = ''
    # w kolejce przechowuje pary (stan,path)
    que = deque()
    que.append((firstState,''))
    moveTable = ['U','D','L','R']
    while que:
        state,path = que.popleft()
        if isWin(state):
            break
        positions = [state[i:i+2] for i in range(0,len(state),2)]
        for m in moveTable:
            newPositions = []
            for p in positions:
                newPositions.append(move(p,m))
            que.append((makeState(newPositions),(path+m)))

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
