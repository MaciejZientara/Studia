#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <iterator>

using namespace std;

int main(){
    vector <double> v;

    std::istream_iterator<double> eos;// end-of-stream iterator
    std::istream_iterator<double> it (std::cin);

    while(it != eos){
        v.push_back(*it);
        it++;
    }

    double sum = accumulate(v.begin(), v.end(), 0.0);
    double av = sum/v.size();

    double tmp = accumulate(v.begin(), v.end(), 0.0, [&](double &sm, double &cur){
        double dif = av-cur;
        return sm + (dif * dif);
    });

    cout << "srednia = " << av << ", odchylenie std = " << sqrt(tmp/v.size()) << "\n";
}

// 1.1 3.3 (ctrl + d)
// srednia = 2.2, odchylenie std = 1.1