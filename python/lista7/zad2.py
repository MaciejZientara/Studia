import urllib.request as urlR
import sched, time
import os
import difflib
import threading

WaitTime = 60 # ilosc seknund pomiedzy Compare

s = sched.scheduler(time.time, time.sleep)

# listy pythonowe sa thread safe, ale same dane przechowywane w liscie nie musza byc
# co oznacza, ze jesli dwa watki odwoluja sie do tych samych danych w liscie np list[0]
# moze wystapic race condition - malo operacji w python jest atomowych!
# listy pythonowe przechowuja wskazniki na dane, a poniewaz kazdy watek bedzie zajmowal sie
# innym polem listy i modyfikowal dane przez wskaznik nie powinno dojsc do zadnych race condition

# zachowam w watku glownym scheduler uruchamiajacy timer
# dopiero w funkcji Compare i firstDownload kazda strona bedzie analizowana w innym watku

urls = []#lista stron do monitorowania

savedData = []#zapisane dane stron, potrzebne do porownywania zmian

threads = []#lista watkow - pomaga w zarzadzaniu nimi

def CompareHelper(i):
    buffer = []
    changed = False
    with urlR.urlopen(urls[i]) as f:
        lines1 = savedData[i]
        lines2 = f.readlines()#zwraca liste 'bytes'
        if type(lines1[0]) == bytes:#string nie ma operacji .decode, wiec musze sprawdzic zgodnosc typu
            for q in range(len(lines1)):
                lines1[q] = lines1[q].decode("utf-8")#zamienia 'bytes' na 'string'
        if type(lines2[0]) == bytes:
            for q in range(len(lines2)):
                lines2[q] = lines2[q].decode("utf-8")
        for l in difflib.Differ().compare(lines1,lines2):#wymaga list 'string', a nie 'bytes'
            if not l.startswith(' '):
                buffer.append(l)
                changed = True
    if changed:
        savedData[i] = lines2
    
    for j in range(i):#poczekaj na zakonczenie sie watkow od 0 do i
        threads[j].join()
    #dopiero wypisz cale wyjscie, unika race condition przy print
    if changed:
        print(urls[i])
        for b in buffer:
            print(b)
    else:
        print(urls[i] + ' is unchanged')
    print('\n')

def Compare(): 
    print("Comparing...           \n")
    for i in range(len(urls)):
        threads[i] = threading.Thread(target=CompareHelper, args=(i,))
        threads[i].start()

    #poczekaj na zakonczenie wszystkich watkow przed rozpoczeciem timera
    for i in range(len(urls)):
        threads[i].join()
    
    s.enter(0, 1, clockDisplay, (WaitTime,))


def clockDisplay(seconds):
    print(f'Next comapre in: {seconds}          ',end='\r')
    if seconds == 1:
        s.enter(1, 1, Compare, ())
    else:
        s.enter(1, 1, clockDisplay, (seconds-1,))


def firstDownloadHelper(u, pos):
    with urlR.urlopen(u) as f:
        savedData[pos] = f.readlines()

def firstDownload():#zapisuje dane do savedData w celu dalszych porownan
    pos = 0
    for u in urls:
        savedData.append('')#zrob miejsce na dane
        threads.append(threading.Thread(target=firstDownloadHelper, args=(u,pos)))#dodaj watek do listy
        threads[-1].start()#rozpocznij ostatni watek w liscie
        pos+=1#ustalam tutaj pozycje, aby uniknac race condition i zlej kolejnosci miedzy urls[] a savedData[]
    #poczekaj na zakonczenie wszystkich watkow przed rozpoczeciem timera
    for i in range(len(threads)):
        threads[i].join()

#tu wypelnij liste urls
urls.append('https://ii.uni.wroc.pl/')
urls.append('https://www.python.org/')
# urls.append('https://www.youtube.com/')

os.system('python3 -m http.server &')#uruchamia strone na localhost
urls.append('http://127.0.0.1:8000/')#localhost
#aby zmienic zawartosc wyswietlana na localhost, wystarczy dodac nowy plik
#w katalogu, ktory wyswietla strona (mozna tez usunac jakis plik)

#UWAGA!!! program hostujacy strone na localhost nie zamyka sie automatycznie!
#pkill python3

firstDownload()

s.enter(0, 1, clockDisplay, (WaitTime,))
s.run()
