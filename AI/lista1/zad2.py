# comment

from copy import deepcopy

words = []
lenWords = 0

mxSum = -1
storedRes = []

def save(sum,res):
    global mxSum
    global storedRes
    mxSum = sum
    storedRes = deepcopy(res)
    # print(storedRes)

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
    # if binSearch(0,lenWords,t) != (t in words):
    #     print(binSearch(0,lenWords,t), t in words, t)
    return binSearch(0,lenWords,t)
    # return t in words

def splitWords(line,sum,res):
    # print(line, res)
    if not line:
        if mxSum < sum:
            save(sum,res)
            return
    for i in range(len(line)):
        tmp = line[0:i+1]
        if isInWords(tmp):
            splitWords(line[i+1:],sum+(len(tmp))**2,res + [tmp])


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
            # print(line)
            splitWords(line,0,[])
            for s in storedRes:
                out.write(str(s)+' ')
                # print(s, end=' ')
            out.write('\n')
            # print()#new line
            mxSum = -1
            storedRes = []

# alfaTab = ['a','ojczyzno','albańczykiem']
# for alfa in alfaTab:
#     print(binSearch(0,lenWords,alfa), alfa in words, alfa)

# for w in words:
#     if binSearch(0,lenWords,w) != (w in words):
#         print(binSearch(0,lenWords,w), w in words, w)