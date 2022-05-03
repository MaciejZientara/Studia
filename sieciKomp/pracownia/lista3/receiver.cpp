#include "receiver.h"

Receiver::Receiver(int sockfd, sockaddr_in *sender, string filename){
    this->sockfd = sockfd;
    this->sender = sender;
    //out - output, app - append, ate - at end of file, binary - instead of text
    while(!this->file.is_open())
        this->file.open(filename, fstream::out | fstream::app | fstream::ate /*| fstream::binary*/);
    // file << "output"; aby zapisac dane do pliku
    received.resize(WINDOW_SIZE);
}

Receiver::~Receiver(){
    file.close();
}

void Receiver::sendRequest(string req){
    //if(sendto()==-1)ERROR!
    if(sendto(sockfd, (uint8_t *)(req.c_str()), req.length(), 0, (struct sockaddr *) sender, sizeof(*sender)) == -1){
        cerr << "SendRequest error\n";
        exit(1);
    }
}

string Receiver::makeRequest(int frameNumber, int fileSize){
    // "GET {offset} {size}"
    string res = "GET " + to_string((frameNumber - 1) * FRAME_SIZE) + " " + 
                to_string(frameNumber == frames ? fileSize % FRAME_SIZE : FRAME_SIZE) + "\n";
    //biore caly rozmiar FRAME lub dla ostatniego frame tyle ile zostalo z pliku do wczytania
    return res;
}

//jesli nie wyslalem jeszcze request dla danego frame lub nastapil timeout
void Receiver::sendAllRequests(int lfrd, int fileSize){
    uint64_t timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    uint64_t timeOUT = timeNow + TIMEOUT;
    for(int i=0; i<WINDOW_SIZE; i++){
        if(received[i]==0 || timeNow > received[i]){
            sendRequest(makeRequest(lfrd+1+i,fileSize));
            received[i] = timeOUT;
        }
    }
}

void Receiver::receive(int fileSize){
    int segments_acquired = 0, 
        lfrcv = 0, //lfr - last frame received
        lfrd = 0; //lfr - last frame read
    frames = ceil((double) fileSize / (double) this->FRAME_SIZE);

    /*
    jedna wspolna kolejka na odebrane pakiety
    iterator lfrd - ktore pakiety aplikacja juz odebrala
    kiedy odczytam pakiet, wyjmuje go z kolejki i zmniejszam ilosc pakietow w kolejce
    odczytuje pakiety while lfrd < lfrcv
    kiedy odbiore pakiet dodaje go do kolejki, zwiekszam ilosc pakietow w kolejce
    oraz zwiekszam lfrcv
    moge odbierac pakiety dopoki ilosc pakietow w kolejce < WINDOW_SIZE ???

    może niech ta kolejka przechowuje obiekty pomocniczej klasy
    class Frame{
        int offset, size;
        uint8_t* data;

        w konstruktorze zrobic cale parsowanie tekstu
    };

    teraz tworze nowy obiekt Frame i sprawdzam czy 
    z taka sama zawartoscia (offset i size) jest juz w kolejce
    jesli nie to dodaje do kolejki 
    (czy robie instert w odpowiednie miejsce czy na koniec???)
    
    moze dodawac do kolejki priorytetowej zamiast kolejki 
    i miec tablice bool otrzymanych
    kiedy moge zwiekszyc lfrcv zaczne zdejmowac elementy ze szczytu heap
    sortowanie po offset

    w kolejce priorytetowej nie mam jak sprawdzi czy jakis frame juz 
    byl dostarczony czy nie, ale do tego moge wykorzystac pomocnicza 
    tablice bool otrzymanych
    */

    while (lfrcv < frames) {
        sendAllRequests(lfrd,fileSize);
        // sliding_window.SelectData();

        // SegmentData *data = sliding_window.GetData();
        // while (data != nullptr) {
        //     SaveDataToFile(file, data);
        //     sliding_window.RemoveFirstSegmentData();
        //     data = sliding_window.GetData();
        // }
        
        // last_frame_received = sliding_window.GetLastFrameReceived();
        // if (segments_acquired != last_frame_received) {
        //     segments_acquired = last_frame_received;
        //     cout << "Acquired " << segments_acquired << '/' << last_frame << " segments.\n";
        // }
    }

}


Receiver::frameData::frameData(uint8_t *frame){
    int iter = 5;//zaczynam po "DATA "
    int ofs = 0, siz = 0;
    while(isdigit(frame[iter])){
        ofs = 10*ofs + (frame[iter]-'0');
        iter++;
    }
    iter++;//przejdz ze spacji do size
    while(isdigit(frame[iter])){
        siz = 10*siz + (frame[iter]-'0');
        iter++;
    }
    iter++;//przejdz ze spacji do danych
    
    offset = ofs;
    size = siz;
    data = (frame + iter);
}

template <typename T>
Receiver::cycleBuffer<T>::cycleBuffer(){
    begin = 0;
    size = 0;
    arr = nullptr;
}
template <typename T>
void Receiver::cycleBuffer<T>::resize(int size){
    begin = 0;
    this->size = size;
    arr = new T[size]();//dodanie () powinno wyzerowac arr
}
template <typename T>
Receiver::cycleBuffer<T>::~cycleBuffer(){
    delete[] arr;
}

template <typename T>
T& Receiver::cycleBuffer<T>::operator[] (int i){
    return arr[(begin+i)%size];
}

template <typename T>
void Receiver::cycleBuffer<T>::incBegin(){
    arr[begin] = 0;
    begin = (begin+1)%size;
}
