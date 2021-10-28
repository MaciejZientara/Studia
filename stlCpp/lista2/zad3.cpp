#include <iostream>
#include <memory>
 
using namespace std;

std::weak_ptr<int> weak;
 
void inspect(){
    cout << "use_count() = " << weak.use_count() << "\n";
    std::shared_ptr<int> spt = weak.lock();
    std::cout << "zawartosc weak = " << *spt << '\n';
    //jesli weak jest wiszacy odwolamy sie do zwolnionej pamieci = segfault
}

void makeShared(){
    auto sp = std::make_shared<int>(125);
    weak = sp;
    inspect();
    //zwalniamy sp
}

int main()
{
    makeShared();
    inspect();
}