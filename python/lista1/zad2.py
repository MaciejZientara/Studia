def is_palindrom(s):
	disallowed_characters = ".,_:; -!"
	text = s
	for character in disallowed_characters:
		text = text.replace(character, "")
	text = text.lower()
	textRev = text [::-1]
#	print(text, textRev)
	return text == textRev


print(is_palindrom('kajak'))
print(is_palindrom('tatar'))
print(is_palindrom('Alfa,betateb.afla!'))
print(is_palindrom("Kobyła ma mały bok."))
print(is_palindrom("Eine güldne, gute Tugend: Lüge nie!"))
