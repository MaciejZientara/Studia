#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    int tab[n+2];
    for(int i = 1; i<=(n-1); i++){
        fprintf(stdout, "? %d %d\n", i, i+1);
        fflush(stdout);
        scanf("%d",&tab[i]);
    }

    if(n%2){
        fprintf(stdout, "? %d %d\n", 1, n);
        fflush(stdout);
        scanf("%d",&tab[n]);
    }
    else{
        fprintf(stdout, "? %d %d\n", 1, n-1);
        fflush(stdout);
        scanf("%d",&tab[n]);
    }

    int ret[n+2];
    int sum=0;
    for(int i=1; i<n; i+=2)
        sum+=tab[i];

    if(n%2){
        ret[n] = tab[n]-sum;
    }else{
        ret[n] = sum-tab[n];
    }
    for(int i=n-1; i>0; i--)
        ret[i] = tab[i] - ret[i+1];
    
    printf("! ");
    for(int i =1; i<=n; i++)
        printf("%d ",ret[i]);
    printf("\n");
    fflush(stdout);
    
    exit(0);
}