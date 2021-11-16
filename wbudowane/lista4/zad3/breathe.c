#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>

#define N 6
#define WAIT 2

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

void colorControl(int16_t color){//        TO FINISH
  int8_t Cpos = ((color+60)%360)/120; //0=R', 1=G', 2=B'
  int8_t Xpos = (color/60)%3; //0=G', 1=R', 2=B'

  int8_t x = (int8_t)(100.0*fmod(color/60.0,2.0));

  int8_t R=0,G=0,B=0;
  switch(Cpos){
  case 0:
    R=100;
    break;
  case 1:
    G=100;
    break;
  case 2:
    B=100;
    break;
  default:
    break;
  }
  switch(Xpos){
  case 0:
    G=x;
    break;
  case 1:
    R=x;
    break;
  case 2:
    B=x;
    break;
  default:
    break;
  }

  for(int16_t q=0; q<2000; q++){
    int8_t port = 0;
    if(R<20)
      port |= 0b1;
    if(G<20)
      port |= 0b10;
    if(B<20)
      port |= 0b100;
    PORTC = port;
    _delay_us(1<<4);
    
    if(R<40)
      port |= 0b1;
    if(G<40)
      port |= 0b10;
    if(B<40)
      port |= 0b100;
    PORTC = port;
    _delay_us(1<<3);
    
    if(R<60)
      port |= 0b1;
    if(G<60)
      port |= 0b10;
    if(B<60)
      port |= 0b100;
    PORTC = port;
    _delay_us(1<<2);

    if(R<80)
      port |= 0b1;
    if(G<80)
      port |= 0b10;
    if(B<80)
      port |= 0b100;
    PORTC = port;
    _delay_us(1<<1);

    if(R<90)
      port |= 0b1;
    if(G<90)
      port |= 0b10;
    if(B<90)
      port |= 0b100;
    PORTC = port;
    _delay_us(1<<0);
  }

/*
  for(int16_t j=0; j<2000; j++){
    int8_t r=R,g=G,b=B;
    for(int8_t i=0; i<100; i++){
      int8_t port = 0;
      r--;
      g--;
      b--;
      if(r<0)
        port |= 0b1;
      if(g<0)
        port |= 0b10;
      if(b<0)
        port |= 0b100;

      PORTC = port;// ustaw piny na kolor
      _delay_us(1);
    }
  }
*/
  // _delay_ms(200);
}

void helper(int16_t color, int8_t i){
// 16e6 / 1024 = 15625
// licznik w 1sek naliczy 15625, w 0.001sek ~ 15
  if(i)
    ICR1 = WAIT<<i;
  else
    ICR1 = WAIT;
  colorControl(color);
}

void breathe(){ //zwiekszamy czas trwania zapalonej diody zmieniając wartość OCR1A
  int16_t color = rand()%360;

  OCR1A = 0;
  PORTB |= _BV(PB1);
  colorControl(color);
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
  colorControl(color);
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
  // timer1_init(); // uruchom licznik
  
  DDRC |= 0x7;
  // PORTC = 0;//wszystkie led wlaczone

  DDRB |= _BV(PB1);
  // PORTB |= _BV(PB1);
  int16_t col = 0;

  while(1){
      colorControl((col++)%360);
      // breathe();
      // _delay_ms(300);
  }
}

/*
    niech anoda (PWM) odpowiada za jasność (miganie)
    natomiast - odpowiadaja za kolory (również migają)
        aby migały zamiast losować z przedziału [1,7] nalezy losowac hue 0-255
        a nastepnie w breathe zamiast delay() miec miganie pinow
*/

