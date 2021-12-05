#include <iostream>
#include <deque>
#include <string>
#include <algorithm>

using namespace std;

class person{
    public:
    string name, lastname;
    int age, weight, height;
    person(string n, string l, int a, int w, int h) : 
    name(n),
    lastname(l),
    age(a),
    weight(w),
    height(h)
    {}
    double BMI(){
        double h = height/100.0;
        return weight/(h*h);
    }
};
std::ostream &operator<<(std::ostream &os, person const &p) { 
    return os << p.name << " " << p.lastname << ", age: " << p.age << ", weigh: " << p.weight << ", height: " << p.height;
}

int main(){
    deque<person> d{
        person("Adam",      "Adamski",         18,     70,    170),
        person("Bartek",    "Bartkowski",      20,    101,    169),
        person("Cecylia",   "Cecylska",        40,     55,    159),
        person("Damian",    "Damianski",       12,     57,    140),
        person("Eryk",      "Erykowski",       17,     74,    160),
        person("Franek",    "Frankowski",      55,     85,    177),
        person("Grzegorz",  "Grzegorski",      32,    120,    180),
        person("Hilda",     "Hildowska",       30,     75,    175),
        person("Ignacy",    "Ignacki",         21,     69,    171),
        person("Jan",       "Janowski",        18,     81,    166),
        person("Krystian",  "Krystianski",     27,     76,    182)
    };


    //partA
    cout << "\n\npartA\n";
    sort(d.begin(),d.end(),[](person A, person B){
        return A.BMI() < B.BMI();
    });
    for(person x : d)
        cout << x << ", BMI: " << x.BMI() << "\n";


    //partB
    cout <<"\n\npartB\n";
    transform(d.begin(),d.end(),d.begin(),[](person p){
        p.weight*=0.9;
        return p;
    });
    for(person x : d)
        cout << x << "\n";


    //partC
    cout <<"\n\npartC\n";
    cout << ">=100kg\n";
    for_each(d.begin(),d.end(),[](person p){if(p.weight>=100)cout << p << "\n";});
    cout << "<100kg\n";
    for_each(d.begin(),d.end(),[](person p){if(p.weight<100)cout << p << "\n";});


    //partD
    cout <<"\n\npartD\n";
    nth_element(d.begin(), d.begin()+(d.size()/2), d.end(), [](person A, person B){return A.height < B.height;});
    for(person x : d)
        cout << x << "\n";    


    //partE
    cout <<"\n\npartE\n";
    random_shuffle(d.begin(), d.begin()+5);
    swap_ranges(d.begin(), d.begin()+5, d.begin()+6);
    random_shuffle(d.begin(), d.begin()+5);
    for(person x : d)
        cout << x << "\n";    
        

    //partF
    cout <<"\n\npartF\n";
    cout << "min " << *min_element(d.begin(),d.end(),[](person A, person B){return A.age < B.age;}) << "\n";
    cout << "max " << *max_element(d.begin(),d.end(),[](person A, person B){return A.age < B.age;}) << "\n";
}
