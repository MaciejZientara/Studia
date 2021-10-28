#include <stdio.h>
#include <algorithm>
#include <math.h>

using namespace std;

int main(){
	int T;
	scanf("%d",&T);
	while(T--){
		int n;
		scanf("%d",&n);
		double w[n], c[n];
		for(int i=0; i<n; i++)
			scanf("%lf %lf",&w[i],&c[i]);

		int tab[n];
		for(int i=0; i<n; i++)
			tab[i]=1;
		tab[n-1] = 1;
		for(int j=n-2; j>=0; j--)
			for(int i=j+1; i<n; i++)
				if(w[j]<w[i] && c[j]>c[i])
					tab[j] = max(tab[j],tab[i]+1);

		int mx = -1;
		for(int i=0; i<n; i++)
			if(tab[i]>mx)
				mx = tab[i];
		printf("%d\n",mx);
	}
}
