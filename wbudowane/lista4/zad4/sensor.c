#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB0
#define BTN_DDR DDRB
#define BTN_PORT PORTB
#define BTN_PIN PINB

volatile int8_t isIRenable = 1;

void enableIRLED(){
  // COM1A = 01   -- toggle on compare match
  TCCR1A |= _BV(COM1A0);
}

void disableIRLED(){
  // COM1A = 00   -- Normal port operation, OC1A disconnected.
  TCCR1A &= ~_BV(COM1A0);
  PORTB &= ~_BV(PB1);
}

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 1100 -- CTC top=ICR1
  // CS1   = 001  -- no prescaler
  // wzór: datasheet 20.12.3 str. 164
  // częstotliwość 16e6/(1+421) ~=~ 37.9 kHz
  // ICR1  = 421
  ICR1 = 421;
  enableIRLED();
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}

void timer0_init(){
  TCCR0A = _BV(WGM01);  // CTC mode, OCRA = TOP
 	TCCR0B = _BV(CS00) | _BV(CS01);  // Timer mode with 64 prescler
	TIMSK0 = _BV(OCIE0A) ;   // Enable timer0 Output Compare Match A Interrupt
  // wzór: datasheet 20.12.3 str. 164
  // częstotliwość 16e6/(64*(1+149)) ~=~ 1e6/600 Hz
  // OCR0A  = 149
  OCR0A = 149;
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
}

ISR (TIMER0_COMPA_vect){    // Timer0 ISR
  TCCR1A ^= _BV(COM1A0);
  PORTB &= ~_BV(PB1);
  // if(isIRenable)
  //   disableIRLED();
  // else
  //   enableIRLED();
  // isIRenable = !isIRenable;
}

int main()
{
  LED_DDR = _BV(LED);
  // BTN_DDR ma już zgaszony bit dla PB0 czyli BTN_PIN
  BTN_PORT = _BV(BTN);//pull up

  // uruchom licznik
  timer1_init();
  timer0_init();
  
  while(1){
    _delay_ms(10);
    if(!(BTN_PIN & _BV(BTN)))
      LED_PORT |= _BV(LED);
    else
      LED_PORT &= ~_BV(LED);
  }
}
