#include <iostream>
#include <memory>
#include <fstream>
#include <string>

using namespace std;

class line_writer{
    public:
    line_writer(ofstream *ofs){
        o = ofs;
    }
    ~line_writer(){
        // nie dajemy zamknięcia ofstream
    }
    void write(string s){
        (*o) << s << endl;
    }
    void closeStream(){
        o->close();
    }
    private:
    ofstream *o;
};

void close_stream(line_writer * x){
    x->write("zamykam strumien");
    x->closeStream();
    delete x;
}

void test(shared_ptr<line_writer> spt4){
    spt4->write("Ja jestem czwarty!");
}

int main(){
    ofstream ofs;
    ofs.open ("test.txt", ofstream::out | ofstream::app);

    while(ofs.is_open()){
        cout << "ofs otwarte\n";
        shared_ptr<line_writer> spt1 (new line_writer(&ofs),close_stream);
        spt1->write("Ja jestem pierwszy!");

        if(auto spt2 = spt1)
            spt2->write("Ja jestem drugi!");
        
        {
            auto spt3 = spt1;
            spt3->write("Ja jestem trzeci!");
        }

        test(spt1);

        auto spt5 = spt1;
        spt5->write("Ja jestem piaty!");
    }
}