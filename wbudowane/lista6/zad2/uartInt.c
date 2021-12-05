#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define true 1
#define false 0

#define BSIZE 16

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem


void debugBlink(){
  PORTB |= _BV(PB5);
  _delay_ms(50);
  PORTB &= ~_BV(PB5);
  _delay_ms(50);
}


// inicjalizacja UART
void uart_init(){
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
  // wlacz rx i tx interrupt
  UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0) | _BV(UDRIE0);
}

void IncIter(int16_t *i){
  (*i)++;
  if((*i)==BSIZE)
    (*i)=0;
}

volatile char bufferTX[BSIZE], bufferRX[BSIZE];//bufory cykliczne
volatile int16_t iterTXr=0, iterTXw=0, iterRXr=0, iterRXw=0;//iteratory Read i Write dla RX i TX
volatile int8_t readyTX;//bool czy gotowy, ale nie dostal jeszcze znaku

ISR(USART_RX_vect){//receive
  bufferRX[iterRXw] = UDR0;
  IncIter(&iterRXw);
}

ISR(USART_TX_vect){//transmit finished
}

ISR(USART_UDRE_vect){//ready to send new data
  if(iterTXr!=iterTXw){
    UDR0 = bufferTX[iterTXr];
    IncIter(&iterTXr);
    readyTX = false;
  }
  else
    readyTX = true;
}


// transmisja jednego znaku
int uart_transmit(char data, FILE *stream){
  if(readyTX){
    readyTX = false;
    UDR0 = data;
  }
  else{
    // czekaj aż miejsce do zapisu dostępne
    while( (((iterTXw+1) == BSIZE)? 0:(iterTXw+1)) == iterTXr)_delay_ms(1);//debugBlink();
    bufferTX[iterTXw] = data;
    IncIter(&iterTXw);
  }
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream){
  // czekaj aż znak dostępny
  while(iterRXw==iterRXr)_delay_ms(1);
  int tmp = bufferRX[iterRXr];
  IncIter(&iterRXr);
  return tmp;
}

FILE uart_file;

int main() {
  DDRB |= _BV(PB5);

  debugBlink();
  debugBlink();
  debugBlink();
  debugBlink();

  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  sei();

  while (1) {
    char c = 'a';
    // scanf("%c",&c);//mozna zakomentowac - dziala printf w nieskonczonosc
    printf("%c",c);
    if(c == 'x')//jak nacisne x i potem kilka dodatkowych znakow to po jakims czasie one sie pokaza, mimo ze aktywne czekanie w debug
      debugBlink();
  }
}
