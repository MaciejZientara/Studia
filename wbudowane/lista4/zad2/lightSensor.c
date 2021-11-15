#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#define LED PB1
#define WAIT 15

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0) | 0x4; // referencja AVcc, wejście ADC4
  DIDR0   = _BV(ADC4D); // wyłącz wejście cyfrowe na ADC4
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

void LEDcontrol(uint16_t dim){
  if(dim < 200){
    PORTB &= ~_BV(LED);//LED OFF
    return;
  }
  if(dim > 600){
    PORTB |= _BV(LED);//LED ON
    return;
  }
  
  PORTB |= _BV(LED);//LED ON
  _delay_ms(1);

  PORTB &= ~_BV(LED);//LED OFF

  if(dim < 600)
      _delay_ms(1);
  if(dim < 500)
      _delay_ms(1 << 1);
  if(dim < 400)
      _delay_ms(1 << 2);
  if(dim < 300)
      _delay_ms(1 << 3);
}

int main()
{
  DDRB |= _BV(LED);// LED DDR
  // zainicjalizuj ADC
  adc_init();
  // mierz napięcie
  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    LEDcontrol(v);
  }
}
