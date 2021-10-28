import timeit

testImp = '''
def doskonale_imperatywna(n):
	ret = []
	for i in range(1,n+1):
		sm = 0
		for tmp in range(1,i):
			if i%tmp == 0:
				sm += tmp
		if sm == i:
			ret.append(i)
	return ret
'''

testSkl = '''
def doskonale_skladana(n):
	return [x for x in range(1,n+1) if x==sum([y for y in range(1,x) if x%y==0])]
'''

testFun = '''
def doskonale_funkcyjna(n):
	return list(filter(lambda x: x==sum([y for y in range(1,x) if x%y==0]), range(2,n)))
'''

def doskonale_imperatywna(n):
	ret = []
	for i in range(1,n+1):
		sm = 0
		for tmp in range(1,i):
			if i%tmp == 0:
				sm += tmp
		if sm == i:
			ret.append(i)
	return ret

def doskonale_skladana(n):
	return [x for x in range(1,n+1) if x==sum([y for y in range(1,x) if x%y==0])]


def doskonale_funkcyjna(n):
	return list(filter(lambda x: x==sum([y for y in range(1,x) if x%y==0]), range(2,n)))

n = 10000

print(doskonale_imperatywna(n))
print(doskonale_skladana(n))
print(doskonale_funkcyjna(n))


print(timeit.timeit(testImp, number=100))
print(timeit.timeit(testSkl, number=100))
print(timeit.timeit(testFun, number=100))


