#include <stdio.h>

using namespace std;

int n;
int am[5005];
bool tab[5005];

int ret;
int firstEmpty=1;
int firstMulti=1;


void findEmpty(){
	for(int i=firstEmpty; i<=n; i++){
		if(tab[i]==false){
			firstEmpty=i;
			break;
		}
		if(i==n)
			firstEmpty = -1;
		}
}
void findMulti(){
	for(int i=firstMulti; i<=n; i++)
		if(am[i]>1){
			firstMulti=i;
			break;
		}
}


int main(){
	scanf("%d", &n);
	for(int i=1; i<=n; i++){
		int tmp;
		scanf("%d",&tmp);
		am[tmp]++;
		tab[tmp]=true;
	}

	while(firstEmpty!=-1){
		findEmpty();
		findMulti();
		tab[firstEmpty]=true;
		am[firstEmpty]++;
		am[firstMulti]--;
		ret++;
	}

	printf("%d\n",ret-1);


}
