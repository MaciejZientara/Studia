#include <iostream>
#include <functional>

using namespace std;

template <typename A, typename B, typename F, typename G, typename H>
auto compose_f_gx_hy(F f, G g, H h){
    return [f,g,h](A x, B y){return f(g(x),h(y));};
}

bool f(int X, int Y){
    return X > Y;
}

int g(int x){
    return x*x;
}

int h(int y){
    return y*2;
}

int main(){
    cout << compose_f_gx_hy<int,int>(f,g,h)(5,10) << "\n";
    cout << compose_f_gx_hy<int,int>(
                            [](int x, int y){return (x%y)==0;},
                            [](int x){return x*x;},
                            [](int y){return y*2;}
                            )(14,7) << "\n";
}
