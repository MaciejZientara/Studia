from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.sql import func
from sqlalchemy import Table, Column, Integer, ForeignKey, String, DateTime, create_engine, select
from sqlalchemy.orm import sessionmaker, validates
import sys

Base = declarative_base()

engine = create_engine("sqlite:///zad1.db", echo=True)

class Osoba(Base):
    __tablename__ = 'Osoby'
    id = Column(Integer, primary_key=True, autoincrement=True)
    imie = Column(String(20), nullable=False)
    nazwisko = Column(String(30), nullable=False)
    email = Column(String(50), nullable=False)
    
    @validates('email')
    def validate_email(self, key, value):
        assert '@' in value
        return value

class Ksiazka(Base):
    __tablename__ = 'Ksiazki'
    id = Column(Integer, primary_key=True)
    nazwa = Column(String(30), nullable=False)
    autor = Column(String(50), nullable=False)
    rok = Column(Integer, nullable=False)

class Wypozyczenie(Base):
    __tablename__ = 'Wypozyczenia'
    id = Column(Integer, primary_key=True)
    id_ksiazka = Column(Integer,ForeignKey("Ksiazki.id"))
    id_osoba = Column(Integer,ForeignKey("Osoby.id"))
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
    id_ksiazki = (sesja.query(Ksiazki).filter(Ksiazka.nazwa == sys.argv[6] and Ksiazka.autor == sys.argv[7] and Ksiazka.rok == int(sys.argv[8])).all())[0].id
    id_osoby = (sesja.query(Osoby).filter(Osoba.imie == sys.argv[3] and Osoba.nazwisko == sys.argv[4] and Osoba.email == sys.argv[5]).all())[0].id
    w = (sesja.query(Wypozyczenia).filter(Wypozyczenia.id_ksiazka == id_ksiazki and Wypozyczenia.id_osoba == id_ksiazki))[0]    
    sesja.delete(w)
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
    id_ksiazki = (sesja.query(Ksiazki).filter(Ksiazka.nazwa == sys.argv[6] and Ksiazka.autor == sys.argv[7] and Ksiazka.rok == int(sys.argv[8])).all())[0].id
    id_osoby = (sesja.query(Osoby).filter(Osoba.imie == sys.argv[3] and Osoba.nazwisko == sys.argv[4] and Osoba.email == sys.argv[5]).all())[0].id
    w = Wypozyczenie(id_ksiazka,id_osoby)
    sesja.add(w)
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
    id_ksiazki = (sesja.query(Ksiazki).filter(Ksiazka.nazwa == sys.argv[6] and Ksiazka.autor == sys.argv[7] and Ksiazka.rok == int(sys.argv[8])).all())[0].id
    id_osoby = (sesja.query(Osoby).filter(Osoba.imie == sys.argv[3] and Osoba.nazwisko == sys.argv[4] and Osoba.email == sys.argv[5]).all())[0].id
    w = (sesja.query(Wypozyczenia).filter(Wypozyczenia.id_ksiazka == id_ksiazki and Wypozyczenia.id_osoba == id_ksiazki))[0]    
    w.data_oddania = func.now()
    sesja.close()

def result_dict(r):
    return dict(zip(r.keys(), r))

def result_dicts(rs): 
    return list(map(result_dict, rs))

def printAll():
    sesja = Session()
    print('Osoby:')
    stmt = select('*').select_from(Osoba)
    result = sesja.execute(stmt).all()
    print(result)
    # print(result_dicts(result))
    print('\n')
    print('Ksiazki:')
    stmt = select('*').select_from(Ksiazka)
    result = sesja.execute(stmt).fetchall()
    print(result_dicts(result))
    print('\n')
    print('Wypozyczenia:')
    stmt = select('*').select_from(Wypozyczenie)
    result = sesja.execute(stmt).fetchall()
    print(result_dicts(result))
    print('\n')

    sesja.close()


if len(sys.argv) == 1:
    print("za malo argumentow")
else:
    if sys.argv[1] == 'wypisz':
        printAll()
    # print("Argument List:", sys.argv)
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