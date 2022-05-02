#include "receiver.h"

Receiver::Receiver(int sockfd, sockaddr_in *sender, string filename){
    this->sockfd = sockfd;
    this->sender = sender;
    //out - output, app - append, ate - at end of file, binary - instead of text
    while(!this->file.is_open())
        this->file.open(filename, fstream::out | fstream::app | fstream::ate /*| fstream::binary*/);
    // file << "output"; aby zapisac dane do pliku
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
    */

    while (lfrcv < frames) {
        // sliding_window.Populate();
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