#include <stdio.h>

using namespace std;

int main(){
	int n, k;
	scanf("%d %d", &n, &k);
	int tab[n];
	for(int i=0; i<n; i++)
		scanf("%d",&tab[i]);
	int sum[n];
	sum[n-1]=tab[n-1];
	for(int i=n-2; i>=0; i--)
		sum[i]=sum[i+1]+tab[i];

	int mn = 1000000000, indx;
	for(int i=0; i<=(n-k); i++){
		int tmp = sum[i]-sum[i+k-1]+tab[i+k-1];
		if(tmp<mn){
			mn = tmp;
			indx = i;
		}
	}
	printf("%d\n",indx+1);
}
