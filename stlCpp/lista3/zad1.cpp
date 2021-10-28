#include <iostream>
#include <math.h>

using namespace std;

int main(){
    long long int x;
    cout << "long long int ma " << sizeof(x)*8 << " bity\n";
    x = 0x8000000000000000;
    cout << "min long long int = " << x << "\n";
    x--;
    cout << "max long long int = " << x << "\n";
    cout << "liczba ta ma " << ceil(log(x)/log(10)) << " cyfr\n";
}
