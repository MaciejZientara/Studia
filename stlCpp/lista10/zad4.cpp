#include <iostream>
#include <filesystem>

using namespace std;

// g++ -std=c++17 -o zad4 zad4.cpp 

int main(){
    const filesystem::path dir{"."};
    // const filesystem::path dir{"nie_istnieje"};

    if(!filesystem::is_directory(dir)){
        cout << "Brak takiego folderu: " << dir << "!\n"; 
        return 0;
    }

    for(auto dir_entry : filesystem::directory_iterator{dir})
        std::cout << dir_entry << '\n';
}