#include <stdio.h>

using namespace std;

int main(){
    int n,m,h;
    scanf("%d %d %d", &n, &m, &h);
    int tab[m+1], sum=0;
    for(int i=1; i<=m; i++){
        scanf("%d",&tab[i]);
        sum+=tab[i];
    }
    
    if(sum < n){
        printf("-1\n");
        return 0;
    }
    // if(sum == n){
    //     printf("1\n");
    //     return 0;
    // }
    // sum > n    
    // if(tab[h] == 1){
    //     printf("0\n");
    //     return 0;
    // }

    double res = 1;
    double i = (sum-tab[h]), j = (sum-1);

    while((i>(sum-tab[h]-n+1)) || (j>(sum-n))){
        if(i>(sum-tab[h]-n+1)){
            res*=i;
            i-=1.0;
        }
        if(j>(sum-n)){
            res/=j;
            j-=1.0;
        }
    }




    // for(int i = (sum-tab[h]); i>(sum-tab[h]-n+1); i--)
    //     res*=i;
    // for(int i = (sum-1); i>(sum-n); i--)
    //     res/=i;

    printf("%.6f\n", 1.0 - res);
}