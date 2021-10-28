#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

vector < long long > original;

int main(){
    int n,m;
    cin >> n >> m;

    original.push_back(0);//index od 1
    for(int i=0; i<n; i++){
        long long tmp;
        cin >> tmp;
        original.push_back(tmp);
    }

    int beg[n+1] = {0}, end[n+1] = {0};
    for(int i=0; i<m; i++){
        int b,e;
        cin >> b >> e;
        beg[b]++;
        end[e]++;
    }

    // for(int i=0; i<=n; i++)
    //     cout << "beg = " << beg[i] << " end = " << end[i] << "\n";


    long long sums[n+1] = {0};
    for(int i=1; i<=n; i++)
        sums[i] = sums[i-1] + beg[i] - end[i-1];

    // for(int i=0; i<=n; i++)
    //     cout << sums[i] << " ";
    // cout << "\n";

    sort(sums+1,sums+n+1);
    sort(original.begin()+1,original.end());

    // for(int i=0; i<=n; i++)
    //     cout << "orig = " << original[i] << " sums = " << sums[i] << "\n";

    long long ret=0;
    for(int i=1; i<=n; i++)
        ret+= original[i] * sums[i];

    cout << ret << "\n";

}