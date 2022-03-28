#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <cassert>
#include <unistd.h>
#include <sys/select.h>
#include <netinet/ip_icmp.h>

#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;


pid_t PID;
int sockfd;

u_int16_t compute_icmp_checksum (const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = (u_int16_t*)buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

unsigned short swapBytes(unsigned short x){
    return ((x&0xff00)>>8) | ((x&0xff)<<8);
}

void print_as_bytes (unsigned char* buff, ssize_t length)
{
	for (ssize_t i = 0; i < length; i++, buff++)
		printf ("%.2x ", *buff);	
}


int main(int argc, char** argv){
    //sprawdzenie ilosci argumentow
    if(argc != 2){
		fprintf(stderr, "Only argument should be IP address\n"); 
		return EXIT_FAILURE;
    }

    //ustawienie adresu odbiorcy
    sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &recipient.sin_addr);

    //ustawienie file descriptor
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    // ustawienie odpowiedniego naglowka pakietu
    PID = getpid();
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = PID;//unikatowy identyfikator - id procesu
    header.icmp_hun.ih_idseq.icd_seq = 0;//numer sekwencyjny
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(header));

    //przygotowanie select - usypianie czekajac na file descriptor
    fd_set rfds;
    timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);//select bedzie nasluchiwal na file descriptor socket

    tv.tv_sec = 1;//czekamy co najwyzej sekunde
    tv.tv_usec = 0;

    
	for (uint ttlCounter = 1;ttlCounter<30;ttlCounter++) {
        header.icmp_hun.ih_idseq.icd_seq = ttlCounter;//wykorzystuje go do okreslenia przy jakim ttl zostal wyslany pakiet
        header.icmp_cksum = 0;
        header.icmp_cksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(header));

        int64_t startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        printf("%d. ", ttlCounter);
        while(setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttlCounter, sizeof(int)) != 0);//ustawiamy nowy ttl, ustawiony kiedy return = 0
		
        size_t bytes_sent;
        int repeat = 3;
        size_t headerLen = sizeof(header);
        while(repeat > 0){
            printf("send\n");
            // tv.tv_sec = 1;//czekamy co najwyzej sekunde, odswiezam timer
            // retval = select(sockfd+1, NULL, &rfds, NULL, &tv);//usypanie, az bedzie mozna wyslac
            // printf("retval dla send = %d\n",retval);
            // if(retval != 1)//wystapil blad select/timeout
            //     continue;
            bytes_sent = sendto(sockfd, &header, headerLen, 0, (struct sockaddr*)&recipient, sizeof(recipient));
            printf("bytes sent = %ld, header len = %ld\n", bytes_sent, headerLen);//debug
            if(bytes_sent == EWOULDBLOCK){
                printf("would block\n");
                //ta funkcja sluzy zwolnieniu miejsca w file descriptor buffer, 
                //spoznione pakiety zajmuja w nim miejsce i uniemozliwiaja wysylke nowych
                recvfrom (sockfd, NULL, IP_MAXPACKET, MSG_DONTWAIT, NULL, NULL);
                // nie chcemy czekac, jesli cos bylo to odczytujemy, ale wyrzucamy od razu wszystkie dane
                continue;
            }
            if(bytes_sent == headerLen)
                repeat--;
        }//wysylam 3 ping, sprawdzam tez czy wyslaly sie pelne header - jesli nie to powtarzam ping

		sockaddr_in sender;	
		socklen_t sender_len = sizeof(sender);
		u_int8_t buffer[IP_MAXPACKET];


		char sender_ip[3][25];
        int64_t endTime[3];
        
        sender_ip[0][0] = '*';
        sender_ip[1][0] = '*';//brak danych, potrzebne gdy nie odebralismy pakietow
        sender_ip[2][0] = '*';

        tv.tv_sec = 1;//czekamy co najwyzej sekunde, odswiezam timer
        for(repeat=3;repeat>0;repeat--){
            printf("ttl = %d, repeat = %d\n", ttlCounter, repeat);//debug
            retval = select(sockfd+1, &rfds, NULL, NULL, &tv);//usypanie, az bedzie mozna odczytac

            if(retval == -1){
                //wystapil blad przy select
                repeat++;//powtorz select
                continue;
            }
            if(retval == 0){
                //timeout
                printf("RECEIVE TIMEOUT\n");
                break;
            }
            //mozna odebrac pakiet

    		ssize_t packet_len = recvfrom (sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
            if (packet_len < 0) {
                fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
                return EXIT_FAILURE;
            }

            //odczytanie IP header i icmp header
            ip* ip_header = (ip*) buffer;
            ssize_t ip_header_len = 4 * ip_header->ip_hl;
            u_int8_t* icmp_packet = buffer + ip_header_len;
            icmp* icmp_header = (icmp*) icmp_packet;
            
            //sprawdzam czy pakiet nalezy do mnie (czy id == PID)

            printf("icmp type = %d\n", icmp_header->icmp_type);

            if(icmp_header->icmp_type == 11){//time exceeded
                ip* ip_head = (ip*) (icmp_packet+8);//+8 wyliczone ze struktury icmp time exceeded
                ssize_t ip_head_len = 4 * ip_head->ip_hl;//ip_head to stary header IP
                u_int8_t* old_packet = icmp_packet+8 + ip_head_len;
                icmp* old_icmp = (icmp*) old_packet;
                printf("TTL ip_head = %d, counter = %d\n",old_icmp->icmp_hun.ih_idseq.icd_seq, ttlCounter);
                if(old_icmp->icmp_hun.ih_idseq.icd_seq != ttlCounter){
                    repeat++;//powtorz select, otrzymalismy stary pakiet (z poprzedniego wysylania, ma stary ttl)
                    continue;
                }
                if(old_icmp->icmp_hun.ih_idseq.icd_id != PID){
                    repeat++;//powtorz select, otrzymalismy nie nalezacy do nas pakiet
                    continue;
                }                
            }
            if(icmp_header->icmp_type == 0){//ping reply
                if(icmp_header->icmp_hun.ih_idseq.icd_id != PID){
                    repeat++;//powtorz select, otrzymalismy nie nalezacy do nas pakiet
                    continue;
                }
                ttlCounter = 100;//otrzymalismy odpowiedz od serwera, pozwala zakonczyc petle for
            }

            //zapisz ile czasu potrzeba bylo na odebranie tego pakietu (w milisekundach)
            endTime[3-repeat] = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - startTime;
            //zapisz adres ip routera
    		inet_ntop(AF_INET, &(sender.sin_addr), sender_ip[3-repeat], sizeof(sender_ip[3-repeat]));
        }
        if(sender_ip[0][0]=='*' && sender_ip[1][0]=='*' && sender_ip[2][0]=='*')
            printf("*");//nie odebrano zadnego pakietu
        else{
            if(sender_ip[0][0]!='*')
                printf("%s ", sender_ip[0]);
    
            if(sender_ip[1][0]!='*' && strcmp(sender_ip[0],sender_ip[1])!=0)
                printf("%s ", sender_ip[1]);
    
            if(sender_ip[2][0]!='*' && strcmp(sender_ip[0],sender_ip[2])!=0 && strcmp(sender_ip[1],sender_ip[2])!=0 )
                printf("%s ", sender_ip[2]);


            if(sender_ip[0][0]=='*' || sender_ip[1][0]=='*' || sender_ip[2][0]=='*')
                printf("???");//nie odebrano wszystkich 3 pakietow
            else
                printf("%ld ms", (endTime[0]+endTime[1]+endTime[2])/3);
        }

        printf("\n");
	}

	return EXIT_SUCCESS;
}
