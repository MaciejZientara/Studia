#include <stdio.h>
#include <algorithm>
#include <math.h>

using namespace std;

void funky(int n){
	for(int i=1; i<=sqrt(n); i++){
		int tmp = n - i*(i+1);
		if(tmp<=0)
			break;
	
		int begin = 1, end = (tmp/2)+1;		
		while(begin<end-1){
			int s = (begin+end)/2;
			if((s+1)>tmp/s)
				end = s;
			else
				begin = s;
		}
		
			//printf("%d*%d + %d*%d\n",i,i+1,end,end+1);
		if(tmp==end*(end+1) || tmp==begin*(begin+1)){
			//printf("%d*%d + %d*%d\n",i,i+1,end,end+1);
			printf("YES\n");
			return;
		}
	}
	printf("NO\n");
}

int main(){
	int n;
	scanf("%d",&n);
	funky(2*n);
}
