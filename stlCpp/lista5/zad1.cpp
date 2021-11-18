#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <algorithm>

using namespace std;

auto parse(string s){
    deque<string> result;
    string tmp;
    auto ln = s.length();
    for(int i=0; i<ln; i++){
        tmp = "";
        while(s[i]!=' ' && s[i]!='\0'){
            tmp+=s[i];
            i++;
        }
        result.push_back(tmp);
    }
    return result;
}

map <string,int> STRtoINT = {{"+",0},{"-",1},{"/",2},{"*",3},{"%",4},{"(",5},{")",6}};

auto change(deque<string> d){
    deque<string> sign;
    deque<string> values;

    while(!d.empty()){
        auto tmp = STRtoINT.find(d.back());
        if(tmp == STRtoINT.end())
            values.push_back(d.back());
        else{
            int v = tmp->second;
            if(v==5){
                string res = values.back();
                values.pop_back();
                res += " " + values.back();
                values.pop_back();
                res += " " + sign.back();
                sign.pop_back();
                values.push_back(res);
            }
            if(v<5)
                sign.push_back(d.back());
        }
        d.pop_back();
    }

    string ret = values.back();
    values.pop_back();
    return parse(ret);
}

int main(){
    string s;

    // s = "( 2 + 2 )";
    // s = "( 2 * ( 3 + 4 ) )";
    s = "( ( ( 3 * 71 ) + 155 ) / ( 7 * 2 ) )";

    // getline(cin,s);

    deque<string> infix = parse(s);
    for_each(infix.begin(),infix.end(),[](string z){cout << z << " ";});
    cout << "\n";

    deque<string> rpn = change(infix);
    for_each(rpn.begin(),rpn.end(),[](string z){cout << z << " ";});
    cout << "\n";
}

