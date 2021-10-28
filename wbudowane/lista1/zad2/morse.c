#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

// int main() { //LED 
//   LED_DDR |= _BV(LED);
//   while (1) {
//     LED_PORT |= _BV(LED);
//     _delay_ms(1000);
//     LED_PORT &= ~_BV(LED);
//     _delay_ms(1000);
//   }

void waitUnit(int x){
  for(int i=0; i<x; i++)
    _delay_ms(200);
}
void LEDoff(){
  LED_PORT &= ~_BV(LED);
}
void LEDon(){
  LED_PORT |= _BV(LED);
}

int8_t morseCode[26][5] = {
  {1,3,0,0,0},  //A
  {3,1,1,1,0},  //B
  {3,1,3,1,0},  //C
  {3,1,1,0,0},  //D
  {1,0,0,0,0},  //E
  {1,1,3,1,0},  //F
  {3,3,1,0,0},  //G
  {1,1,1,1,0},  //H
  {1,1,0,0,0},  //I
  {1,3,3,3,0},  //J
  {3,1,3,0,0},  //K
  {1,3,1,1,0},  //L
  {3,3,0,0,0},  //M
  {3,1,0,0,0},  //N
  {3,3,3,0,0},  //O
  {1,3,3,1,0},  //P
  {3,3,1,3,0},  //Q
  {3,1,3,0,0},  //R
  {1,1,1,0,0},  //S
  {3,0,0,0,0},  //T
  {1,1,3,0,0},  //U
  {1,1,1,3,0},  //V
  {1,3,3,0,0},  //W
  {3,1,1,3,0},  //X
  {3,1,3,3,0},  //Y
  {3,3,1,1,0}   //Z
};

// https://commons.wikimedia.org/wiki/File:International_Morse_Code.svg

void morse(char *s){
  int iter = 0;
  char c = s[iter];
  while(c!='\0'){
    if(c==' '){ //SPACJA NIE DZIAŁA! problem ze screen?
      //spacja ma 7, ale po kazdym znaku czeka sie 3, wiec tutaj tylko 4
      LEDoff();
      waitUnit(4);
    }
    else{
      int ptr = c - 'A';
      for(int i=0; i<5; i++){
        LEDon();
        waitUnit(morseCode[ptr][i]);
        LEDoff();
        waitUnit(1);
      }
    }

    c = s[++iter];
    waitUnit(3);
  }
    
}

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // program testowy
  printf("Hello!\r\n");
  while(1) {
    char s[20];
    scanf("%s", s);
    printf("Odczytano: %s\r\n", s);
    morse(s);
  }
}

