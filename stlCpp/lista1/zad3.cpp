#include <iostream>
#include <string>
#include <set>

using namespace std;

// template<typename T> using vec = vector<T>;

using st = set<string>;

int main(){
    st zbior {"alfa","beta","gamma","delta"};
    for(auto z : zbior)
        cout << z << "\n";
}
