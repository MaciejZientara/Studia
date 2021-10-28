#include <stdio.h>

using namespace std;

int main(){
    long long n, k;
    scanf("%lld %lld", &n, &k);

    // bool turn = false; //false - Sasha, true - Lena
    // long long sumS = 0, sumL = 0;
    // while(sumS + sumL < n){
    //     if(sumS + sumL + k > n)
    //         break;
    //     if(turn)
    //         sumL+=k;
    //     else
    //         sumS+=k;
    //     turn = !turn;
    // }

    // if(sumS>sumL)
    //     printf("YES\n");
    // else
    //     printf("NO\n");

    if((n/k)%2)
        printf("YES\n");
    else
        printf("NO\n");


}