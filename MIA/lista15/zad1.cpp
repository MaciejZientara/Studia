#include <stdio.h>

using namespace std;

int main(){
    int n,k;
    scanf("%d %d", &n, &k);

    int tab[n];
    for(int i=1; i<n; i++)
        scanf("%d", &tab[i]);
    
    int iter=1;
    while(iter<k)
        iter += tab[iter];
    
    if(iter==k)
        printf("YES\n");
    else
        printf("NO\n");


}