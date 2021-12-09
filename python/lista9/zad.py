import sys
from PyQt5.QtWidgets import QApplication, QMessageBox, QLabel, QMainWindow, QTableWidget, QTableWidgetItem, QPushButton, QGridLayout, QWidget, QDialog, QLineEdit
from PyQt5.QtGui import QIntValidator,QDoubleValidator,QFont
from PyQt5 import QtCore
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.sql import func
from sqlalchemy import Table, Column, Integer, ForeignKey, String, DateTime, create_engine, select, Boolean
from sqlalchemy.orm import sessionmaker, validates

Base = declarative_base()

engine = create_engine("sqlite:///zad.db", echo=True)


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
    wypozyczono = Column(Boolean, nullable=False, default=False)

class Wypozyczenie(Base):
    __tablename__ = 'Wypozyczenie'
    id = Column(Integer, primary_key=True)
    id_ksiazka = Column(Integer,ForeignKey("Ksiazka.id"))
    id_osoba = Column(Integer,ForeignKey("Osoba.id"))
    data_wypozyczenia = Column(DateTime(timezone=True), server_default=func.now())
    data_oddania = Column(DateTime(timezone=True), nullable=True)


Base.metadata.create_all(engine)

Session = sessionmaker(bind=engine)
sesja = Session()


def dodaj_osobe(txtBoxes):
    o = Osoba(imie = txtBoxes[0].text(), nazwisko = txtBoxes[1].text(), email = txtBoxes[2].text())
    sesja.add(o)
    sesja.commit()

def dodaj_ksiazke(txtBoxes):
    k = Ksiazka(nazwa = txtBoxes[0].text(), autor = txtBoxes[1].text(), rok = int(txtBoxes[2].text()))
    sesja.add(k)
    sesja.commit()

def usun_osobe(txtBoxes):
    o = Osoba(imie = txtBoxes[0].text(), nazwisko = txtBoxes[1].text(), email = txtBoxes[2].text())
    sesja.delete(o)
    sesja.commit()

def usun_ksiazke(txtBoxes):
    k = Ksiazka(nazwa = txtBoxes[0].text(), autor = txtBoxes[1].text(), rok = int(txtBoxes[2].text()))
    sesja.delete(k)
    sesja.commit()

def usun_wypozyczenie(txtBoxes):
    id_ksiazki = (sesja.query(Ksiazka).filter(Ksiazka.nazwa == txtBoxes[3].text() and Ksiazka.autor == txtBoxes[4].text() and Ksiazka.rok == int(txtBoxes[5].text())).all())[0]
    id_osoby = (sesja.query(Osoba).filter(Osoba.imie == txtBoxes[0].text() and Osoba.nazwisko == txtBoxes[1].text() and Osoba.email == txtBoxes[2].text()).all())[0]
    w = (sesja.query(Wypozyczenie).filter(Wypozyczenie.id_ksiazka == id_ksiazki.id and Wypozyczenie.id_osoba == id_osoby.id))[0]
    id_ksiazki.wypozyczono = False
    sesja.delete(w)
    sesja.commit()

def wypozyczenie_ksiazki(txtBoxes):
    id_ksiazki = sesja.query(Ksiazka).filter(Ksiazka.nazwa.like(txtBoxes[3].text()), Ksiazka.autor.like(txtBoxes[4].text()), Ksiazka.rok.like(txtBoxes[5].text())).all()
    id_osoby = sesja.query(Osoba).filter(Osoba.imie.like(txtBoxes[0].text()), Osoba.nazwisko.like(txtBoxes[1].text()), Osoba.email.like(txtBoxes[2].text())).all()
    # id_ksiazki = sesja.query(Ksiazka).filter(Ksiazka.nazwa == txtBoxes[3].text() and Ksiazka.autor == txtBoxes[4].text() and Ksiazka.rok == int(txtBoxes[5].text())).all()
    # id_osoby = sesja.query(Osoba).filter(Osoba.imie == txtBoxes[0].text() and Osoba.nazwisko == txtBoxes[1].text() and Osoba.email == txtBoxes[2].text()).all()
    w = Wypozyczenie(id_ksiazka = id_ksiazki[0].id, id_osoba = id_osoby[0].id)
    if id_ksiazki[0].wypozyczono == False:
        id_ksiazki[0].wypozyczono = True
        sesja.add(w)
        sesja.commit()
    else:
        print("Ksiazka juz wypozyczona!")

