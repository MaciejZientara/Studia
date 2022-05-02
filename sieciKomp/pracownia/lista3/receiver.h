#ifndef RECEIVER
#define RECEIVER

#include <ctype.h>//std::isdigit(int arg) (arg moze byc char)
#include <string>
#include <iostream>
#include <fstream>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h> // bzero
#include <cmath>

using namespace std;

class Receiver{
    public:
    Receiver(int sockfd, sockaddr_in *sender, string filename);
    ~Receiver();
    void receive(int fileSize);

    private:
    void sendRequest(string req);
    string makeRequest(int segmentNumber, int fileSize);

    int sockfd;
    sockaddr_in *sender;
    fstream file;
    int frames;
    const static int WINDOW_SIZE = 2500;
    const static int FRAME_SIZE = 1000;
    const static int MILISECONDS = 750;
    const static int ADDRESS_LENGTH = 19;
};

#endif