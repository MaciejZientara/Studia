#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;

#define N 1000

int main(){
    auto gen = default_random_engine(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<> d1(0,N);
    binomial_distribution<> d2(N,0.5);
    normal_distribution<> d3(N,1.0);

    ofstream myfile;
    while(!myfile.is_open())
        myfile.open("zad3results.txt");

    myfile << "uniform,binominal,normal\n";
    for(int i=0; i<N; i++){
        myfile << d1(gen) << "," << d2(gen) << "," << d3(gen) << "\n";
        gen = default_random_engine(chrono::system_clock::now().time_since_epoch().count());
    }

    myfile.close();
}