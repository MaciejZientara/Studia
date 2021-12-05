#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

void rec(string s,string res,int iter){
    if(iter>=s.length()){
        cout << res << "\n";
        return;
    }
    for_each(s.begin(),s.end(),[&](char& c){
        if(c==' ')
            return;
        char tmp = c;
        c = ' ';
        res[iter] = tmp;
        rec(s,res,iter+1);
        c = tmp;
        res[iter] = ' ';
    });
}

int main(){
    string s;

    s = "abc";
    // cin >> s;

    string res(s.length(),' ');
    //stworz string res o takiej samej dlugosci co s
    rec(s,res,0);
}