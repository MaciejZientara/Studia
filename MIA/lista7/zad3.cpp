#include <stdio.h>

using namespace std;

int main(){
    int n, k;
    scanf("%d %d", &n, &k);
    if(k<1 || k>((1<<n)-1) || k%2==0){
        printf("-1\n");
        return 0;
    }

    int tab[n];
    for(int i=0; i<n; i++)
        tab[i]=i+1;


    for(int l=1; l<k; l+=2){
//        int mid = 
    }

    //zamieniac miejscami pierwsza czesc z druga jezeli sa parzytsej dlugosci aby dodac 2
    //jak nieparzyste to zamienic pierwszy z drugiego z ostatnim z pierwszego





}