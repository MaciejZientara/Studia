#include <iostream>
#include <functional>

using namespace std;

#define PK(a,b,c) po_kolei<c>(a,b)

template<typename T, typename F1, typename F2>
auto po_kolei(F1 f1, F2 f2){
    return [f1,f2](T x){f1(x);f2(x);};
}

void test(int &x){
    cout << "test " << x++ << " \n";
}

int main(){
    int x = 1;
    po_kolei<int&>(
        [](int &x){cout << x++ << " pierwsze\n";},
        po_kolei<int&>(
            [](int &x){cout <<  x++ << " drugie\n";},
            [](int &x){cout <<  x++ << " trzecie\n";}
        )
    )(x);

    auto lambda1 = [](int &x){cout << x++ << " pierwsze\n";};
    auto lambda2 = [](int &x){cout <<  x++ << " drugie\n";};
    auto lambda3 = [](int &x){cout <<  x++ << " trzecie\n";};

    PK(
        lambda1,
        PK(
            lambda2,
            lambda3,
            int&
        ),
        int&
    )(x);

    PK(PK(test,test,int&),PK(test,test,int&),int&)(x);
}

/*
#include <iostream>
#include <functional>

using namespace std;

#define PK(a,b,c) po_kolei<typeof(a),typeof(b),c>(a,b)

template<typename F1, typename F2, typename T>
auto po_kolei(F1 f1, F2 f2){
    return [f1,f2](T x){f1(x);f2(x);};
}

void test(int &x){
    cout << "test " << x++ << " \n";
}

int main(){
    int x = 1;
    po_kolei<function<void(int&)>,function<void(int&)>,int&>(
        [](int &x){cout << x++ << " pierwsze\n";},
        po_kolei<function<void(int&)>,function<void(int&)>,int&>(
            [](int &x){cout <<  x++ << " drugie\n";},
            [](int &x){cout <<  x++ << " trzecie\n";}
        )
    )(x);

    auto lambda1 = [](int &x){cout << x++ << " pierwsze\n";};
    auto lambda2 = [](int &x){cout <<  x++ << " drugie\n";};
    auto lambda3 = [](int &x){cout <<  x++ << " trzecie\n";};

    PK(
        lambda1,
        PK(
            lambda2,
            lambda3,
            int&
        ),
        int&
    )(x);

    PK(PK(test,test,int&),PK(test,test,int&),int&)(x);
}
*/