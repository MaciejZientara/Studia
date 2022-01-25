#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <math.h>

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
  ADMUX   = _BV(REFS0) | _BV(MUX1) | _BV(MUX0); // referencja AVcc, wejście ADC3
  DIDR0   = _BV(ADC3D); // wyłącz wejście cyfrowe na ADC3
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  // ADCSRA |= _BV(ADEN) | _BV(ADIE); // włącz ADC + interrupt
  ADCSRA |= _BV(ADEN); // włącz ADC
}

FILE uart_file;


int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();

  volatile uint16_t prev = 0;

  do{
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
  }while(ADC>10);

  prev = ADC;

  while(1) {
    /*
      I = C * dV/dT
      I = 130 uA = 1.3 * 10e-4
      dT = 0.1 s
      dV = (ADC - prev)/1023.0 * 5.0

      C =  1.3 * 10e-5 / ((ADC - prev) * 5.0) = 13.0 / ((ADC - prev) * 5.0) uF
    */
    _delay_ms(100);
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    float res = 13.0 / ((ADC - prev)/1023.0 * 5.0);
    if(!isinf(res)){
      printf("C = %f uF\r\n", res);
      prev = ADC;
    }

  }
}
