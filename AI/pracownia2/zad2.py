from collections import deque


arr = []

N = 0
M = 0

endPosition = []

gameStates = {}# slownik stan - (kroki,pozycja poczatkowa gracza dla tego stanu)

const0 = ord('0')
constA = ord('a')

# uzyje zapisu hex zeby interpretowac mozliwe ruchy danego pudelka
# h | UDLR
# 0 | 0000
# 1 | 0001
# 2 | 0010
# 3 | 0011
# 4 | 0100
# 5 | 0101
# 6 | 0110
# 7 | 0111
# 8 | 1111
# 9 | 1001
# a | 1010
# b | 1011
# c | 1100
# d | 1101
# e | 1110
# f | 1111

def isWin(state):
    return all([state[i:i+2] in endPosition for i in range(0,len(state),3)])

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

def move(position,dir):
    if dir == 'U':
        return hexToChar(charToHex(position[0])-1) + position[1]
    if dir == 'D':
        return hexToChar(charToHex(position[0])+1) + position[1]
    if dir == 'L':
        return position[0] + hexToChar(charToHex(position[1])-1)
    if dir == 'R':
        return position[0] + hexToChar(charToHex(position[1])+1)
    return '--'


def BFS(crates, player):#podaje liste z posortowanymi skrzyniami oraz pozycje gracza
    # zwraca liste ruchow skrzyn odpowiadajacej kolejnosci skrzyn podanych w argumencie
    clen = len(crates)
    moves = ['0' for i in range(clen)]

    visited = [[0 for m in range(M)] for n in range(N)]
    # print(visited)

    for c in range(len(crates)):
        i = charToHex(crates[c][0])
        j = charToHex(crates[c][1])
        arr[i][j] = 'B'#przedstaw aktualny stan na mapie

    (pi,pj) = (charToHex(player[0]),charToHex(player[1]))
    que = deque()
    que.append((pi,pj))
    visited[pi][pj] = ''
    while que:
        i,j = que.popleft()
        if (arr[i][j]=='W') or (arr[i][j]=='B') or (arr[i][j]=='*'):
            continue
        # visited[i][j] = 1
        if visited[i-1][j] == 0:
            que.append((i-1,j))
            if arr[i-1][j]!='W' and arr[i-1][j]!='B':
                visited[i-1][j] = visited[i][j] + 'U'
        if visited[i+1][j] == 0:
            que.append((i+1,j))
            if arr[i+1][j]!='W' and arr[i+1][j]!='B':
                visited[i+1][j] = visited[i][j] + 'D'
        if visited[i][j-1] == 0:
            que.append((i,j-1))
            if arr[i][j-1]!='W' and arr[i][j-1]!='B':
                visited[i][j-1] = visited[i][j] + 'L'
        if visited[i][j+1] == 0:
            que.append((i,j+1))
            if arr[i][j+1]!='W' and arr[i][j+1]!='B':
                visited[i][j+1] = visited[i][j] + 'R'

    for c in range(len(crates)):
        i = charToHex(crates[c][0])
        j = charToHex(crates[c][1])
        arr[i][j]='.'#zresetuj stan
        [(ui,uj),(di,dj),(li,lj),(ri,rj)] = [(i-1,j),(i+1,j),(i,j-1),(i,j+1)]
        
        mv = 0
        if visited[ui][uj] != 0 and ((arr[di][dj]=='.') or (arr[di][dj]=='G')):
            mv += 4 # ruch D
        if visited[di][dj] != 0 and ((arr[ui][uj]=='.') or (arr[ui][uj]=='G')):
            mv += 8 # ruch U
        if visited[ri][rj] != 0 and ((arr[li][lj]=='.') or (arr[li][lj]=='G')):
            mv += 2 # ruch L
        if visited[li][lj] != 0 and ((arr[ri][rj]=='.') or (arr[ri][rj]=='G')):
            mv += 1 # ruch R
        # print('in BFS mv =',mv)
        moves[c] = hexToChar(mv)

    return moves,visited

def makeState(crates,moves):
    result = ''
    for x,y in zip(crates,moves):
        result += (x+y)
    return result


def makeFirstState():
    global N
    N = len(arr)
    global M
    M = len(arr[0])
    playerPosition = '--'

    crates = []
    for i in range(N):
        for j in range(M):
            if arr[i][j] == 'K' or arr[i][j] == '+':
                playerPosition = hexToChar(i)+hexToChar(j)
            if arr[i][j] == 'G' or arr[i][j] == '*':
                endPosition.append(hexToChar(i)+hexToChar(j))
            if arr[i][j] == 'B' or arr[i][j] == '*':
                crates.append(hexToChar(i)+hexToChar(j))
            if arr[i][j] != 'W':
                arr[i][j] = '.'
    crates.sort()
    moves = (BFS(crates,playerPosition))[0]
    return makeState(crates,moves),playerPosition

def isValid(crates,moves):#tylko sprawdzam czy nie ma skrzynek na sobie
    if not isWin and ('0' in moves):
        return False #                                                       dead states
    return all([crates[i] not in crates[i+1:] for i in range(len(crates))])

def sokoban():
    stateQue = deque()
    firstState,plPos = makeFirstState()
    stateQue.append(firstState)
    gameStates[firstState] = ('',plPos)

    reultPath = 0
    while stateQue: #not empty
        currentState = stateQue.popleft()

        curLen = len(currentState)
        crates = ['--' for i in range(int(curLen/3))]
        moves = ['-' for i in range(int(curLen/3))]
        for i in range(int(curLen/3)):
            crates[i] = currentState[3*i:3*i+2]
            moves[i] = currentState[3*i+2]

        if isWin(currentState):
            tmpPath = gameStates[currentState][0]
            if (reultPath == 0) or (len(reultPath) > len(tmpPath)):
                reultPath = tmpPath

        for m in range(len(moves)):
            visited = (BFS(crates,gameStates[currentState][1]))[1]
            moveMap = {0b1000 : 'U', 0b100 : 'D', 0b10 : 'L', 0b1 : 'R'}
            reverseMove = {'U' : 'D', 'D' : 'U', 'L' : 'R', 'R' : 'L'}
            for mM in moveMap:
                oldCratePosition = crates[m]
                if (charToHex(moves[m]) & mM) > 0:
                    # zamienic pozycje w crates, posortowac, bfs i przywrocic pozycje w crates
                    crates[m] = move(crates[m],moveMap[mM])
                    sortCrate = sorted(crates)
                    newMoves = (BFS(sortCrate,oldCratePosition))[0]
                    newState = makeState(sortCrate,newMoves)
                    if isValid(sortCrate,newMoves):
                        posBeforePush = move(oldCratePosition,reverseMove[moveMap[mM]])
                        path = visited[charToHex(posBeforePush[0])][charToHex(posBeforePush[1])]
                        if path == 0:
                            path = ''
                        steps = (gameStates[currentState][0] + path + moveMap[mM], oldCratePosition)
                        isIn = newState in gameStates
                        if (not isIn) or (isIn and (len(steps[0]) < len(gameStates[newState][0]))):
                            gameStates[newState] = steps
                            stateQue.append(newState)
                    crates[m] = oldCratePosition



    print(reultPath)
    out.write(str(reultPath)+'\n')

with open('zad_output.txt','w') as out:
    with open('zad_input.txt','r') as file:
        for line in file:
            arr.append([])
            for l in line:
                if l != '\n':
                    arr[-1].append(l)
        sokoban()
