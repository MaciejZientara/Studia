letters = []
DONE = False

def fillLetters(a):
	for c in a:
		isIn = False
		for p in letters:
			if c in p:
				isIn = True
		if not isIn:
			letters.append([c,0])

def incByKey(k):
	for p in letters:
		if k in p:
			p[1] = (p[1]+1)%10

def getByKey(k):
	for p in letters:
		if k in p:
			return p[1]

def isFine(a,b,c):
	if getByKey(a[0])==0 or getByKey(b[0])==0 or getByKey(c[0])==0:
		return False
	if getByKey(a[0]) + getByKey(b[0]) > 9:
		return False
	for i in range(1,len(letters)):
		for j in range(i):
			if letters[i][1] == letters[j][1]:
				return False
	l = len(a)	
	if (getByKey(a[l-1])+getByKey(b[l-1]))%10!=getByKey(c[l-1]):
		return False
	for i in range(l-1):
		if (getByKey(a[i]) + getByKey(b[i]) + int( (getByKey(a[i+1]) + getByKey(b[i+1])) / 10) ) % 10 != getByKey(c[i]):
			return False
	return True
		
def recFunc(a,b,c,index):
	global DONE
	if index == len(letters):
		return
	for i in range(10):
		if DONE:
			return
		letters[index][1] = (letters[index][1]+1)%10
		if isFine(a,b,c):
			DONE = True
			return
		recFunc(a,b,c,index+1)
		

def func(a,b,c):
	fillLetters(a)
	fillLetters(b)
	fillLetters(c)

	recFunc(a,b,c,0)

	return letters

def textToNumber(a):
	res = ""
	for c in a:
		res+=str(getByKey(c))
	print(a,res)


#A = 'BDCCB'#13221
#B = 'GHFEF'#67545
#C = 'IAHGG'#80766
#A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9


A = 'KIOTO'
B = 'OSAKA'
C = 'TOKIO'

print(func(A,B,C))

textToNumber(A)
textToNumber(B)
textToNumber(C)

# A + B = C
