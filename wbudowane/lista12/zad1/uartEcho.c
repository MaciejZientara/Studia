#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define true 1
#define false 0

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
  // wlacz rx i tx interrupt
  UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0) | _BV(UDRIE0);
}

char echo;//znak do nadania
int8_t received;//bool czy odebrano znak, jesli nie to nic nie nadajemy
int8_t ready;//bool czy gotowy do wysylki, ale nie dostal jeszcze znaku

void send(){
  UDR0 = echo;
  received = false;
  ready = false;
}

ISR(USART_RX_vect){//receive
  echo = UDR0;
  received = true;
  if(ready)
    send();
}

ISR(USART_TX_vect){//transmit finished
}

ISR(USART_UDRE_vect){//ready to send new data
  if(received)
    send();
  else
    ready = true;
}


int main() {
  // zainicjalizuj UART
  uart_init();

  set_sleep_mode(SLEEP_MODE_IDLE);
  sei();

  echo = '\r';

  while (1) {
    sleep_mode();
  }
}
