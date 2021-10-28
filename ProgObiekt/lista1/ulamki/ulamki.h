#include <stdio.h>
#include <stdlib.h>

typedef struct ul{
    int l;
    int m;
}ulamek;

ulamek* nowy_ulamek(int a, int b);

ulamek* dodaj_ulamek(ulamek* a, ulamek* b);

ulamek* odejmij_ulamek(ulamek* a, ulamek* b);

ulamek* pomnoz_ulamek(ulamek* a, ulamek* b);

ulamek* podziel_ulamek(ulamek* a, ulamek* b);

void Bdodaj_ulamek(ulamek* a, ulamek* b);

void Bodejmij_ulamek(ulamek* a, ulamek* b);

void Bpomnoz_ulamek(ulamek* a, ulamek* b);

void Bpodziel_ulamek(ulamek* a, ulamek* b);

void wypisz_ulamek(ulamek* a);

