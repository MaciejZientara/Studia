#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>


// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

void LEDcontrol(uint16_t dim){
    static const int8_t WAIT = 1; 
    PORTB |= _BV(PB5);//LED ON
    _delay_ms(WAIT);

    PORTB &= ~_BV(PB5);//LED OFF

    if(dim > 100)
        _delay_ms(WAIT);
    if(dim > 200)
        _delay_ms(WAIT << 1);
    if(dim > 300)
        _delay_ms(WAIT << 2);
    if(dim > 400)
        _delay_ms(WAIT << 3);
    // if(dim > 500)
    //     _delay_ms(WAIT << 4);
}

int main()
{
  DDRB |= _BV(PB5);// LED DDR
  // zainicjalizuj ADC
  adc_init();
  // mierz napięcie
  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    LEDcontrol(v);//*2
  }
}

