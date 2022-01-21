#include <iostream>
#include <filesystem>

using namespace std;

// g++ -std=c++17 -o zad5 zad5.cpp 

int main(){
    const filesystem::path dir{".."};

    if(!filesystem::is_directory(dir)){
        cout << "Brak takiego folderu: " << dir << "!\n"; 
        return 0;
    }

    unsigned long long sum = 0, count = 0;

    for(auto &dir_entry : filesystem::recursive_directory_iterator{dir}){
        // std::cout << dir_entry << '\n';
        if(!dir_entry.is_directory()){
            sum+=dir_entry.file_size();
            count++;
        }
    }
    cout << "count = " << count << ", total file size = " << sum << "\n";
}