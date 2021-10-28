sudan_memo = []
def sudan(n, x, y):
	if n==0:
		return x+y
	if y == 0:# and x >=0:
		return x
	
	for pair in sudan_memo:
		if [n,x,y] == pair[:3]:
			return pair[3]
	tmp = sudan(n-1,sudan(n,x,y-1),sudan(n,x,y-1)+y+1)
	sudan_memo.append([n,x,y,tmp])
	return tmp

def sudan_brutal(n,x,y):
	if n==0:
		return x+y
	if y == 0:# and x >=0:
		return x
	return sudan(n-1,sudan(n,x,y-1),sudan(n,x,y-1)+y+1)

print(sudan(2,1,2))

#print(sudan_memo)

#max arguments without memo: 
# 1,1,25
# n=2 and y=2 is too much (long execution time)
#max arguments with memo: 2,5,2
# if either n or y = 3, runs out of recursion memory
