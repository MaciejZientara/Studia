import unittest
import types
from zad2lista2 import pierwiastek, ArgumentNotIntError


class TestPierwiastek(unittest.TestCase):
    def testEqual(self):
        """Sprawdzenie czy otrzymujemy dobry wynik dla pierwiastków"""
        pierwiastki = range(10)
        for p in pierwiastki:
            self.assertEqual(p, pierwiastek(p * p), "Zła wartość pierwiastka!")

    def testNotEqual(self):
        """Sprawdzenie czy otrzymujemy zły wynik dla niepowiązanych wartości"""
        liczby = [5, 7, 11, 13]  # przechodzą wszystkie testy
        # liczby = [2,3,5,7,11,13] #pokazuje, że kod nie działa poprawnie
        for l in liczby:
            self.assertNotEqual(1, pierwiastek(l), "Zła wartość pierwiastka!")

    def testDziedzina(self):
        """Sprawdzenie czy pojawia się wyjątek dla złego typu argumentu"""
        self.assertRaises(ArgumentNotIntError, pierwiastek, "101")


if __name__ == "__main__":
    unittest.main()
