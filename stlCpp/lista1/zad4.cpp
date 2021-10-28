#include <iostream>
#include <string>

using namespace std;

enum class name : uint16_t {
    Sarah = 1,
    Martha = 3,
    Max = 2,
    Tom = 4
};

void print(string info, name n){
    cout << info << ", ";
    if(static_cast<uint16_t>(n)%2 == 1)
        cout << "Miss ";
    else
        cout << "Mister ";
    switch (static_cast<uint16_t>(n))
    {
    case 1: cout << "Sarah\n"; break;
    case 3: cout << "Martha\n"; break;
    case 2: cout << "Max\n"; break;
    case 4: cout << "Tom\n"; break;
    
    default:
        break;
    }
}

int main(){
    print("Have a safe trip", name::Sarah);
    print("Welcome home", name::Tom);
}
