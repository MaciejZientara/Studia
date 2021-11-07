#include <iostream>
#include <typeinfo>
#include <type_traits>

using namespace std;

template<typename From, typename To>
void copy(From a, To &b){
    if constexpr(is_pointer_v<From>){
        if constexpr(!is_convertible<remove_pointer_t<From>,To>::value){
            cout << "cannot convert " << typeid(remove_pointer_t<From>).name() << " to " << typeid(To*).name() << "\n";
            return;
        }
        else{
            b = (To)*a;
            return;
        }
    }
    else{
        if constexpr(!is_convertible<From,To>::value){
            cout << "cannot convert " << typeid(From).name() << " to " << typeid(To).name() << "\n";
            return;
        }
        else{
            b = (To)a;
            return;
        }
    }
}

// g++ -std=c++17 -o zad4 zad4.cpp

int main(){
    int a = 25, b = 0;
    copy(a,b);
    cout << b << "\n";
    float c = 0.0;
    copy(&b,c);
    cout << c << "\n";
    string s;
    copy(&c,s);
    copy(s,c);
    cout << s << "\n";
    char d = 'a';
    copy(&d,b);
    cout << b << "\n";
}