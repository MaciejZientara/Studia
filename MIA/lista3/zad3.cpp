#include <stdio.h>
#include <algorithm>
#include <math.h>

using namespace std;


int main(){
	int n;
	scanf("%d", &n);
	int tab[n];
	bool fine[n];
	for(int i=0; i<n; i++)
		scanf("%d", &tab[i]);
	
	fine[0] = (tab[0]<tab[1]);
	for(int i=1; i<n-1; i++){
		if(n%2==0)
			fine[i] = (tab[i]<tab[i+1]);
		else
			fine[i] = (tab[i]>tab[i+1]);
	}
	
	if((n-1)%2==0)
		fine[n-1] = (tab[n-2]>tab[n-1]);
	else
		fine[n-1] = (tab[n-2]<tab[n-1]);

	/*
	int ret = 0;
	for(int i=0; i<n; i++)
		if(fine[i]==0)
			ret++;
	
	printf("%d\n",ret);
	*/
	for(int i=0; i<n; i++)
		printf("%d ", fine[i]);
	printf("\n");
}
