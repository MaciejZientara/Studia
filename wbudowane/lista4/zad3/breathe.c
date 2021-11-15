#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define N 4
#define WAIT 15

void timer1_init(){
  // ustaw tryb licznika
  // COM1A = 10   -- fast PWM non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 011  -- prescaler 64

  // częstotliwość 16e6/(1024*(1+15624)) = 1 Hz
  // wzór: datasheet 20.12.3 str. 164

  ICR1 = WAIT<<N;
  OCR1A = WAIT;
  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10) | _BV(CS11);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}

void colorControl(int8_t color, int8_t delay){//        TO FINISH
  _delay_ms(200);
}

void helper(int8_t color, int8_t i){
// 16e6 / 1024 = 15625
// licznik w 1sek naliczy 15625, w 0.001sek ~ 15
  if(i)
    ICR1 = WAIT<<i;
  else
    ICR1 = WAIT;
  colorControl(color,50);
}

void breathe(){ //zwiekszamy czas trwania zapalonej diody zmieniając wartość OCR1A
  int8_t color = (rand()%7)+1; //                            TODO hue randomize
  color ^= 0x7;
  PORTC = color; // ustaw piny na kolor

  OCR1A = 0;
  PORTB |= _BV(PB1);
  colorControl(color,50);
  OCR1A = WAIT;

  for(int8_t i=N-1; i>=0; i--){
    TCNT1 = 0;
    helper(color,i);
  }
  for(int8_t i=0; i<=N; i++){
    TCNT1 = 0;
    helper(color,i*2);
  }

  OCR1A = 0;
  PORTB |= _BV(PB1);
  colorControl(color,50);
}

int adc(){
  ADMUX   = _BV(REFS0) | 0x5; // referencja AVcc, wejście ADC5
  DIDR0   = _BV(ADC5D); // wyłącz wejście cyfrowe na ADC5               ????
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC

  ADCSRA |= _BV(ADSC); // wykonaj konwersję
  while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
  ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
  uint16_t v = ADC; // weź zmierzoną wartość (0..1023)

  ADCSRA &= ~_BV(ADEN); // wyłącz ADC
  return (int)v;
}

int main(){
  srand(adc());
  timer1_init(); // uruchom licznik
  
  DDRC |= 0x7;
  PORTC = 0;//wszystkie led wlaczone

  while(1){
      breathe();
      _delay_ms(300);
  }
}

/*
    niech anoda (PWM) odpowiada za jasność (miganie)
    natomiast - odpowiadaja za kolory (również migają)
        aby migały zamiast losować z przedziału [1,7] nalezy losowac hue 0-255
        a nastepnie w breathe zamiast delay() miec miganie pinow
*/

