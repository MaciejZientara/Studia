import decimal as d

def vat_faktura(lista):
	return 0.23 * sum(lista)


def vat_paragon(lista):
	return sum(0.23 * y for y in lista)

def vat_fakturaDec(lista):
	return d.Decimal(0.23) * sum(lista)


def vat_paragonDec(lista):
	return sum(d.Decimal(0.23) * y for y in lista)

lista = [1,2,3,4,5,10,15,20,25]

# print(lista)
# print("faktura: ", vat_faktura(lista))
# print("paragon: ", vat_paragon(lista))

zakupy = [0.2, 0.5, 4.59, 6]

print(vat_faktura(zakupy))
print(vat_paragon(zakupy))
print(vat_faktura(zakupy) == vat_paragon(zakupy))

zakupyDec = [d.Decimal(0.2), d.Decimal(0.5), d.Decimal(4.59), d.Decimal(6)]

print(vat_fakturaDec(zakupyDec))
print(vat_paragonDec(zakupyDec))
print(vat_fakturaDec(zakupyDec) == vat_paragonDec(zakupyDec))
