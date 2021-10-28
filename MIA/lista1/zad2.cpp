#include <stdio.h>

using namespace std;

int main(){
	int n, m;
	scanf("%d %d", &n, &m);
	
	char tab[n][m];
	int row[n] = {0}, collumn[m] = {0};

	int amount=0;

	for(int i=0; i<n; i++)
		for(int j=-1; j<m; j++){
			char tmp;
			scanf("%c", &tmp);
			if(tmp == '*'){
				amount++;
				row[i]++;
				collumn[j]++;
			}
			if(tmp != '\n')
				tab[i][j] = tmp;
		}	

	for(int i=0; i<n; i++)
		for(int j=0; j<m; j++){
			int tmp = row[i] + collumn[j];
			if(tab[i][j]=='*')
				tmp--;
			if(tmp==amount){
				printf("YES\n%d %d\n",i+1,j+1);
				return 0;
			}
		}

	printf("NO\n");
}
