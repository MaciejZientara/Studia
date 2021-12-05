#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int main(){
    vector<int> v{2,2,2,2,2,4,4,3,1,1,1,1,1,5,5,5,4,3,3,0};
    // vector<int> v{1,1,3,2,4,3,4,2,5,2,5,2,2,5,4,3,5,3,1,5,1,2,4,2,2,2,4,1,5,3,1};

    int mx = -1;
    map<int,int> m;

    for_each(v.begin(),v.end(),[&](auto x){
        if(m.find(x)==m.end()){
            int tmp = count(v.begin(),v.end(),x);
            m.insert(make_pair(x,tmp));
            mx = max(mx,tmp);
        }
    });

    for_each(m.begin(),m.end(),[mx](pair<int,int>p){
        if(p.second == mx)
            cout << "(" << p.first << "," << p.second << ") ";
    });
    cout << "\n";
}