def zwrot_ksiazki(txtBoxes):
    id_ksiazki = (sesja.query(Ksiazka).filter(Ksiazka.nazwa == txtBoxes[3].text() and Ksiazka.autor == txtBoxes[4].text() and Ksiazka.rok == int(txtBoxes[5].text())).all())[0]
    id_osoby = (sesja.query(Osoba).filter(Osoba.imie == txtBoxes[0].text() and Osoba.nazwisko == txtBoxes[1].text() and Osoba.email == txtBoxes[2].text()).all())[0]
    w = (sesja.query(Wypozyczenie).filter(Wypozyczenie.id_ksiazka == id_ksiazki.id and Wypozyczenie.id_osoba == id_osoby.id))[-1]#dodaj do ostatniego pasujacego wypozyczenia
    w.data_oddania = func.now()
    id_ksiazki.wypozyczono = False
    sesja.commit()


# Create the application
app = QApplication(sys.argv)
# Set up table
table = QTableWidget()

def setTableContent(t):
    q = sesja.query(t)
    desc = q.column_descriptions
    names = desc[0]['type'].__table__.columns.keys()
    table.setColumnCount(len(names))
    table.setHorizontalHeaderLabels(names)
    table.setRowCount(0)
    for u in q.all():
        rows = table.rowCount()
        table.setRowCount(rows + 1)
        for i in range(len(names)):
            table.setItem(rows, i, QTableWidgetItem(str(getattr(u,names[i]))))                
    table.resizeColumnsToContents()

def makeTextBox(mxLen, onlyInt = False):
    le = QLineEdit()
    if onlyInt:
        le.setValidator(QIntValidator())
    le.setMaxLength(mxLen)
    # le.setFont(QFont("Arial",20))
    return le

def acceptButtonAction(txtBoxes, func, popup):
    for t in txtBoxes:
        if not t.text():
            return
    if func==0:
        dodaj_osobe(txtBoxes)
        setTableContent(Osoba)
    if func==1:
        dodaj_ksiazke(txtBoxes)
        setTableContent(Ksiazka)
    if func==2:
        wypozyczenie_ksiazki(txtBoxes)
        setTableContent(Wypozyczenie)
    if func==3:
        zwrot_ksiazki(txtBoxes)
        setTableContent(Wypozyczenie)
    if func==4:
        usun_osobe(txtBoxes)
        setTableContent(Osoba)
    if func==5:
        usun_ksiazke(txtBoxes)
        setTableContent(Ksiazka)
    if func==6:
        usun_wypozyczenie(txtBoxes)
        setTableContent(Wypozyczenie)
    popup.done(0)


