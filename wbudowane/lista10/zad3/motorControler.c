#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 010  -- prescaler 8
  // ICR1  = 15624
  // częstotliwość 16e6/(8*(1+3905)) ~=~ 512 Hz
  // wzór: datasheet 20.12.3 str. 164
  ICR1 = 3905;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS11) /*| _BV(CS12)*/;
  // ustaw pin OC1A (PB1), OC1B (PB2) jako wyjście
  DDRB |= _BV(PB1) | _BV(PB2);
}

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

int main()
{
  // uruchom licznik
  timer1_init();
    
  // zainicjalizuj ADC
  adc_init();

  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    PORTB = 0;//????

    float helper = ICR1 / 512.0;
    if(v < 512){
      TCCR1A = _BV(COM1A1) | _BV(WGM11);
      float tmp = helper * (512.0-v);
      //ICR1 * (512.0-v) / 512.0;
      OCR1A = (uint16_t)tmp;// ustaw wypełnienie
    }
    else{
      TCCR1A = _BV(COM1B1) | _BV(WGM11);
      float tmp = helper * (v-512.0);
      //ICR1 * (v-512.0) / 512.0;
      OCR1B = (uint16_t)tmp;// ustaw wypełnienie
    }
  }
}

