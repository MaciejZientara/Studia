#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <type_traits>

using namespace std;

template <typename T>
class autoVec{
    private:
    vector<T> vec;
    public:
    T& operator[] (int i){
        while(vec.size()<=i){
            T tmp;
            if constexpr(is_convertible<T,int>::value)
                tmp = 0;
            vec.push_back(tmp);
        }
        return vec[i];
    }
    auto begin(){
        return vec.begin();
    }
    auto end(){
        return vec.end();
    }
    auto size(){
        return vec.size();
    }
};

class graph{
    private:
    autoVec<int> vertices;//moze byc wierzchołek bez krawędzi, int zamiast bool - vector<bool> dziala specyficznie
    autoVec<autoVec<int>> edges;
    public:
    void addVert(int a){
        vertices[a] = true;//true - wierzchołek istnieje
    }
    void rmVert(int a){
        vertices[a] = false;//false - wierzchołek nie istnieje
        int len = edges.size();
        for(int i=0; i<len; i++)
            edges[i][a] = 0;
        len = edges[a].size();
        for(int i=0; i<len; i++)
            edges[a][i] = 0;
    }
    void addEdge(int a, int b, int weight){
        vertices[a] = true;
        vertices[b] = true;
        edges[a][b] = weight;
    }
    void rmEdge(int a, int b){
        edges[a][b] = 0;//waga 0 - brak krawędzi
    }
    void chEdge(int a, int b, int weight){
        if(vertices[a]==false || vertices[b]==false)
            return;
        edges[a][b] = weight;
    }
    bool BFS(int start, int end){
        int N = vertices.size();

        bool visited[N];
        for(int i=0; i<N; i++)
            visited[i] = !vertices[i];// if vertices == false, don't consider this vertex

        deque<int> que;

        visited[start] = true;
        que.push_back(start);

        while(!que.empty())
        {
            start = que.front();
            que.pop_front();
            if(start == end)
                return true;//znaleziono scieżkę

            int len = edges[start].size();
            for(int i=0; i<len; i++){
                if(edges[start][i]>0 && !visited[i]){//jest krawedz i nieodwiedzony wierzcholek
                    visited[i] = true;
                    que.push_back(i);
                }
            }
        }
        return false;//nie znaleziono ścieżki
    }
    bool isPath(int a, int b){
        if(vertices[a]==false || vertices[b]==false)
            return false;
        return BFS(a,b);
    }
    void debug(){
        int len = vertices.size();
        for(int i=0; i<len; i++)
            cout << "(" << i << ", " << vertices[i] << ") ";
        cout << "\n";

        len = edges.size();
        for(int i=0; i<len; i++){
            for(auto e : edges[i])
                cout << e << " ";
            cout << "\n";
        }
    }
};

int main(){
    graph g;
    g.addEdge(1,2,1);
    g.addEdge(2,1,1);
    g.addEdge(1,3,1);
    g.addEdge(3,1,1);
    g.addEdge(2,3,1);
    g.addEdge(3,2,1);
    // g.debug();
    cout << (g.isPath(1,2) ? "Y" : "N") << "\n";
    g.rmVert(1);
    // g.debug();
    g.addEdge(1,4,1);
    g.addEdge(4,1,1);
    // g.debug();
    cout << (g.isPath(1,2) ? "Y" : "N") << "\n";
    
}
