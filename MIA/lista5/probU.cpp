#include <iostream>
#include <string>
#include <set>

using namespace std;

set<string> st;

void add(string s){
	set<string>::iterator it = st.find(s);
	if(it==st.end())
		st.insert(s);
	else
		st.erase(it);
}

int main(){
	int t;
	cin >> t;
	while(t--){
		st.clear();//
		int n;
		cin >> n;
		string tab[n];
		for(int i=0; i<n; i++)
			cin >> tab[i];
		int iter = 0;//1?
		for(int i=0; i<n; i++){
			iter++;
			string tmp;
			cin >> tmp;
			add(tmp);
			add(tab[i]);
		
			if(st.empty()){
				cout << iter << " ";
				iter = 0;//1?
			}
		}
		cout << "\n";
	}
}

