#include <stdio.h>
#include <algorithm>

using namespace std;

int main(){
    int a, b;
    scanf("%d %d", &a, &b);

    int res=7-max(a,b);
    int div = 6;

    if(res%2==0){
        res/=2;
        div/=2;
    }
    if(res%3==0){
        res/=3;
        div/=3;
    }
    printf("%d/%d\n",res,div);
}