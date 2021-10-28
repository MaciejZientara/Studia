#include <iostream>

using namespace std;

int main(){
    int n,d;
    cin >> n >> d;
    int tab[n];
    for(int i=0; i<n; i++){
        cin >> tab[i];
    }

/*
    zaczynam for od i=0 i szukam pozycji dla j (np binsearch, albo liniowo od prawej)

    jak znajde to beda nalezaly wszystkie pary zlozone z i oraz dwoch inncyh punktow miedzy i+1 a j włącznie

    potem krok dla i i przesuniecie j (dalej mozna liniowo)




*/

    long long ret = 0;

    long long j=n-1;
    while(tab[j] - tab[0] > d)
        j--;
    if(j<2)
        j=2;
    //znaleziony j dla pierwszego elementu
    
    for(long long i=0; i<n-2; i++){
        while(tab[j+1]-tab[i] <= d && j<(n-1))
            j++;

        if(j-i<2)
            j=i+2;

        if(tab[j]-tab[i] <= d)
            ret += ((j-i) * (j-i-1))/2;    
    }



//brut
    // for(int i=0; i<n; i++){
    //     bool fine = true;
    //     for(int j=i+2; j<n && fine; j++){
    //         if(tab[j] - tab[i] <= d)
    //             ret += (j-i-1);
    //         else
    //             fine = false;
    //     }
    // }

    cout << ret << "\n";
}