#include <stdio.h>
#include <queue>
#include <algorithm>
#include <math.h>

using namespace std;

priority_queue<int> q;

//binsearch po wyniku
//strzelamy od 1 n*m wynik i sprawdzamy ile w kazdym wierszu 
//jest liczb mniejszych od tego wyniku
//jak trafimy jakas wartosc, musimy jeszcze sprawdzic, czy 
//taka wartosc istnieje w tablicy i wypisujemy odpowiednie

int main(){
	int n, m, k;
	scanf("%d %d %d", &n, &m, &k);
	
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++)
			q.push(-i*j);
	
	while(--k)
		q.pop();

	printf("%d\n",-q.top());

}
