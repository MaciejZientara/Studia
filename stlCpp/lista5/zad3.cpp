#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

template <typename T>
class autoVec{
    private:
    vector<T> vec;
    public:
    T& operator[] (int i){
        while(vec.size()<=i){
            T tmp = 0;
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
    autoVec<bool> vertices;//moze byc wierzchołek bez krawędzi
    autoVec<autoVec<int>> edges;
    public:
    void addVert(int a){
        vertices[a] = true;//true - wierzchołek istnieje
    }
    void rmVert(int a){
        vertices[a] = false;//false - wierzchołek nie istnieje
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
    bool BFS(int start, int end){                                                           //TO FINISH, if vertices == false, don't consider this vertex!!!!
        int N = vertices.size();

        bool visited[N];
        for(int i=0; i<N; i++)
            visited[i] = false;

        list<int> ls;                                            //#include <list>, int zamiast T...

        visited[start] = true;                                              //start nie musi byc intem... moze jednak T -> int
        ls.push_back(start);

        while(!ls.empty())
        {
            start = ls.front();
            ls.pop_front();
            if(start == end)
                return true;//znaleziono scieżkę

            for_each(edges[start].begin(),edges[start].end(),
            [&](int elem){
                if(!visited[elem]){
                    visited[elem] = true;
                    ls.push_back(elem);
                }
            });
        }
        return false;//nie znaleziono ścieżki
    }
    bool isPath(int a, int b){
        if(vertices[a]==false || vertices[b]==false)
            return false;
        return BFS(a,b);
    }
};

int main(){

}
