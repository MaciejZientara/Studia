#https://danepubliczne.imgw.pl/data/dane_pomiarowo_obserwacyjne/dane_meteorologiczne/miesieczne/klimat/
# k_m_d_format.txt zawiera informacje do header
# dane z katalogu 2020, plik k_m_d_2020.csv

#https://www.gov.pl/web/koronawirus/wykaz-zarazen-koronawirusem-sars-cov-2
#dane do pobrania, dodanie pierwszego header 'Dzien' (Dzien;Data;Nowe przypadki; ...)

import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime


klimat = pd.read_csv('k_m_d_2020.csv', encoding='iso8859-2', header=None, 
    names=[
    'Kod_Stacji',
    'Nazwa_Stacji',
    'Rok',
    'Miesiąc',
    'Absolutna_Temperatura_Max',
    'TMAX',
    'Średnia_Temperatura_Max',
    'TMXS',
    'Absolutna_Temperatura_Min',
    'TMIN',
    'Średnia_Temperatura_Min',
    'TMNS',
    'Średnia_Temperatura_Miesiąc',
    'STM',
    'Min_Temperatura_Grunt',
    'TMNG',
    'Miesiąc_Suma_Opady',
    'SUMM',
    'Max_Suma_Doba_Opady',
    'OPMX',
    'Pierwszy_Dzień_Opad_Max',
    'Ostatni_Dzień_Opad_Max',
    'Max_Wysokość_Śnieg',
    'PKSN',
    'Liczba_Dni_Śnieg',
    'Liczba_Dni_Deszcz',
    'Liczba_Dni_Opad_Śnieg'
])


dateparse = lambda dates: [datetime.strptime(d, '%d.%m.%Y').date() for d in dates]
strconv = lambda x : str(x.replace(' ', ''))

chorzy = pd.read_csv('Zakazenia30323112020.csv', encoding='utf8', sep=';', parse_dates=['Data'], date_parser=dateparse, converters={"Nowe przypadki" : strconv, "Wszystkie zgony kumulatywnie" : strconv})

# print(chorzy)

def averageList(l):
    return (sum(l) / len(l))

def AverageMonths(months, kl, ch):
    for i in months:
        klimat_okres_tmp = klimat.loc[(klimat['Miesiąc'] == i)]
        chorzy_okres_tmp = chorzy.loc[(pd.DatetimeIndex(chorzy['Data']).month == i)]

        kl_tmp = []
        ch_tmp = []
        for i, j in klimat_okres_tmp.iterrows():
            kl_tmp.append(int(j['Średnia_Temperatura_Miesiąc']))
        for i, j in chorzy_okres_tmp.iterrows():
            ch_tmp.append(int(j['Nowe przypadki']))

        kl.append(averageList(kl_tmp))
        ch.append(averageList(ch_tmp))
        kl_tmp.clear()
        ch_tmp.clear()


months1 = [4,5,6,7]
klimat1_MonthAverage = []
chorzy1_MonthAverage = []

AverageMonths(months1,klimat1_MonthAverage,chorzy1_MonthAverage)

months2 = [8,9,10,11]
klimat2_MonthAverage = []
chorzy2_MonthAverage = []

AverageMonths(months2,klimat2_MonthAverage,chorzy2_MonthAverage)


fig, axs = plt.subplots(2, 2)
fig.suptitle('Zależność temperatury do zachorowań', fontsize=16)

axs[0, 0].plot(months1,klimat1_MonthAverage, color = 'g', marker = 'o', linestyle = 'dashed')
axs[0, 1].plot(months2,klimat2_MonthAverage, color = 'g', marker = 'o', linestyle = 'dashed')
axs[1, 0].plot(months1,chorzy1_MonthAverage, color = 'r', marker = 'o', linestyle = 'dashed')
axs[1, 1].plot(months2,chorzy2_MonthAverage, color = 'r', marker = 'o', linestyle = 'dashed')


axs[0, 0].set_title('Wiosna->Lato')
axs[0, 1].set_title('Lato->Jesień')
# axs[1, 0].set_title('')
# axs[1, 1].set_title('')
axs[0,0].set(xlabel='Miesiące', ylabel='Temperatury')
axs[0,1].set(xlabel='Miesiące', ylabel='Temperatury')
axs[1,0].set(xlabel='Miesiące', ylabel='Zachorowania')
axs[1,1].set(xlabel='Miesiące', ylabel='Zachorowania')

for ax in axs.flat:
    ax.label_outer()

plt.show()
