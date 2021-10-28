#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>

using namespace std;

#define MAX 1000000005

int main(){
	int n;
	scanf("%d", &n);
	if(n<2){
		printf("-1\n");
		return 0;
	}
	string s;
	cin >> s;


	int mn = MAX, x;
	scanf("%d", &x);
	for(int i=1; i<n; i++){
		int tmp;
		scanf("%d", &tmp);
		if(s[i]=='L'){
			if(s[i-1]=='R')
				mn = min(mn, (tmp-x)/2);
		}
		else
			x = tmp;
	}

	if(mn == MAX)
		printf("-1\n");
	else
		printf("%d\n",mn);

}
