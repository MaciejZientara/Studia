def pierwiastek(n):
	i = 1
	suma = 0
	while suma+(2*i-1) <= n:
		suma+=(2*i-1)
		i+=1
	return i-1

n = input()
print(pierwiastek(int(n)))
