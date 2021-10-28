#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class triplet{
    public:
    int a,b,c;
    triplet(int A, int B, int C){
        a=A;b=B;c=C;
    }
};

vector <long long> original;
vector <triplet> oper;

int main(){
    int n,m,k;
    cin >> n >> m >> k;

    original.push_back(0);//index od 1
    for(int i=0; i<n; i++){
        long long tmp;
        cin >> tmp;
        original.push_back(tmp);
    }

    oper.push_back(triplet(0,0,0));//index od 1
    for(int i=0; i<m; i++){
        int a,b,c;
        cin >> a >> b >> c;
        triplet tmp(a,b,c);
        oper.push_back(tmp);
    }

    // for(int i=0; i<=m; i++)
    //     cout << "oper " << oper[i].a << " " << oper[i].b << " " << oper[i].c << "\n"; 

    int beg[m+1] = {0}, end[m+1] = {0};
    for(int i=0; i<k; i++){
        int b,e;
        cin >> b >> e;
        beg[b]++;
        end[e]++;
    }

    long long sum=0;
    long long Lbeg[n+1] = {0}, Lend[n+1] = {0};
    for(int i=1; i<=m; i++){
        sum+= beg[i] - end[i-1];
        if(sum){
            long long val = (long long)oper[i].c * sum;
            Lbeg[oper[i].a]+= val;
            Lend[oper[i].b]+= val;
        }
    }

    long long Lsum=0;
    for(int i=1; i<=n; i++){
        Lsum+= Lbeg[i]-Lend[i-1];
        cout << (long long)original[i]+Lsum << " ";
    }
    cout << "\n";


/*
    for(int q=oper[i].a; q<=oper[i].b; q++)
        original[q]+=(long long)oper[i].c*(long long)sum;



    int ile[m+1] = {0};//ile razy dana oper

    for(int i=0; i<k; i++){
        int b,e;
        cin >> b >> e;
        for(int j=b; j<=e; j++){
            ile[j]++;
        }
    }

    for(int j=1; j<=m; j++)
        if(ile[j])
            for(int q=oper[j].a; q<=oper[j].b; q++)
                original[q]+=(long long)oper[j].c*(long long)ile[j];

    for(int i=1; i<=n; i++)
        cout << original[i] << " ";
    cout << "\n";
*/     
}