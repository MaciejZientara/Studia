import unittest
import types
from zad2lista1 import is_palindrom, ArgumentNotStringError


class TestIsPalindrom(unittest.TestCase):
    def testEqual(self):
        """Sprawdzenie czy otrzymujemy dobry wynik dla palindromów"""
        palindromy = ["kajak", "Kobyła ma mały bok.",
                      "Eine güldne, gute Tugend: Lüge nie!"]
        for p in palindromy:
            self.assertEqual(True, is_palindrom(p), "Nie wykrył palindromu!")

    def testNotEqual(self):
        """Sprawdzenie czy otrzymujemy zły wynik dla nie-palindromów"""
        niepalindromy = ["tatar", "mama", "słowo"]
        for p in niepalindromy:
            self.assertNotEqual(
                True,
                is_palindrom(p),
                "Wykrył palindrom tam gdzie go nie ma!")

    def testDziedzina(self):
        """Sprawdzenie czy pojawia się wyjątek dla złego typu argumentu"""
        self.assertRaises(ArgumentNotStringError, is_palindrom, 101)


if __name__ == "__main__":
    unittest.main()
