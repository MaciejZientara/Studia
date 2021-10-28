#include <stdio.h>
#include <stdlib.h>
#include "ulamki.h"

int main(){
    ulamek* a=nowy_ulamek(4,2);
    ulamek* b=nowy_ulamek(1,2);
    wypisz_ulamek(a);
    wypisz_ulamek(b);
    printf("\n");
    wypisz_ulamek(pomnoz_ulamek(a,b));
    wypisz_ulamek(podziel_ulamek(a,b));
    wypisz_ulamek(podziel_ulamek(b,a));
    wypisz_ulamek(dodaj_ulamek(a,b));
    wypisz_ulamek(odejmij_ulamek(a,b));
    wypisz_ulamek(odejmij_ulamek(b,a));

    printf("\n\n");

    b=nowy_ulamek(1,2);
    Bpomnoz_ulamek(a,b);
    wypisz_ulamek(b);
    b=nowy_ulamek(1,2);
    Bpodziel_ulamek(a,b);
    wypisz_ulamek(b);
    b=nowy_ulamek(1,2);
    Bpodziel_ulamek(b,a);
    wypisz_ulamek(a);
    a=nowy_ulamek(4,2);
    Bdodaj_ulamek(a,b);
    wypisz_ulamek(b);
    b=nowy_ulamek(1,2);
    Bodejmij_ulamek(a,b);
    wypisz_ulamek(b);
    b=nowy_ulamek(1,2);
    Bodejmij_ulamek(b,a);
    wypisz_ulamek(a);

}
