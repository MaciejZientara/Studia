#include <iostream>
#include <ctime>

using namespace std;

//g++ -std=c++17 -o zad7 zad7.cpp

void printDate(time_t time){
    tm *date = localtime(&time);
    cout << date->tm_mday << " ";
    switch (int month = date->tm_mon; month)
    {
    case 0:
        cout << "styczeń ";
        break;
    case 1:
        cout << "luty ";
        break;
    case 2:
        cout << "marzec ";
        break;
    case 3:
        cout << "kwiecień ";
        break;
    case 4:
        cout << "maj ";
        break;
    case 5:
        cout << "czerwiec ";
        break;
    case 6:
        cout << "lipiec ";
        break;
    case 7:
        cout << "sierpień ";
        break;
    case 8:
        cout << "wrzesień ";
        break;
    case 9:
        cout << "październik ";
        break;
    case 10:
        cout << "listopad ";
        break;
    case 11:
        cout << "grudzień ";
        break;
    
    default:
        break;
    }
    cout << 1900+date->tm_year << "\n";
}

int main(){
    long int data = time(0);
    // cin >> data;
    printDate(data);
}