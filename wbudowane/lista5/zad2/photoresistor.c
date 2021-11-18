#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init(){
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
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

// inicjalizacja ADC

void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  ADCSRA |= _BV(ADIE); // włącz ADC interrupt
  ADCSRA |= _BV(ADATE); // ADC Auto Trigger Enable
  ADCSRB |= _BV(ADTS1); // ADTS 010 - External Interrupt Request 0
}

void io_init()
{
  // ustaw pull-up na PD2 (INT0)
  PORTD |= _BV(PD2);
  // ustaw wyzwalanie przerwania na INT0 zboczem opadającym
  EICRA |= _BV(ISC01);
  // odmaskuj przerwania dla INT0
  EIMSK |= _BV(INT0);
}

FILE uart_file;

volatile uint16_t measure;

ISR(ADC_vect){
  measure = ADC;
}

ISR(INT0_vect) {
  // printf("INT0\r\n");
}

int main()
{
  sei();
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  // zainicjalizuj INT0
  io_init();
  // mierz napięcie
  while(1) {
    float tmp = ((measure * 2200.0) / (1023.0 - measure));
    printf("Odczytano: %hd, %.3f Ohm\r\n",measure, tmp);
    _delay_ms(500);
  }
}

/*

v = 1023*R/(R+2200)
vR + 2200v = 1023*R
2200v = R(1023 - v)

*/
