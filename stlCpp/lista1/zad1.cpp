#include <iostream>

using namespace std;

bool trigraphsavailable(){
	// are trigraphs available??/
	return false;
	return true;
}

/*
    ??/ = \
    '\' w połączeniu z // czyli zakomentowaniem lini
    zakomentuje też następną linię kodu 

    po kompilacji:
    zad1.cpp:6:28: warning: trigraph ??/ ignored, use -trigraphs to enable [-Wtrigraphs]

    >> g++ -o zad1 zad1.cpp
    >> ./zad1
    0

    >> g++ -o zad1 zad1.cpp -trigraphs
    >> ./zad1
    1
*/


int main(){
    cout << trigraphsavailable() << "\n";
}

