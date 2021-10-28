#include <stdio.h>
#include <vector>

using namespace std;

bool *visited;

vector < vector <int> > lang;
vector < vector <int> > v;

void DFS(int x){
    visited[x] = true;

    for(int i=0; i<v[x].size(); i++)
        if(visited[v[x][i]]==false)
            DFS(v[x][i]);
}

int main(){
    bool onlyZero = true;
    int n,m;
    scanf("%d %d", &n, &m);
    for(int i=0; i<=m; i++){
        vector <int> tmp;
        lang.push_back(tmp);
    }
    for(int i=1; i<=n; i++){
        int k;
        scanf("%d",&k);
        if(k!=0)
            onlyZero = false;
        for(int j=0; j<k; j++){
            int tmp;
            scanf("%d", &tmp);
            lang[tmp].push_back(i);
        }
    }
    // printf("after input\n");

    for(int i=0; i<=n; i++){
        vector <int> tmp;
        v.push_back(tmp);
    }
    for(int i=1; i<=m; i++){
        // printf("size = %ld\n", lang[i].size()-1);
        for(int j=1; j<(int)lang[i].size(); j++){
            // printf("i=%d j=%d\n",i,j);
            // printf("ij=%d ij+1=%d\n",lang[i][j],lang[i][j+1]);
            v[lang[i][0]].push_back(lang[i][j]);
            v[lang[i][j]].push_back(lang[i][0]);
        }
    }
    // printf("after v\n");

    visited = new bool[n+1];
    for(int i=0; i<=n; i++)
        visited[i] = false;

    int res = 0;
    for(int i=1; i<=n; i++){
        if(visited[i]==false){
            res++;
            // printf("res+ dla %d\n",i);
            DFS(i);
        }
    }

    if(onlyZero)
        printf("%d\n",n);
    else
        printf("%d\n",res-1);


}
/*
jezeli kazdy zna 0 jezykow
odp n

robimy graf po samych osobach (jezeli 2 osoby maja chociaz jeden wspolny jezyk to jest miedzy nimi krawedz)
liczymy (np. dfs) ilosc spojnych skladwych, dla k skladowych odp to k-1
*/