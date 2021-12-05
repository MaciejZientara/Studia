#include <iostream>
#include <vector>
#include <functional>
#include <utility>
#include <string>
#include <algorithm>
#include <stdarg.h>

#define MP(a) make_pair<int,a>

using namespace std;

template <typename F>
class FSM{//FiniteStateMachine
    private:
        int state = 0;
        vector<vector<F>> vec;
    public:
    // konstruktor 
    FSM(int n, ...){
        va_list va;
        va_start(va,n);
        for(int q=0; q<n; q++){
            typedef pair<int,F> PAIR;
            auto pr = va_arg(va, PAIR);
            while(vec.size()<=pr.first){
                vector<F> tmp;
                vec.push_back(tmp);
            }
            vec[pr.first].push_back(pr.second);
        }
        va_end(va);
    }

    // operator wywołania funkcji
    void operator() (char c) {
        for(int i=0; i<vec[state].size(); i++){
            int tmp = vec[state][i](c);
            if(tmp != state){
                state = tmp;
                break;
            }
        }
    }
    void getState(){
        cout << state << "\n";
    }
}; 

int main(int argc, char**argv){
    typedef function<int(char)> FUN;

    for(int i=1; i<argc; i++){
        string s = argv[i];

        // liczba podzielna przez 3
        // auto f = for_each(s.begin(), s.end(),FSM<FUN>(9,
        //                                             MP(FUN)(0,[](char c){int tmp = (c-'0')%3; if(tmp==0) return 0; else return 0;}),
        //                                             MP(FUN)(0,[](char c){int tmp = (c-'0')%3; if(tmp==1) return 1; else return 0;}),
        //                                             MP(FUN)(0,[](char c){int tmp = (c-'0')%3; if(tmp==2) return 2; else return 0;}),
        //                                             MP(FUN)(1,[](char c){int tmp = (c-'0')%3; if(tmp==0) return 1; else return 1;}),
        //                                             MP(FUN)(1,[](char c){int tmp = (c-'0')%3; if(tmp==1) return 2; else return 1;}),
        //                                             MP(FUN)(1,[](char c){int tmp = (c-'0')%3; if(tmp==2) return 0; else return 1;}),
        //                                             MP(FUN)(2,[](char c){int tmp = (c-'0')%3; if(tmp==0) return 2; else return 2;}),
        //                                             MP(FUN)(2,[](char c){int tmp = (c-'0')%3; if(tmp==1) return 0; else return 2;}),
        //                                             MP(FUN)(2,[](char c){int tmp = (c-'0')%3; if(tmp==2) return 1; else return 2;})
        //                                             ));

        // liczba binarna
        auto f = for_each(s.begin(), s.end(),FSM<FUN>(2,
                                                    MP(FUN)(0,[](char c){int tmp = c-'0'; return tmp;}),
                                                    MP(FUN)(1,[](char c){int tmp = c-'0'; return !tmp;})
                                                    ));

        f.getState();
    }
}