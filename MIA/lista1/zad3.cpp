#include <stdio.h>
#include <algorithm>

#define MAX 1000000007

using namespace std;

int n;

int findRadius(int i, int j){
	return min(min(1+i,1+j),min(n-i,n-j));
}

int main(){
	scanf("%d", &n);

	int tab[n][n];
	for(int i=0; i<n; i++)
		for(int j=-1; j<n; j++){
			char tmp;
			scanf("%c", &tmp);
			if(tmp!='\n')
				tab[i][j] = tmp - '0';
		}

	long long mx=-1;

	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++){
			int r = findRadius(i,j);
			long long P = tab[i][j], X = tab[i][j]; 
			if(P>mx)
				mx=P;
			if(X>mx)
				mx=X;
			for(int q=1; q<r; q++){
				if(P!=0){
					P*=tab[i+q][j];
					P*=tab[i][j+q];
					P*=tab[i-q][j];
					P*=tab[i][j-q];
				}
				if(X!=0){
					X*=tab[i+q][j+q];
					X*=tab[i+q][j-q];
					X*=tab[i-q][j+q];
					X*=tab[i-q][j-q];
				}
				if(P>mx)
					mx=P;
				if(X>mx)
					mx=X;
			}
		}

	printf("%lld\n",mx%MAX);
	

}
