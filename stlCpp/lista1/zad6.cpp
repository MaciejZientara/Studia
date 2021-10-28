#include <iostream>
#include <math.h>

using namespace std;

//g++ -std=c++17 -o zad6 zad6.cpp

void solve(double a, double b, double c){
    if(double delta = b*b - 4.0*a*c; delta > 0.0){
        printf("%lf %lf\n", (-b-sqrt(delta))/(2.0*a),(-b+sqrt(delta))/(2.0*a));
    }
    else{
        if(delta == 0.0)
            printf("%lf\n", -b/(2.0*a));
        else
            printf("none\n");
    }

}

int main(){
    double a,b,c;
    cin >> a >> b >> c;
    solve(a,b,c);
}

// 1 -2 1
// 1 5 6
// -1 1 -5