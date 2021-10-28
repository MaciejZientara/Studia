#include <stdio.h>
#include <vector>

using namespace std;

vector < vector <int> > v;
bool *visited;
int cycle = 0;

void DFS(int w, int p){
    visited[w] = true;
    for(int i=0; i<v[w].size(); i++){
        int n = v[w][i];
        if(n!=p){
            if(visited[n] == false)
                DFS(n,w);
            else
                cycle++;
        }
    }
}

int main(){
    int n,m;
    scanf("%d %d", &n, &m);

    for(int i=0; i<=n; i++){
        vector <int> tmp;
        v.push_back(tmp);
    }

    visited = new bool[n+1];
    for(int i=0; i<=n; i++)
        visited[i]=false;
    

    for(int i=0; i<m; i++){
        int a,b;
        scanf("%d %d", &a, &b);
        v[a].push_back(b);
        v[b].push_back(a);
    }

    DFS(1,0);

    for(int i=1; i<=n; i++)
        if(visited[i]==false)
            cycle=0;

    if(cycle==2)
        printf("FHTAGN!\n");
    else
        printf("NO\n");

    delete[] visited;
}