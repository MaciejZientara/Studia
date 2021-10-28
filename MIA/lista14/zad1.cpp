#include <stdio.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(){
    int r1, r2, c1, c2;
    scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
    
    int res = 0;
    int rows = abs(r1-r2), columns = abs(c1-c2);

    //rook
    if(r1!=r2)
        res++;
    if(c1!=c2)
        res++;
    printf("%d ", res);

    res=0;
    //bishop
    if(((r1+c1)%2)==((r2+c2)%2)){
        if(rows!=columns)
            res=2;
        else
            res=1;
    }
    printf("%d ", res);

    res=0;
    //king
    res = max(rows,columns);//rows + columns - min(rows,columns);

    printf("%d\n", res);
}