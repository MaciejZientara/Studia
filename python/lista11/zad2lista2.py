class ArgumentNotIntError(Exception):
    """Wyjątek dla złego typu argumentu"""

    pass


def pierwiastek(n):
    """Oblicza przybliżenie pierwiastka argumentu

    Args:
        n (int): liczba naturalna

    Raises:
        ArgumentNotIntError: zły typ argumentu

    Returns:
        przybliżenie sqrt(n) (int)
    """

    if not isinstance(n, int):
        raise ArgumentNotIntError
    i = 1
    suma = 0
    while suma + (2 * i - 1) <= n:
        suma += (2 * i - 1)
        i += 1
    return i - 1


if __name__ == "__main__":
    n = input()
    print(pierwiastek(int(n)))
