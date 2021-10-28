def kompresja(tekst):
	ret = []
	for c in tekst:
		if len(ret) == 0:
			ret.append((c,1))
		else:
			tmp = len(ret) - 1
			(a,b) = ret[tmp]
			if c == a:
				ret[tmp] = (a,b+1)
			else:
				ret.append((c,1))
	return ret

def dekompresja(compr):
	ret = ""
#	for pair in compr:
#		ret += pair[0] * pair[1]
	for (a,b) in compr:
		ret += a * b
	return ret

x = input()
compr = kompresja(x)
print(compr)
print(dekompresja(compr))

# https://www.randomtextgenerator.com/
# run: python3 zad5.py < text_file

