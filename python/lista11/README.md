Zadania z poprzednich list to:
* zadanie 2 z listy 1
* zadanie 2 z listy 2

A) Wykorzystuję pyunit do testów jednostkowych

    >>> make

pliki z testami to:
* test21.py dla zadania 2 z listy 1
* test22.py dla zadania 2 z listy 2

B) Rozwiązanie w pliku Profile21.txt

Zawiera komendę wykrzystaną do wygenerowania pliku,
oraz ocenę/analizę pomiaru na dole.

C) Zamiast PEP8 checker wykorzystałem gotowy formater:

    >>> pip install --upgrade autopep8
    >>> make pep

D) Wykorzystuję PyDoc, a komentarze są napisane w stylu Google

komenda make wygeneruje pliki html z dokmentacją, natomiast make clean usunie je

    >>> make

    >>> make clean