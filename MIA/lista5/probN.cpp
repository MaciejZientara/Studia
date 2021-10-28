#include <stdio.h>

using namespace std;

int main(){
	int T;
	scanf("%d",&T);
	while(T--){
		int tab[1001] = {0};
		int V;
		scanf("%d",&V);
		while(V--){
			int tmp;
			scanf("%d",&tmp);
			tab[tmp]++;
		}
		int mx = -1;
		int val;
		
		for(int i=1; i<1001; i++)
			if(tab[i]>mx){
				val = i;
				mx = tab[i];
			}
		printf("%d\n",val);
	}
}
