#include <iostream>
#include <vector>

using namespace std;

auto Lucas(uint32_t n){
    if(n==0)
        return 2;
    if(n==1)
        return 1;
    return Lucas(n-1) + Lucas(n-2);
}

auto LucasMemoized(uint32_t n){
    static vector <auto> v = {2,1}; //jak dać do vector<auto>??
    if(n < v.size())
        return v[n];
    auto res = LucasMemoized(n-1) + LucasMemoized(n-2);
    v.push_back(res);
    return res;
}


int main(){
    int n;
    cin >> n;
    cout << Lucas(n) << "\n";
	//  cout << LucasMemoized(n) << "\n";
}
