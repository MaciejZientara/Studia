#include <stdio.h>
#include <algorithm>
#include <math.h>

using namespace std;

int n;
int mxA, mxB;

void rec(int i, int A, int B, int tab[]){
	if(A>B)
		rec(i,B,A,tab);
	else{
		if(i<0){
			mxA = A;
			mxB = B;
			return;
		}
		rec(i-1,A+tab[i],B,tab);
	}
}

int main(){
	scanf("%d",&n);
	int tab[n];
	for(int i=0; i<n; i++)
		scanf("%d",&tab[i]);

	rec(n-1,0,0,tab);

	printf("%d %d\n", mxA, mxB);
}
