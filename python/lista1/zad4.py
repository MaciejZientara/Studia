import sys
import random as r

def experiment():
	flips = 0
	last = -1 #0 - reszka, 1 - orzel
	inRow = 0
	while inRow!=3:
		flips += 1
		tmp = r.randint(0,1)
		if last == tmp:
			inRow += 1
		else:
			inRow = 1
			last = tmp
	return flips

lst = [experiment() for i in range(int(sys.argv[1]))]
print(sum(lst)/len(lst))