class PopupWindow(QDialog):
    def __init__(self, type, func):
        super().__init__()
        # self.setWindowTitle("Podaj informacje")
        self.resize(300,200)
        grid = QGridLayout()

        textBoxes = []
        rowIter = 0
        if (type&1)>0:#potrzebne dane osoby
            lb = QLabel("Osoba")
            lb.setAlignment(QtCore.Qt.AlignCenter)
            grid.addWidget(lb,rowIter,0,1,3)
            rowIter+=1
            grid.addWidget(QLabel("Imie"),rowIter,0,1,1)
            textBoxes.append(makeTextBox(20))
            grid.addWidget(textBoxes[-1],rowIter,1,1,2)
            rowIter+=1
            grid.addWidget(QLabel("Nazwisko"),rowIter,0,1,1)
            textBoxes.append(makeTextBox(30))
            grid.addWidget(textBoxes[-1],rowIter,1,1,2)
            rowIter+=1
            grid.addWidget(QLabel("Email"),rowIter,0,1,1)
            textBoxes.append(makeTextBox(50))
            grid.addWidget(textBoxes[-1],rowIter,1,1,2)
            rowIter+=1

        if (type&2)>0:#potrzebne dane ksiazki
            lb = QLabel("Ksiazka")
            lb.setAlignment(QtCore.Qt.AlignCenter)
            grid.addWidget(lb,rowIter,0,1,3)
            rowIter+=1
            grid.addWidget(QLabel("Nazwa"),rowIter,0,1,1)
            textBoxes.append(makeTextBox(30))
            grid.addWidget(textBoxes[-1],rowIter,1,1,2)
            rowIter+=1
            grid.addWidget(QLabel("Autor"),rowIter,0,1,1)
            textBoxes.append(makeTextBox(50))
            grid.addWidget(textBoxes[-1],rowIter,1,1,2)
            rowIter+=1
            grid.addWidget(QLabel("Rok"),rowIter,0,1,1)
            textBoxes.append(makeTextBox(4,True))
            grid.addWidget(textBoxes[-1],rowIter,1,1,2)
            rowIter+=1
            
        grid.addWidget(QLabel(""),rowIter,0,1,0)
        rowIter+=1
        acceptButton = QPushButton("Zatwierdz")
        acceptButton.clicked.connect(lambda: acceptButtonAction(textBoxes, func, self))
        grid.addWidget(acceptButton,rowIter,0,1,3)

        self.layout = grid
        self.setLayout(self.layout)

def makePopUp(type, func, name):
    w = PopupWindow(type,func)
    w.setWindowTitle(name)
    w.exec()

# Create the application's window
class TabOsoba(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("BazaDanychApp")
        self.resize(600,400)
        # Set up buttons
        buttons = [
            QPushButton("Dodaj Osoba"),
            QPushButton("Dodaj Ksiazka"),
            QPushButton("Dodaj Wypozyczenie"),
            QPushButton("Dodaj Zwrot"),
            QPushButton("Usun Osoba"),
            QPushButton("Usun Ksiazka"),
            QPushButton("Usun Wypozyczenie"),
            QPushButton("NOP"),
            QPushButton("Wyswietl Osoba"),
            QPushButton("Wyswietl Ksiazka"),
            QPushButton("Wyswietl Wypozyczenie"),
            QPushButton("Wyswietl Querry"),
        ]
        buttons[0].clicked.connect(lambda: makePopUp(1,0,"Dodaj Osoba"))
        buttons[1].clicked.connect(lambda: makePopUp(2,1,"Dodaj Ksiazka"))
        buttons[2].clicked.connect(lambda: makePopUp(3,2,"Dodaj Wypozyczenie"))
        buttons[3].clicked.connect(lambda: makePopUp(3,3,"Dodaj Zwrot"))
        buttons[4].clicked.connect(lambda: makePopUp(1,4,"Usun Osoba"))
        buttons[5].clicked.connect(lambda: makePopUp(2,5,"Usun Ksiazka"))
        buttons[6].clicked.connect(lambda: makePopUp(3,6,"Usun Wypozyczenie"))
        # buttons[7].clicked.connect()
        buttons[8].clicked.connect(lambda: setTableContent(Osoba))
        buttons[9].clicked.connect(lambda: setTableContent(Ksiazka))
        buttons[10].clicked.connect(lambda: setTableContent(Wypozyczenie))
        # buttons[11].clicked.connect()

        # Set main widget
        widget = QWidget()

        # Set up grid layout
        grid = QGridLayout()  
        widget.setLayout(grid)

        grid.addWidget(buttons[0],0,0)
        grid.addWidget(buttons[1],0,1)
        grid.addWidget(buttons[2],0,2)
        grid.addWidget(buttons[3],0,3)
        grid.addWidget(buttons[4],1,0)
        grid.addWidget(buttons[5],1,1)
        grid.addWidget(buttons[6],1,2)
        # grid.addWidget(buttons[7],1,3)

        grid.addWidget(table,2,0,1,4)

        grid.addWidget(buttons[8],3,0)
        grid.addWidget(buttons[9],3,1)
        grid.addWidget(buttons[10],3,2)
        # grid.addWidget(buttons[11],3,3)


        setTableContent(Osoba)

        self.setCentralWidget(widget)



win = TabOsoba()
win.show()


sys.exit(app.exec_())