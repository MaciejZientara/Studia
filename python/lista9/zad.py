import sys
from PyQt5.QtWidgets import QApplication, QMessageBox, QLabel, QMainWindow, QTableWidget, QTableWidgetItem, QPushButton, QGridLayout, QWidget
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

# TODO dodac funkcje z lista8/zad1.py




# Create the application

app = QApplication(sys.argv)


# Create the application's window
class TabOsoba(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("BazaDanychApp")
        self.resize(800,600)
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

        # Set up table
        table = QTableWidget()

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
        grid.addWidget(buttons[11],3,3)

        # Set up the view and load the data
        # self.view = grid
        q = sesja.query(Osoba)
        desc = q.column_descriptions
        names = desc[0]['type'].__table__.columns.keys()
        table.setColumnCount(len(names))
        table.setHorizontalHeaderLabels(names)
        for u in q.all():
            rows = table.rowCount()
            table.setRowCount(rows + 1)
            for i in range(len(names)):
                table.setItem(rows, i, QTableWidgetItem(str(getattr(u,names[i]))))                
        table.resizeColumnsToContents()
        # self.setCentralWidget(self.view)
        self.setCentralWidget(widget)



win = TabOsoba()
win.show()


sys.exit(app.exec_())