#ifndef RECEIVER
#define RECEIVER

#include <ctype.h>//std::isdigit(int arg) (arg moze byc char)
#include <string>
#include <iostream>
#include <fstream>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h> // bzero
#include <cmath> // ceil
#include <queue> // priority queue
#include <chrono>
#include <memory> // unique_ptr

using namespace std;

class Receiver{
    public:
    Receiver(int sockfd, sockaddr_in *sender, string filename);
    ~Receiver();
    void receive(int fileSize);

    private:
    void sendRequest(string req);
    void sendRequests(int lfrd, int fileSize);
    void receiveData(int& lfrcv, int lfrd);
    void saveData(int& lfrd, int lfrcv);
    void printProgress(int lfrd, int lfrcv, int frames);

    int frameNrToOffset(int frameNumber);
    int offsetToFrameNr(int offset);

    int sockfd;
    sockaddr_in *sender;
    fstream file;
    int frames;
    const static int WINDOW_SIZE = 2500;
    const static int FRAME_SIZE = 1000;
    const static int TIMEOUT = 500;
    const static int SELECT_WAIT = 750;
    const static int IP_MAX_LENGTH = 20;

    template <typename T>
    class cycleBuffer{
        private:
        int begin = 0, size;
        T* arr;
        public:
        cycleBuffer();
        void resize(int size);
        ~cycleBuffer();
        T& operator[] (int i);
        void incBegin();
    };

    class frameData{
        public:
        int offset, size;
        uint8_t *data;
        frameData(uint8_t *frame);
        ~frameData();
    };
    struct compareFrameData{
        bool operator() (unique_ptr<frameData> const& f1, unique_ptr<frameData> const& f2){
            return (f1->offset > f2->offset);
        }
    };

    /*pozwala mi rozroznic czy pakiet ma timeout, nie zostal jeszcze wyslany, zostal otrzymany
        0 - nie wyslano jeszcze request
        -1 - otrzymano pakiet
        else to timestamp+timeout czyli kiedy wyslalem request (milisekundy) + czas timeout,
        kiedy aktualny czas > od zapisanego to znaczy, ze nastapil timeout i trzeba wyslac ponownie request*/
    cycleBuffer<uint64_t> received;
    //tutaj dodaje nowe pakiety i stad odczytuje, zeby zapisac do pliku
    priority_queue<unique_ptr<frameData>, vector<unique_ptr<frameData>>, compareFrameData> que;
};

#endif