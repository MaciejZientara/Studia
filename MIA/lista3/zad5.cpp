#include <iostream>
#include <math.h>

#define MAX 100000000000000ll

using namespace std;

long long n, k;

int length(long long a){
	return 1+(int)(log(a)/log(10));
}

bool wavy(long long a){
	int l = length(a);
	int tab[l];
	long long tmp = a;
	for(int i=l-1; i>=0; i--){
		tab[i]=tmp%10;
		tmp/=10;
	}
	bool ret = true;
	for(int i=1; i<l-1 && ret; i++){
		ret = (((tab[i-1]>tab[i]) && (tab[i+1]>tab[i])) || 
				((tab[i-1]<tab[i]) && (tab[i+1]<tab[i])));
	}

	return ret;
}

void findwavy(){
	if(n>MAX/k)
		cout << -1 << "\n";
	long long iter=0, tmp=0, i=0;
	while(iter<k){
		i++;
		tmp+=n;
		if(tmp>MAX){
			cout << -1 << "\n";
			return;
		}
		if(wavy(tmp))
			iter++;
		if(i>n && iter==0){
			cout << "-1\n";
			return;
		}
	}
	
	cout << tmp << "\n";
}

int main(){
	cin >> n >> k;
	findwavy();
}
