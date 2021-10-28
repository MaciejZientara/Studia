#include <stdio.h>
#include <math.h>
#include <map>

using namespace std;

map <int, int> M;

int divisorAmount(int x){
    map<int,int>::iterator it = M.find(x);
    if(it!=M.end())
        return it->second;

    int n = sqrt(x);
    int ret = 0;
    for(int i=1; i<=n; i++)
        if(x%i==0){
            ret++;//i
            if(i!=(x/i))
                ret++;//x/i
        }


    M.insert(pair<int,int>(x,ret));
    return ret;
}


int main(){
    int a,b,c;
    scanf("%d %d %d", &a, &b, &c);

    int sum = 0;
    for(int i=1; i<=a; i++)
        for(int j=1; j<=b; j++)
            for(int k=1; k<=c; k++)
                sum+=divisorAmount(i*j*k);
    
    printf("%d\n", sum);
}