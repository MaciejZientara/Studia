import urllib.request as urlR
import sched, time
import os
import difflib

WaitTime = 60 # ilosc seknund pomiedzy Compare

s = sched.scheduler(time.time, time.sleep)

urls = []#lista stron do monitorowania

savedData = []#zapisane dane stron, potrzebne do porownywania zmian

def Compare(): 
    print("Comparing...           ")

    for i in range(len(urls)):
        changed = False
        with urlR.urlopen(urls[i]) as f:
            lines1 = savedData[i]
            lines2 = f.readlines()
            for l in difflib.Differ().compare(lines1,lines2):
                if not l.startswith(' '):
                    print(l)
                    changed = True
        if not changed:
            print(urls[i] + ' is unchanged')
        else:
            savedData[i] = lines2

    s.enter(0, 1, clockDisplay, (WaitTime,))


def clockDisplay(seconds):
    print(f'Next comapre in: {seconds}          ',end='\r')
    if seconds == 1:
        s.enter(1, 1, Compare, ())
    else:
        s.enter(1, 1, clockDisplay, (seconds-1,))


def firstDownload():#zapisuje dane do savedData w celu dalszych porownan
    for u in urls:
        with urlR.urlopen(u) as f:
            savedData.append(f.readlines())

#tu wypelnij liste urls
urls.append('https://ii.uni.wroc.pl/')
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
