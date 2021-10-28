import timeit
from math import sqrt

testImp = '''
def pierwsze_imperatywna(n):
	ret = []
	for i in range(2,n):
		prime = True
		for tmp in range(2,int(sqrt(i)+1)):
			if i % tmp == 0:
				prime = False
		if prime:
			ret.append(i)
	return ret
'''

testSkl = '''
def pierwsze_skladana(n):
	return [x for x in range(2,n) if len([y for y in range(2,int(sqrt(x)+1)) if x%y==0])==0]
'''

testFun = '''
def pierwsze_funkcyjna(n):
	return list(filter(lambda x: len([y for y in range(2,int(sqrt(x)+1)) if x%y==0])==0, range(2,n)))
'''

def pierwsze_imperatywna(n):
	ret = []
	for i in range(2,n):
		prime = True
		for tmp in range(2,int(sqrt(i)+1)):
			if i % tmp == 0:
				prime = False
		if prime:
			ret.append(i)
	return ret

def pierwsze_skladana(n):
	return [x for x in range(2,n) if len([y for y in range(2,int(sqrt(x)+1)) if x%y==0])==0]


def pierwsze_funkcyjna(n):
	return list(filter(lambda x: len([y for y in range(2,int(sqrt(x)+1)) if x%y==0])==0, range(2,n)))

n = 100

print(pierwsze_imperatywna(n))
print(pierwsze_skladana(n))
print(pierwsze_funkcyjna(n))


print(timeit.timeit(testImp, number=100))
print(timeit.timeit(testSkl, number=100))
print(timeit.timeit(testFun, number=100))
