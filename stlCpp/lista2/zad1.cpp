#include <iostream>
#include <memory>
 
using namespace std;

class counter{
    public:
    uint64_t c = 1;
    virtual ~counter(){
        cout << c << "\n";
    }
};

void recInc(unique_ptr<counter[]> arr, int n, int m){
    if(!m)
        return;
    arr[rand()%n].c+=m;
    recInc(move(arr),n,m-1);    
}

int main(){
    srand(time(0));
    int n = 10, m = 7;
    // cin >> n >> m;
    unique_ptr<counter[]> arr (new counter[n]);
    recInc(move(arr),n,m);
}