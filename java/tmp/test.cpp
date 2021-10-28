#include <stdio.h>

void funkcja(int a, int* b, int c){
    a++;
    (*b)++;
    c++;
}

int main(){
    int a=0, b=0, c=0;
    funkcja(a,&b,c);
    printf("%d %d %d\n",a,b,c);

    int _1 = 1;
    int _ = 1;
    int abs1;
    // int 123abc;

    int x=10;
    for(; ""; x<5)
        x--;
    x--;

    //ile algorytmów ma Garbage collector w javie, czy możesz wybrać którego ma użyć?
    //czy możesz zasugerować uruchomienie GC funkcją system.gc?

}