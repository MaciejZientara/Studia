class ArgumentNotStringError(Exception):
    """Wyjątek dla złego typu argumentu"""

    pass


def is_palindrom(s):
    """Sprawdza czy słowo podane jako argument jest palindromem

    Args:
        s (string): słowo do sprawdzenia

    Raises:
        ArgumentNotStringError: zły typ argumentu

    Returns:
        True/False w zależności czy argument s jest palindromem
    """

    if not isinstance(s, str):
        raise ArgumentNotStringError
    disallowed_characters = ".,_:; -!"
    text = s
    for character in disallowed_characters:
        text = text.replace(character, "")
    text = text.lower()
    textRev = text[::-1]
#	print(text, textRev)
    return text == textRev


if __name__ == "__main__":
    print(is_palindrom('kajak'))
    print(is_palindrom('tatar'))
    print(is_palindrom('Alfa,betateb.afla!'))
    print(is_palindrom("Kobyła ma mały bok."))
    print(is_palindrom("Eine güldne, gute Tugend: Lüge nie!"))
