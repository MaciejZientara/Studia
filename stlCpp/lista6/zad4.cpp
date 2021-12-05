#include <iostream>
#include <fstream>
#include <ctype.h>
#include <numeric>

using namespace std;

#define N 26

int letterCount[N];//abcdefghijklmnopqrstuvwxyz

int main(int argc, char **argv){
    if(argc<2){
        cout << "podaj nazwe pliku!\n";
        return 0;
    }
    ifstream myfile;
    while(!myfile.is_open())
        myfile.open(argv[1]);
    char c;
    myfile >> c;
    while(!myfile.eof()){
        if(isalpha(c))//czy jest litera
            letterCount[(char)tolower(c)-'a']++;//sprowadz do malej litery alfabetu i zwieksz counter tej litery w tablicy
        myfile >> c;
    }
    myfile.close();

    float sum = accumulate(letterCount, letterCount+N, 0);

    for(int i=0; i<N; i++){
        int x = letterCount[i];
        cout << char('a'+i) << " ma czestosc wystepowania: " << x << "/" << sum << " = " << (float)x/sum << "\t\t";
        //histogram
        int stars = (x*100.0)/sum;
        while(stars--)
            cout << "*";
        cout << "\n";
    }
}