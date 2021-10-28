#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

vector < int > v;
int month[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

int main(){
	int n;
	scanf("%d", &n);
	for(int i=0; i<n; i++){
		string name, date;
		cin >> name >> date;
		int m =10*(date[0]-'0')+(date[1]-'0'), d =10*(date[3]-'0')+(date[4]-'0');
		int sum=0;
		for(int j=1; j<m; j++)
			sum+=month[j];
		sum+=d;
		v.push_back(sum);
	}

	sort(v.begin(),v.end());

	// for(int i=0; i<n; i++)
	// 	cout << v[i] << "\n";

	int date=0;
	for(int j=1; j<10; j++)
		date+=month[j];
	date+=27;

	int mx=-1, dist=500, res;

	for(int i=0; i<n; i++){
		int tmp = (v[(i+1)%n] - v[i] + 365)%365;
		int td = (v[(i+1)%n] - date + 365)%365;

		if(td<=1)
			td+=364;

		if(mx==tmp){
			if(td<dist){
				dist = td;
				res = v[(i+1)%n];
				res--;
			}
		}
		if(mx<tmp){
			dist = td;
			mx = tmp;
			res = v[(i+1)%n];
			res--;
		}
	}

	// cout << "res1 " << res << "\n";
	if(res<=0)
		res=365;
	// cout << "res2 " << res << "\n";


	int m=0;
	while(res>month[m])
		res-=month[m++];

	if(m<10)
		cout << "0";
	cout << m << "-";
	if(res<10)
		cout << "0";
	cout << res << "\n";
}
/*

3
Henk 01-09
Roos 09-20
Pietje 11-11

>09-19

16
Henk 01-09
Luc 12-31
Jan 03-22
Roos 09-20
Pietje 11-11
Anne 02-28
Pierre 09-25
Dan 12-15
Lieze 11-17
Charlotte 05-01
Lenny 08-02
Marc 04-25
Martha 06-12
John 03-26
Matthew 01-20
John 01-20

>08-01

3
JohnIII 04-29
JohnVI 10-28
JohnIIX 04-28

>04-27

3
CharlesII 04-30
CharlesV 10-29
CharlesVII 04-29

>10-28

2
a 01-01
b 01-02

> 12-31

2
a 01-02
b 01-03

> 01-01

*/