#include <iostream>

#define MAX 10000000

using namespace std;

int mn = MAX;
string s;
void function(char c, int b, int e, int ret){
    //cout << c << " " << b  << " " << e << " " << ret << "\n";
    if(ret>mn){return;}

    if(b>=e){
        if(mn>ret)
            mn = ret;
        return;
    }

    int sum = 0;
    int mid = (b+e)/2;
    for(int i=mid; i<e; i++)
        if(s[i]!=c)
            sum++;
    function(c+1,b,mid,ret+sum);
    
    sum=0;
    if(mid==b)
        mid++;
    for(int i=b; i<mid; i++)
        if(s[i]!=c)
            sum++;
    function(c+1,mid,e,ret+sum);
}


int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        cin >> s;
        function('a',0,n,0);
        cout << mn << "\n";
        mn = MAX;
    }
}