#include <stdio.h>
#include <algorithm>

using namespace std;

int main(){
	int n,k;
	scanf("%d %d", &n, &k);
	int tab[n];
	for(int i=0; i<n; i++)
		scanf("%d",&tab[i]);
	sort(tab,tab+n);
	
	int ret=1;
	if(n>1)
		for(int i=0; i<n-1; i++)
			if(tab[i]+tab[i+1]<=k)
				ret++;
	
	printf("%d\n",ret);

}
