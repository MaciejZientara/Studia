#include <stdio.h>

using namespace std;

int ret=0, mn=1000;

int main(){
	int n;
	scanf("%d", &n);
	for(int i=0; i<n; i++){
		int kg, cost;
		scanf("%d %d", &kg, &cost);
		if(cost<mn)
			mn = cost;
		ret += kg*mn;
	}
	printf("%d\n", ret);
}
