# używając programowania dynamicznego sprawdzam wszystkie kombinacje podziałów
# lini tekstu, dp[i] zawiera -1 jesli nie znalazlem podziału do i-tej litery, 
# lub maksymalną sumę kwadratów długości słów znalezionych do i-tej litery

words = []
lenWords = 0

def binSearch(b,e,x):
    # print(b,int((b+e)/2),e,x,'\t',words[int((b+e)/2)-1],words[int((b+e)/2)],words[int((b+e)/2)+1])
    if e > b:
        mid = int((b+e)/2) #int div 2
        if words[mid] == x:
            return True
        elif words[mid] > x:
            return binSearch(b,mid,x)
        else:
            return binSearch(mid+1,e,x) 
    else:
        return False

def isInWords(t):
    return binSearch(0,lenWords,t)

def splitWords(line):
    lineLen = len(line)
    dp = [-1 for i in range(lineLen+1)]
    dp[0] = 0
    res = [[] for i in range(lineLen+1)]
    changed = True
    while changed:
        # print(res)
        changed = False
        for i in range(lineLen+1)[::-1]:
            if dp[i] != -1:
                j = 0
                while i+j+1 <= lineLen:
                    tmp = line[i:i+1+j]
                    if isInWords(tmp):
                        ln = len(tmp)
                        val = (ln*ln) + dp[i]
                        if dp[i+1+j] < val:
                            dp[i+1+j] = val
                            res[i+1+j] = res[i] + [tmp]
                            changed = True
                    j+=1
    return res[lineLen]

with open('polish_words.txt','r') as wordsFile:
    for w in wordsFile:
        if w[-1] == '\n':
            w = w[:-1]
        if w:
            words.append(w)
lenWords = len(words)
words.sort()

with open('zad2_output.txt','w') as out:
    with open('zad2_input.txt','r') as file:
        for line in file:
            if line[-1] == '\n':
                line = line[:-1]
            storedRes = splitWords(line)
            for s in storedRes:
                out.write(str(s)+' ')
                # print(s, end=' ')
            out.write('\n')
            # print()#new line
            mxSum = -1
            storedRes = []
