#include <iostream>
#include <ratio>

using namespace std;

typedef ratio<1,1> har1;
typedef ratio_add< har1,ratio<1,2>>har2;
typedef ratio_add< har2,ratio<1,3>>har3;
typedef ratio_add< har3,ratio<1,4>>har4;
typedef ratio_add< har4,ratio<1,5>>har5;
typedef ratio_add< har5,ratio<1,6>>har6;
typedef ratio_add< har6,ratio<1,7>>har7;

int main(){
    
    cout << har1::num << " / " << har1::den << "\n";
    cout << har2::num << " / " << har2::den << "\n";
    cout << har3::num << " / " << har3::den << "\n";
    cout << har4::num << " / " << har4::den << "\n";
    cout << har5::num << " / " << har5::den << "\n";
    cout << har6::num << " / " << har6::den << "\n";
    cout << har7::num << " / " << har7::den << "\n";

}