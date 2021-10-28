#include <stdio.h>
#include <algorithm>
#include <math.h>

using namespace std;

class arr{
	public:
	int val, index;
};

bool cmpArr(const arr &a, const arr &b){
	return a.val < b.val;
}


int main(){
	int n, m;
	scanf("%d %d",&n,&m);
	int A[n];
	arr B[m];
	for(int i=0; i<n; i++)
		scanf("%d",&A[i]);
	for(int i=0; i<m; i++){
		scanf("%d",&B[i].val);
		B[i].index = i;
	}
	sort(A,A+n);
	sort(B,B+m,cmpArr);

	int iter=0;
	int tmp[m];

/*
	for(int i=0; i<n; i++)
		printf("A[%d]=%d\n",i,A[i]);
	
	for(int i=0;i<m;i++)
		printf("B[%d] = (%d,%d)\n",i,B[i].val,B[i].index);
*/

	//while(A[iter]<=B[0].val)
	//	iter++;
	//tmp[B[0].index]=iter;
	for(int i=0;i<m;i++){
		while(iter<n && A[iter]<=B[i].val)
			iter++;
		tmp[B[i].index]=iter;
	}
	for(int i=0; i<m; i++)
		printf("%d ",tmp[i]);
	printf("\n");
	
}
