#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;

vector <double> v;
set <int> s;
list <string> l;

void fill(){
    int amount = 100 + rand()%200;
    while(amount--){
        int tmp = rand()%1000;
        s.insert(tmp);
        v.push_back(tmp/13.0);
        l.push_back(to_string(tmp));
    }
}

template <typename T>
class PrintInRange{
    private:
        T a,b; // zakres
    public:
    // konstruktor 
    PrintInRange(T A, T B) : a(A), b(B){}
    // operator wywołania funkcji
    void operator() (T elem) const {
        if(elem > a && elem < b)
            cout << elem << " ";
    }
}; 

void partA(){
    cout << "\nPart A\n";
    cout << "vector<double>\n";
    for_each(v.begin(), v.end(),PrintInRange<double>(10.0,25.0));
    cout << "\n";
    cout << "set<int>\n";
    for_each(s.begin(), s.end(),PrintInRange<int>(10,25));
    cout << "\n";
    cout << "list<string>\n";
    for_each(l.begin(), l.end(),PrintInRange<string>("1","3"));
    cout << "\n";
}

template <typename T>
class PrintPK{
    private:
        int p,k,itr=0; // zakres
    public:
    // konstruktor 
    PrintPK(int P, int K) : p(P), k(K){}
    // operator wywołania funkcji
    void operator() (T elem) {
        if(p)
            p--;
        else{
            if(itr==0)
                cout << elem << " ";
            itr = (itr+1)%k;
        }
    }
}; 


void partB(){
    cout << "\nPart B\n";
    cout << "vector<double>\n";
    for_each(v.begin(), v.end(),PrintPK<double>(100,2));
    cout << "\n";
    cout << "set<int>\n";
    for_each(s.begin(), s.end(),PrintPK<int>(100,2));
    cout << "\n";
    cout << "list<string>\n";
    for_each(l.begin(), l.end(),PrintPK<string>(100,2));
    cout << "\n";
}

template <typename T>
class Average{
    private:
        T sum=0;
        int amount=0;
    public:
    // konstruktor 
    // Average(int P, int K) : p(P), k(K){}
    // operator wywołania funkcji
    void operator() (T elem) {
        amount++;
        sum+=elem;
    }
    void result(){
        cout << sum/(double)amount << "\n";
    }
}; 

void partC(){
    cout << "\nPart C\n";
    cout << "vector<double>\n";
    auto f1 = for_each(v.begin(), v.end(),Average<double>());
    f1.result();
    cout << "\n";
    cout << "set<int>\n";
    auto f2 = for_each(s.begin(), s.end(),Average<int>());
    f2.result();
    cout << "\n";
    // cout << "list<string>\n";
    // auto f3 = for_each(l.begin(), l.end(),Average<string>());
    // f3.result();
    // cout << "\n";
}

template <typename T, typename ITERATOR>
class MinMax{
    private:
        ITERATOR mn, mx, it, end;
    public:
    // konstruktor 
    MinMax(ITERATOR MN, ITERATOR MX, ITERATOR IT, ITERATOR END) : mn(MN), mx(MX), it(IT), end(END){}
    // operator wywołania funkcji
    void operator() (T elem) {
        T tmp = min(*mn,*it);
        if(tmp == *it)
            mn = it;
        tmp = max(*mx,*it);
        if(tmp == *it)
            mx = it;
        it = next(it);
    }
    void result(){//                                                TODO, powinienem zwrocic pare iteratorow
        cout << "min = " << *mn << " max = " << *mx << "\n";
    }
}; 

void partD(){
    cout << "\nPart D\n";
    cout << "vector<double>\n";
    auto f1 = for_each(v.begin(), v.end(),MinMax<double,typeof(v.begin())>(v.begin(),v.begin(),v.begin(),v.end()));
    f1.result();
    cout << "\n";
    cout << "set<int>\n";
    auto f2 = for_each(s.begin(), s.end(),MinMax<double,typeof(s.begin())>(s.begin(),s.begin(),s.begin(),s.end()));
    f2.result();
    cout << "\n";
    cout << "list<string>\n";
    auto f3 = for_each(l.begin(), l.end(),MinMax<double,typeof(l.begin())>(l.begin(),l.begin(),l.begin(),l.end()));
    f3.result();
    cout << "\n";
}

template <typename T>
class Concat{
    private:
        T res="";
    public:
    // operator wywołania funkcji
    void operator() (T elem) {
        res+=elem;
    }
    void result(){
        cout << res << "\n";
    }
}; 

void partE(){
    cout << "\nPart E\n";
    // cout << "vector<double>\n";
    // auto f1 = for_each(v.begin(), v.end(),Concat<double>());
    // f1.result();
    // cout << "\n";
    // cout << "set<int>\n";
    // auto f2 = for_each(s.begin(), s.end(),Concat<int>());
    // f2.result();
    // cout << "\n";
    cout << "list<string>\n";
    auto f3 = for_each(l.begin(), l.end(),Concat<string>());
    f3.result();
    cout << "\n";
}

int main(){
    srand(time(0));
    fill();
    partA();
    partB();
    partC();
    partD();
    partE();
}