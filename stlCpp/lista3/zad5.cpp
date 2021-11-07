#include <iostream>
#include <time.h>
#include <chrono>

using namespace std;

int N = 100;

void MatrixPower2(float **tab){
    auto tmp = new float*[N];
    for(int i=0; i<N; i++)
        tmp[i] = new float[N];


    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            tmp[i][j]=0.0;

    for(int i=0; i<N; i++)
        for(int k=0; k<N; k++)      //k before j to have lower
            for(int j=0; j<N; j++)  //cache miss ratio
                tmp[i][j] += tab[i][k] * tab[k][j];

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            tab[i][j] = tmp[i][j];
    
    for(int i=0; i<N; i++)
        delete [] tmp[i];
    delete [] tmp;
}

void FUNCTION(){
    auto tab = new float*[N];
    for(int i=0; i<N; i++)
        tab[i] = new float[N];

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            tab[i][j] = (float(rand())/float((RAND_MAX)) * 1.5)+0.5;// [0.0, 1.0] -> [0.0, 1.5] -> [0.5, 2.0]

    MatrixPower2(tab);

    for(int i=0; i<N; i++)
        delete [] tab[i];
    delete [] tab;
}

void TimeMeasureSteady(int n, int t){
    N = n;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for(int i=0; i<t; i++)
        FUNCTION();
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    
    cout << "N = " << n << ", #test = " << t << ", average time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count()/t << "[µs]" << endl;
}

void TimeMeasureHigh(int n, int t){
    N = n;
    chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now();
    for(int i=0; i<t; i++)
        FUNCTION();
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

    chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(end - begin);

    cout << "N = " << n << ", #test = " << t << ", average time = " << time_span.count()/t << "[s]" << endl;
}

int main(){
    srand(time(0));

    TimeMeasureSteady(100,100);
    cout << "\n";
    TimeMeasureSteady(1000,10);
    cout << "\n";
    TimeMeasureHigh(10000,1);
    cout << "\n";
}

/*
N = 100, #test = 100, average time = 5390[µs]

N = 1000, #test = 10, average time = 5385321[µs]

N = 10000, #test = 1, average time = 5184.28[s]
*/