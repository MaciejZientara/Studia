#include <stdio.h>

using namespace std;

int nextDistinct(int n){
	int tab[10];
	while(true){
		n++;
		for(int i =0; i<10; i++)
			tab[i]=0;
		
		int tmp = n;
		while(tmp > 0){
			tab[tmp%10]++;
			tmp/=10;
		}
		
		bool isFine = true;
		for(int i =0; i<10; i++)
			if(tab[i]>1)
				isFine = false;
		if(isFine)
			break;
	}
	return n;
}

int main(){
	int n;
	scanf("%d",&n);
	printf("%d\n",nextDistinct(n));
}
