#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define BTN PB2
#define BTN_DDR DDRB
#define BTN_PORT PORTB
#define BTN_PIN PINB

#define LED PA1
#define LED_DDR DDRA
#define LED_PORT PORTA


void setLED(int8_t state){
  if(state)
    LED_PORT |= _BV(LED);
  else
    LED_PORT &= ~_BV(LED);
}

int8_t getState(uint64_t *tab, uint16_t place){
  return (tab[place>>6] & (uint64_t)(1ULL<<(place&63)))>=1;
}

void setState(uint64_t *tab, uint16_t place, uint8_t state){
  if(state)
    tab[place>>6] |= (uint64_t)(1ULL<<(place&63));
  else
    tab[place>>6] &= ~(uint64_t)(1ULL<<(place&63));
}

void timer1_init(){
  // ustaw tryb licznika
  // WGM1  = 1100 -- CTC top=ICR1
  // CS1   = 010  -- prescalar 8
  // wzór: datasheet 20.12.3 str. 164
  // częstotliwość 1e6/8*(1+124) ~=~ 1000 Hz
  // ICR1  = 124
  ICR1 = 124;
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS11);
  TIMSK1 = _BV(OCIE1A); //Timer/Counter1, Output Compare A Match Interrupt Enable
  sei();
}


ISR (TIM1_COMPA_vect ){
  static uint64_t cycle[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  static uint16_t read=0, write=1000;

  setLED(getState(cycle,read));
  setState(cycle,write,!(BTN_PIN & _BV(BTN)));

  if((++read)==1024)read=0;
  if((++write)==1024)write=0;
}

int main() {
  LED_DDR |= _BV(LED);
  BTN_DDR &= ~_BV(BTN);
  BTN_PORT |= _BV(BTN);

  timer1_init();

  set_sleep_mode(SLEEP_MODE_IDLE);

  while (1) {
    sleep_mode();
  }
}
