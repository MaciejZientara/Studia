#include "receiver.h"
//wszystkie potrzebne include w receiver.h

using namespace std;

bool isIP(string arg){
    int val = 0;
    for(char a : arg){
        if(isdigit(a)){
            val = 10*val+((int)a-'0');
            continue;
        }
        if(a == '.'){
            if(val>255)
                return false;
            val = 0;
            continue;
        }
        //nie jest cyfra ani kropka
        return false;
    }
    if(val>255) //po ostatnim numerze nie ma '.', 
                //trzeba go rozpatrzyc oddzielnie
        return false;
    return true;
}

bool isNumber(string arg){
    for(char a : arg)
        if(!(isdigit(a)))
            return false;
    return true;
}

void checkArguments(int argc, char** argv){
    if(argc != 5){
        cout << "Zla ilosc argumentow!\n";
        exit(1);
    }
    if(!isIP(argv[1])){
        cout << "Zly adres IP!\n";
        exit(1);
    }
    if(!isNumber(argv[2])){
        cout << "Port nie jest numerem!\n";
        exit(1);
    }
    //nazwy pliku nie trzeba sprawdzac
    if(!isNumber(argv[4])){
        cout << "Rozmiar nie jest numerem!\n";
        exit(1);
    }
}

int prepareSocket(string IP, int port){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1){
        cerr << "Socket error\n";
        exit(1);
    }
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, IP.c_str(), &address.sin_addr);
    bind(sock, (struct sockaddr *) &address, sizeof(address));
    return sock;
}

sockaddr_in* prepareSender(string IP, int port){
    struct sockaddr_in *sender = new sockaddr_in;
    bzero(sender, sizeof(*sender));
    sender->sin_family = AF_INET;
    sender->sin_port = htons(port);
    inet_pton(AF_INET, IP.c_str(), &sender->sin_addr);
    return sender;
}

void downloadFile(string IP, int port, string filename, int size){
    //przygotuj socket
    int sockfd = prepareSocket(IP, port);
    //przygotuj adres nadawcy
    sockaddr_in* sender = prepareSender(IP, port);

    //pobieranie danych
    Receiver rec(sockfd, sender, filename);
    rec.receive(size);

    delete sender;
}

int main(int argc, char** argv){
    checkArguments(argc, argv);
    //           adres IP , port , nazwa pliku i rozmiar
    downloadFile(argv[1],stoi(argv[2]),argv[3],stoi(argv[4]));
    cout << "hello!\n";
}