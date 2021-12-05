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
  ADMUX  |= 0xe;        // MUX[3:0], input 1.1V (VBG)
//   DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

FILE uart_file;

#define N 100
uint16_t measureDelay[N];
uint16_t measureSleep[N];

void calibrateADC(){
  for(int8_t i=0; i<5; i++){
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
  }
}

void ADC_delay(){
  for(int8_t i=0; i<N; i++){
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    measureDelay[i] = ADC;
  }
}

volatile int8_t iter = 0;
ISR(ADC_vect){
  measureSleep[iter++] = ADC;
}

void ADC_Sleep(){
  ADCSRA |= _BV(ADIE); // włącz ADC interrupt
  sei();
  set_sleep_mode(SLEEP_MODE_ADC);

  while(iter!=N){
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    sleep_mode();
  }
  ADCSRA &= ~_BV(ADIE); // wyłącz ADC interrupt  
}

float variance(uint16_t tab[]){
  float average = 0.0;
  for(int8_t i=0; i<N; i++)
    average+=tab[i];
  average/=N;

  float result = 0.0;
  for(int8_t i=0; i<N; i++){
    float tmp = tab[i] - average;
    result += (tmp*tmp);
  }
  result/=N;
  return result;
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  // mierz napięcie

  calibrateADC();

  ADC_delay();
  ADC_Sleep();

  printf("Wariancja dla delay = %.4f, Wariancja dla sleep = %.4f\r\n",variance(measureDelay),variance(measureSleep));


  //DEBUG
  /*
  for(int8_t i=0; i<N; i++)printf("%hd ", measureDelay[i]);
  printf("\r\n");  
  for(int8_t i=0; i<N; i++)printf("%hd ", measureSleep[i]);
  printf("\r\n");
  */

  _delay_ms(2000);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  while(1) {
    sleep_mode();
  }
}