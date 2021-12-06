from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.sql import func
from sqlalchemy import Table, Column, Integer, ForeignKey, String, DateTime, create_engine, select
from sqlalchemy.orm import sessionmaker, validates
import sys

#   ================================================
#
#   na dole kodu znajduja sie przykladowe polecenia do przetestowania programu
#
#   ================================================

Base = declarative_base()

engine = create_engine("sqlite:///zad1.db", echo=True)

class Osoba(Base):
    __tablename__ = 'Osoba'
    id = Column(Integer, primary_key=True, autoincrement=True)
    imie = Column(String(20), nullable=False)
    nazwisko = Column(String(30), nullable=False)
    email = Column(String(50), nullable=False)
    
    @validates('email')
    def validate_email(self, key, value):
        assert '@' in value
        return value

class Ksiazka(Base):
    __tablename__ = 'Ksiazka'
    id = Column(Integer, primary_key=True)
    nazwa = Column(String(30), nullable=False)
    autor = Column(String(50), nullable=False)
    rok = Column(Integer, nullable=False)

class Wypozyczenie(Base):
    __tablename__ = 'Wypozyczenie'
    id = Column(Integer, primary_key=True)
    id_ksiazka = Column(Integer,ForeignKey("Ksiazka.id"))
    id_osoba = Column(Integer,ForeignKey("Osoba.id"))
    data_wypozyczenia = Column(DateTime(timezone=True), server_default=func.now())
    data_oddania = Column(DateTime(timezone=True), nullable=True)


Base.metadata.create_all(engine)

Session = sessionmaker(bind=engine)


def dodaj_osobe():
    if len(sys.argv) < 6:
        print('prosze podac', end = ' ')
    if len(sys.argv) < 4:
        print('Osoba.imie', end = ' ')
        return
    if len(sys.argv) < 5:
        print('Osoba.nazwisko', end = ' ')
        return
    if len(sys.argv) < 6:
        print('Osoba.email', end = ' ')
        return
    sesja = Session()
    o = Osoba(imie = sys.argv[3], nazwisko = sys.argv[4], email = sys.argv[5])
    sesja.add(o)
    sesja.commit()
    sesja.close()

def dodaj_ksiazke():
    if len(sys.argv) < 6:
        print('prosze podac', end = ' ')
    if len(sys.argv) < 4:
        print('Ksiazka.nazwa', end = ' ')
        return
    if len(sys.argv) < 5:
        print('Ksiazka.autor', end = ' ')
        return
    if len(sys.argv) < 6:
        print('Ksiazka.rok', end = ' ')
        return
    sesja = Session()
    k = Ksiazka(nazwa = sys.argv[3], autor = sys.argv[4], rok = int(sys.argv[5]))
    sesja.add(k)
    sesja.commit()
    sesja.close()

def usun_osobe():
    if len(sys.argv) < 6:
        print('prosze podac', end = ' ')
    if len(sys.argv) < 4:
        print('Osoba.imie', end = ' ')
        return
    if len(sys.argv) < 5:
        print('Osoba.nazwisko', end = ' ')
        return
    if len(sys.argv) < 6:
        print('Osoba.email', end = ' ')
        return
    sesja = Session()
    o = Osoba(imie = sys.argv[3], nazwisko = sys.argv[4], email = sys.argv[5])
    sesja.delete(o)
    sesja.commit()
    sesja.close()

def usun_ksiazke():
    if len(sys.argv) < 6:
        print('prosze podac', end = ' ')
    if len(sys.argv) < 4:
        print('Ksiazka.nazwa', end = ' ')
        return
    if len(sys.argv) < 5:
        print('Ksiazka.autor', end = ' ')
        return
    if len(sys.argv) < 6:
        print('Ksiazka.rok', end = ' ')
        return
    sesja = Session()
    k = Ksiazka(nazwa = sys.argv[3], autor = sys.argv[4], rok = int(sys.argv[5]))
    sesja.delete(k)
    sesja.commit()
    sesja.close()

def usun_wypozyczenie():
    if len(sys.argv) < 9:
        print('prosze podac', end = ' ')
    if len(sys.argv) < 4:
        print('Osoba.imie', end = ' ')
        return
    if len(sys.argv) < 5:
        print('Osoba.nazwisko', end = ' ')
        return
    if len(sys.argv) < 6:
        print('Osoba.email', end = ' ')
        return
    if len(sys.argv) < 7:
        print('Ksiazka.nazwa', end = ' ')
        return
    if len(sys.argv) < 8:
        print('Ksiazka.autor', end = ' ')
        return
    if len(sys.argv) < 9:
        print('Ksiazka.rok', end = ' ')
        return
    sesja = Session()
    id_ksiazki = (sesja.query(Ksiazka).filter(Ksiazka.nazwa == sys.argv[6] and Ksiazka.autor == sys.argv[7] and Ksiazka.rok == int(sys.argv[8])).all())[0].id
    id_osoby = (sesja.query(Osoba).filter(Osoba.imie == sys.argv[3] and Osoba.nazwisko == sys.argv[4] and Osoba.email == sys.argv[5]).all())[0].id
    w = (sesja.query(Wypozyczenie).filter(Wypozyczenie.id_ksiazka == id_ksiazki and Wypozyczenie.id_osoba == id_ksiazki))[0]    
    sesja.delete(w)
    sesja.commit()
    sesja.close()

