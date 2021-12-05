#include <iostream>
#include <random>
#include <chrono>

using namespace std;

template <typename T>
void MyShuffle(T tab[], int size){
    auto gen = default_random_engine(chrono::system_clock::now().time_since_epoch().count());
    for(int n = size-1; n>0; n--){//n to rozmiar tablicy do rozpatrzenia w każdym obrocie
        uniform_int_distribution<T> d(0,n);//potrzebujemy rozkładu jednostajnego na pozostałym przedziale 0 do n
        swap(tab[n], tab[d(gen)]);//zamieniamy ostatni element z losowym z pozostalego przedzialu
    }
}

int main(){
    int t[] = {0,1,2,3,4,5,6,7,8,9};
    MyShuffle(t, 10);
    for(auto x : t)
        cout << x << " ";
    cout << "\n";
    char l[] = "0123456789";
    MyShuffle(l, 10);
    for(auto x : l)
        cout << x << " ";
    cout << "\n";
}