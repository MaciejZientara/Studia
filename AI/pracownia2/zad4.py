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
                endPosition.append(hexToChar(i)+hexToChar(j))
            if arr[i][j] != '#':
                arr[i][j] = ' '
    return makeState(startPos)

def move(pos,dir):
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

def debugArrPrint(state):
    positions = [state[i:i+2] for i in range(0,len(state),2)]
    for i in range(N):
        for j in range(M):
            tmp = arr[i][j]
            pos = (hexToChar(i)+hexToChar(j))
            isInPos = pos in positions
            isInEnd = pos in endPosition
            if isInPos and isInEnd:
                tmp = 'B'
            if isInPos:
                tmp = 'S'
            if isInEnd:
                tmp = 'G'
            print(tmp,end='')
        print()

def combinePositions(firstState):
    accaptableDifferentPositions = 3
    state = firstState
    path = ''
    moveTable = ['U','D','L','R']
    #zachlanne zmniejszanie ilosci pozycji pojedynczymi ruchami
    while (len(state)/2) > accaptableDifferentPositions:
        if len(path) > 70:
            break
        resPath = path
        positions = [state[i:i+2] for i in range(0,len(state),2)]
        for m in moveTable:
            newPositions = [move(p,m) for p in positions]
            tmpState = makeState(newPositions)
            if len(tmpState) < len(state):
                state = tmpState
                resPath+=m
        if path == resPath:#jeden ruch nie zmniejsza juz ilosci
            break
        path = resPath

    print('comb1',state,len(state)/2,path,len(path))
    debugArrPrint(state)
    #poruszanie sie po schemacie (zygzak)
    zigzag1 = ['D','D','R','R','D','D','L','L']
    for i in range(int(N/4)+1):
        for z in zigzag1:
            positions = [state[i:i+2] for i in range(0,len(state),2)]
            state = makeState([move(p,z) for p in positions])
            path+=z
    zigzag2 = ['U','U','R','R','D','D','R','R']
    for i in range(int(M/4)+1):
        for z in zigzag2:
            positions = [state[i:i+2] for i in range(0,len(state),2)]
            state = makeState([move(p,z) for p in positions])
            path+=z

    print('comb2',state,len(state)/2,path,len(path))
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
