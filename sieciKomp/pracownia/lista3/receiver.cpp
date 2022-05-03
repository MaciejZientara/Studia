#include "receiver.h"

Receiver::Receiver(int sockfd, sockaddr_in *sender, string filename){
    this->sockfd = sockfd;
    this->sender = sender;
    //out - output, app - append, ate - at end of file, binary - instead of text
    while(!this->file.is_open())
        this->file.open(filename, fstream::out | fstream::app | fstream::ate | fstream::binary);
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

int Receiver::frameNrToOffset(int frameNumber){
    return (frameNumber - 1) * FRAME_SIZE;
}

int Receiver::offsetToFrameNr(int offset){
    return (offset/FRAME_SIZE)+1;
}

string Receiver::makeRequest(int frameNumber, int fileSize){
    // "GET {offset} {size}"
    string res = "GET " + to_string(frameNrToOffset(frameNumber)) + " " + 
                to_string(frameNumber == frames ? fileSize % FRAME_SIZE : FRAME_SIZE) + "\n";
    //biore caly rozmiar FRAME lub dla ostatniego frame tyle ile zostalo z pliku do wczytania
    return res;
}

//jesli nie wyslalem jeszcze request dla danego frame lub nastapil timeout
void Receiver::sendRequests(int lfrd, int fileSize){
    uint64_t timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    uint64_t timeOUT = timeNow + TIMEOUT;
    for(int i=0; i<WINDOW_SIZE; i++){
        if(received[i]==0 || timeNow > received[i]){
            string req = makeRequest(lfrd+1+i,fileSize);
            sendRequest(req);
            received[i] = timeOUT;
        }
    }
}

void Receiver::receiveData(int& lfrcv, int lfrd){
    //przygotowanie select - usypianie czekajac na file descriptor
    fd_set rfds;
    timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);//select bedzie nasluchiwal na file descriptor socket

    // TIMEOUT jest w milisekundach
    tv.tv_sec = TIMEOUT/1000;
    tv.tv_usec = (TIMEOUT*1000)%1000000;

    char expected_ip[IP_MAX_LENGTH];
    inet_ntop(AF_INET, &(sender->sin_addr), expected_ip, IP_MAX_LENGTH);

    while(true){//wyjde kiedy nastapi timeout
        retval = select(sockfd+1, &rfds, NULL, NULL, &tv);//usypanie, az bedzie mozna odczytac
        if(retval == -1)//wystapil blad przy select
            continue;
        if(retval == 0)//timeout
            break;

        //mozna odebrac pakiet
        sockaddr_in currentSender;	
		socklen_t sender_len = sizeof(currentSender);
		uint8_t buffer[IP_MAXPACKET];

        ssize_t packet_len = recvfrom (sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&currentSender, &sender_len);
        if (packet_len < 0) {
            cerr << "ReceiveData error\n";
            exit(1);
        }

        char sender_ip[IP_MAX_LENGTH];
        inet_ntop(AF_INET, &(currentSender.sin_addr), sender_ip, IP_MAX_LENGTH);

        //rozpatrz przyjety pakiet tylko kiedy zgadza sie IP oraz port
        if((strcmp(expected_ip,sender_ip)==0) && (currentSender.sin_port == sender->sin_port)){
            frameData* frD = new frameData(buffer);
            int frameNumber = offsetToFrameNr(frD->offset);
            if((frameNumber > lfrd) && (received[frameNumber-lfrd-1]!=-1)){
                //received[]!=-1 oznacza, ze jeszcze nie otrzymalem tego pakietu
                received[frameNumber-lfrd-1]=-1;
                que.push(frD);
            }
            else
                delete frD;
        }
    }
    while((received[lfrcv-lfrd]==-1) && lfrcv<frames)
        lfrcv++;
}

void Receiver::saveData(int& lfrd, int lfrcv){
    while(lfrd < lfrcv){
        file.write((char*)que.top()->data,que.top()->size);
        frameData* tmp = que.top();
        que.pop();
        delete tmp;
        lfrd++;
        received.incBegin();
    }
}

void Receiver::printProgress(int lfrd, int lfrcv, int frames){
    static int iter = 0;
    static const char loading[] = {'|','/','-',92};// 92 to symbol '\'
    std::cout << "\r" << loading[iter] << " read:" << lfrd << ", received: " << lfrcv << "/" << frames;
    std::fflush(stdout);
    iter = (iter+1)%4;
}

void Receiver::receive(int fileSize){
    int segments_acquired = 0, 
        lfrcv = 0, //lfr - last frame received
        lfrd = 0; //lfr - last frame read
    frames = ceil((double) fileSize / (double) this->FRAME_SIZE);

    while(lfrcv < frames){
        //wyslanie request
        sendRequests(lfrd,fileSize);
        //odebranie odpowiedzi
        receiveData(lfrcv,lfrd);
        //zapisanie do pliku
        saveData(lfrd,lfrcv);

        printProgress(lfrd,lfrcv,frames);
    }
    std::cout << "\rDONE                                                                         \n";//duzo spacji, zeby zaslonic ewentualny tekst
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
    data = new uint8_t[size];
    memcpy(data,frame+iter,size);
}

Receiver::frameData::~frameData(){
    delete[] data;
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
