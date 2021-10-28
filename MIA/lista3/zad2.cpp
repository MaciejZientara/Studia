#include <stdio.h>
#include <algorithm>
#include <vector>

using namespace std;

class node{
	public:
	int v,a,b;
	node(int V, int A, int B){
		v=V;
		a=A;
		b=B;
	}
};

bool cmpNode(const node &a, const node &b){
	return a.v > b.v;
}


int main(){
	int n;
	scanf("%d", &n);
	
	vector<node> p;

	for(int i=0; i<(2*n-1); i++)
		for(int j=0; j<=i; j++){
			int tmp;
			scanf("%d", &tmp);
			node n(tmp,i+1,j);
			p.push_back(n);
		}

	sort(p.begin(),p.end(),cmpNode);	
	
	int tab[2*n] = {0};
	
	for(int i=0; i<p.size(); i++){		
		if(!tab[p[i].a] && !tab[p[i].b]){
			tab[p[i].a]=p[i].b+1;
			tab[p[i].b]=p[i].a+1;
		}
	}

	for(int i=0; i<2*n; i++)
		printf("%d ", tab[i]);
	printf("\n");

}





