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

void timer1_init(){
  // ustaw tryb licznika
  // WGM1  = 0100 -- CTC top=OCR1A
  // CS1   = 001  -- prescalar 1
  // wzór: datasheet 20.12.3 str. 164
  // częstotliwość 16e6/1*(1+1599) = 10000 Hz
  // OCR1A  = 1599
  OCR1A = 1599;
  TCCR1B |= _BV(WGM12) | _BV(CS10);
  // TIMSK1 |= _BV(OCIE1A); //Timer/Counter1, Output Compare A Match Interrupt Enable
}

ISR (TIMER1_COMPA_vect){
  ADCSRA |= _BV(ADSC); // wykonaj konwersję
}

volatile uint64_t sum = 0;
volatile uint16_t last = 0;
volatile uint32_t count = 0;

ISR(ADC_vect){
  uint16_t tmp = ADC;
  sum += tmp;

  printf("%f\r\n", (float)ADC);

  // uint16_t tmp = ADC;
  // sum += tmp - last;
  // last = tmp;
  // count++;

  // printf("C = %f uF\r\n", (1.3 / (5.0 * (float)(ADC - last)))/(float)100);
  // last = ADC;
}

int main()
{
  sei();
  timer1_init();
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  // mierz napięcie

  volatile uint16_t prev = 0;

  do{
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
  }while(ADC>10);

  prev = ADC;

  printf("timer + adc interrupt on\r\n");
  TIMSK1 |= _BV(OCIE1A); //Timer/Counter1, Output Compare A Match Interrupt Enable
  ADCSRA |= _BV(ADIE); // włącz ADC interrupt

  while(1) {
    /*
      I = C * dV/dT
      I = 130 uA = 1.3 * 10e-4
      dT = 0.1 s
      dV = (ADC - prev) * 5.0

      C =  1.3 * 10e-5 / ((ADC - prev) * 5.0) = 13.0 / ((ADC - prev) * 5.0) uF
    */
    // _delay_ms(100);
    // ADCSRA |= _BV(ADSC); // wykonaj konwersję
    // while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    // ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    // printf("C = %f uF\r\n", 13.0 / ((ADC - prev) * 5.0));
    // prev = ADC;


    _delay_ms(200);
    // printf("C = %f uF\r\n", (1300000.0 / ((5.0*sum/1023.0))));


    /*
      I = C * dV/dT
      I = 130 uA = 1.3 * 10e-4
      dT = 10e-4

      C = 10e-8 * 1.3 / dV 
    */
    // _delay_ms(200);
    // printf("C = %f\r\n", (1.3 / ((float)sum/count)));
    // // printf("C = %f\r\n", (1.3 / ((float)sum/count))/(float)10e8);
    // // printf("C = %f uF\r\n", (1.3 / ((float)sum/count))/(float)100);
    // count = 0;
    // sum = 0;
    // last = 0;
  }
}



/*

moze to na zwyklym delay w main zrobic, zupelnie bez timera

najpierw wziac pomiar, poczekac iles ms, nowy pomiar -> mamy dV i ms jako dT


*/
