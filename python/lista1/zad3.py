def tabliczka(x1, x2, y1, y2):
	result = []
	for i in range(x2-x1+2):
		result.append(["" for j in range(y2-y1+2)])
	
	for i in range(1,x2-x1+2):
		result[0][i] = x1-1+i

	for j in range(1,y2-y1+2):
		result[j][0] = y1-1+j

	for j in range(1,y2-y1+2):
		for i in range(1,x2-x1+2):
			result[j][i] = result[j][0] * result[0][i]
	
	offsets = [max([len(str(result[j][i])) for j in range(y2-y1+2)]) for i in range(x2-x1+2)]
#	print(offsets)
#	print(result)
	
	for j in range(y2-y1+2):
		res = ""
		for i in range(x2-x1+2):
			tmp = str(result[j][i])
			res += (" " * (1+offsets[i]-len(tmp)) + tmp)
		print(res)


tabliczka(3, 5, 2, 4)
tabliczka(5, 7, 13, 15)
