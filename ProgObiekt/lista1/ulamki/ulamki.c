#include <stdio.h>
#include <stdlib.h>
#include "ulamki.h"

int nwd(int a, int b){
    if(a<0)
        a*=-1;
    if(b<0)
        b*=-1;
    if(a>b){
        int c=a;
        a=b;
        b=c;
    }
    while(b>0){
        int c=a%b;
        a=b;
        b=c;
    }
    return a;
}

ulamek* nowy_ulamek(int a, int b){
    ulamek* c=(ulamek*)calloc(sizeof(ulamek),1);
    if(b==0){
        printf("DZIELENIE PRZEZ ZERO!\n");
        b++;
    }
    if(b<0){
        a*=-1;
        b*=-1;
    }
    c->l=a;
    c->m=b;
    int x=nwd(c->l,c->m);
    c->l/=x;
    c->m/=x;
    return c;
}

ulamek* dodaj_ulamek(ulamek* a, ulamek* b){
    ulamek* c=(ulamek*)calloc(sizeof(ulamek),1);
    c->l=a->l*b->m + a->m*b->l;
    c->m=a->m*b->m;
    int x=nwd(c->l,c->m);
    c->l/=x;
    c->m/=x;
    return c;
}

ulamek* odejmij_ulamek(ulamek* a, ulamek* b){
    ulamek* c=(ulamek*)calloc(sizeof(ulamek),1);
    c->l=b->l*-1;
    c->m=b->m;
    return dodaj_ulamek(a,c);
}

ulamek* pomnoz_ulamek(ulamek* a, ulamek* b){
    ulamek* c=(ulamek*)calloc(sizeof(ulamek),1);
    c->l=a->l*b->l;
    c->m=a->m*b->m;
    int x=nwd(c->l,c->m);
    c->l/=x;
    c->m/=x;
    return c;
}

ulamek* podziel_ulamek(ulamek* a, ulamek* b){
    ulamek* c=(ulamek*)calloc(sizeof(ulamek),1);
    c->l=b->m;
    c->m=b->l;
    return pomnoz_ulamek(a,c);
}



void Bdodaj_ulamek(ulamek* a, ulamek* b){
    b->l=a->l*b->m + a->m*b->l;
    b->m*=a->m;
    int x=nwd(b->l,b->m);
    b->l/=x;
    b->m/=x;
}

void Bodejmij_ulamek(ulamek* a, ulamek* b){
    b->l*=-1;
    Bdodaj_ulamek(a,b);
}

void Bpomnoz_ulamek(ulamek* a, ulamek* b){
    b->l*=a->l;
    b->m*=a->m;
    int x=nwd(b->l,b->m);
    b->l/=x;
    b->m/=x;
}

void Bpodziel_ulamek(ulamek* a, ulamek* b){
    int tmp=b->m;
    b->m=b->l;
    b->l=tmp;
    Bpomnoz_ulamek(a,b);
}



void wypisz_ulamek(ulamek* a){
    printf("%d/%d\n",a->l,a->m);
}

