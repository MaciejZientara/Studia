#include <iostream>
#include <typeinfo>
#include <cstring>
#include <iomanip> 

using namespace std;


int main(){
    float a=0.0f;
    double b=0.0;

    cout << "float " << a << "\n";
    cout << "double " << b << "\n";

    uint32_t A;
    uint64_t B;
    memcpy(&A,&a,sizeof(a));
    A|=1;
    memcpy(&a,&A,sizeof(A));
    
    memcpy(&B,&b,sizeof(b));
    B|=1;
    memcpy(&b,&B,sizeof(B));
    
    cout << "float " << a << "\n";
    cout << "double " << b << "\n";

    a=1.0;
    b=1.0;

    cout << "float " << a << "\n";
    cout << "double " << b << "\n";

    memcpy(&A,&a,sizeof(a));
    // printf("%x\n",A);
    A|=1;
    // printf("%x\n",A);
    memcpy(&a,&A,sizeof(A));
    
    memcpy(&B,&b,sizeof(b));
    // printf("%lx\n",B);
    B|=1;
    // printf("%lx\n",B);
    memcpy(&b,&B,sizeof(B));
    
    cout << "float " << setprecision(7) << a << "\n";
    cout << "double " << setprecision(14) << b << "\n";

    A = 0x7f7fffff;
    B = 0x7fefffffffffffff;
    memcpy(&a,&A,sizeof(A));
    memcpy(&b,&B,sizeof(B));

    cout << "max float " << a << "\n";
    cout << "max double " << b << "\n";
}