def wypozyczenie_ksiazki():
    if len(sys.argv) < 9:
        print('prosze podac', end = ' ')
    if len(sys.argv) < 4:
        print('Osoba.imie', end = ' ')
        return
    if len(sys.argv) < 5:
        print('Osoba.nazwisko', end = ' ')
        return
    if len(sys.argv) < 6:
        print('Osoba.email', end = ' ')
        return
    if len(sys.argv) < 7:
        print('Ksiazka.nazwa', end = ' ')
        return
    if len(sys.argv) < 8:
        print('Ksiazka.autor', end = ' ')
        return
    if len(sys.argv) < 9:
        print('Ksiazka.rok', end = ' ')
        return
    sesja = Session()
    id_ksiazki = sesja.query(Ksiazka).filter(Ksiazka.nazwa.like(sys.argv[6]), Ksiazka.autor.like(sys.argv[7]), Ksiazka.rok.like(sys.argv[8])).all()
    id_osoby = sesja.query(Osoba).filter(Osoba.imie.like(sys.argv[3]), Osoba.nazwisko.like(sys.argv[4]), Osoba.email.like(sys.argv[5])).all()
    # id_ksiazki = sesja.query(Ksiazka).filter(Ksiazka.nazwa == sys.argv[6] and Ksiazka.autor == sys.argv[7] and Ksiazka.rok == int(sys.argv[8])).all()
    # id_osoby = sesja.query(Osoba).filter(Osoba.imie == sys.argv[3] and Osoba.nazwisko == sys.argv[4] and Osoba.email == sys.argv[5]).all()
    w = Wypozyczenie(id_ksiazka = id_ksiazki[0].id, id_osoba = id_osoby[0].id)
    sesja.add(w)
    sesja.commit()
    sesja.close()

def zwrot_ksiazki():
    if len(sys.argv) < 9:
        print('prosze podac', end = ' ')
    if len(sys.argv) < 4:
        print('Osoba.imie', end = ' ')
        return
    if len(sys.argv) < 5:
        print('Osoba.nazwisko', end = ' ')
        return
    if len(sys.argv) < 6:
        print('Osoba.email', end = ' ')
        return
    if len(sys.argv) < 7:
        print('Ksiazka.nazwa', end = ' ')
        return
    if len(sys.argv) < 8:
        print('Ksiazka.autor', end = ' ')
        return
    if len(sys.argv) < 9:
        print('Ksiazka.rok', end = ' ')
        return
    sesja = Session()
    id_ksiazki = (sesja.query(Ksiazka).filter(Ksiazka.nazwa == sys.argv[6] and Ksiazka.autor == sys.argv[7] and Ksiazka.rok == int(sys.argv[8])).all())[0].id
    id_osoby = (sesja.query(Osoba).filter(Osoba.imie == sys.argv[3] and Osoba.nazwisko == sys.argv[4] and Osoba.email == sys.argv[5]).all())[0].id
    w = (sesja.query(Wypozyczenie).filter(Wypozyczenie.id_ksiazka == id_ksiazki and Wypozyczenie.id_osoba == id_ksiazki))[-1]#dodaj do ostatniego pasujacego wypozyczenia
    w.data_oddania = func.now()
    sesja.commit()
    sesja.close()

def result_dict(r):
    return dict(zip(r.keys(), r))

def result_dicts(rs): 
    return list(map(result_dict, rs))

def printAll():
    sesja = Session()
    print('Osoby:')
    for u in sesja.query(Osoba).all():
        print(u.__dict__)
    print('\n')
    print('Ksiazki:')
    for u in sesja.query(Ksiazka).all():
        print(u.__dict__)
    print('\n')
    print('Wypozyczenia:')
    for u in sesja.query(Wypozyczenie).all():
        print(u.__dict__)
    print('\n')

    sesja.close()


if len(sys.argv) == 1:
    print("za malo argumentow")
else:
    if sys.argv[1] == 'wypisz':
        printAll()
    if sys.argv[1] == 'dodaj':
        if sys.argv[2] == 'ksiazka':
            dodaj_ksiazke()
        if sys.argv[2] == 'osoba':
            dodaj_osobe()
        if sys.argv[2] == 'wypozyczenie':
            wypozyczenie_ksiazki()
        if sys.argv[2] == 'zwrot':
            zwrot_ksiazki()
    if sys.argv[1] == 'usun':
        if sys.argv[2] == 'ksiazka':
            usun_ksiazke()
        if sys.argv[2] == 'osoba':
            usun_osobe()
        if sys.argv[2] == 'wypozyczenie':
            usun_wypozyczenie()


# python3 zad1.py dodaj ksiazka hp jkr 2000
# python3 zad1.py dodaj osoba Jan Kowalski JK@gmail.com
# python3 zad1.py dodaj wypozyczenie Jan Kowalski JK@gmail.com hp jkr 2000
# python3 zad1.py wypisz
# python3 zad1.py dodaj zwrot Jan Kowalski JK@gmail.com hp jkr 2000
# python3 zad1.py wypisz
