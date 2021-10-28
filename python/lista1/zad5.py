def common_prefix(lista):
	lista = [l.lower() for l in lista]	
	lista.sort()
#	print(lista)
	m = len(max(lista, key=len))
	for i in range(m+1)[::-1]:
		#[f(x) if condition else g(x) for x in sequence]
		lista = [l[:-1] if len(l)>i else l for l in lista]
		for j in range(len(lista)-2):
		#	print(lista[j],lista[j+1],lista[j+2])
			if lista[j] == lista[j+1] and lista[j] == lista[j+2]:
				return lista[j]
	return ""

print(common_prefix(["Cyprian", "cyberotoman", "cynik", "ceniąc", "czule"]))

print(common_prefix(["alfa","alfabetagammaAAA","alfagamma","alfabetagammaB","alfa","alfabetagammaC"]))
