#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//counter to ile cykli zegara przy okreslonym wypelnieniu
//ledMask to maska do PORT uzywana zapalania okreslonych diod
//state odpowiada za wypelnienie (OCR0A = 1<<state)
//w zaleznosci od ud (1=up,0=down) wykonuje state++/state--
volatile uint8_t counter, state, ledMask, ud;
// przypisuje counter=COUNTER
#define COUNTER 60

#define PORT PORTB
#define DDR DDRB

void timer_init(){
  // ustaw tryb licznika
  // fast PWM mode
  // WGM0  = 011 -- fast PWM top=0xFF
  // CS0   = 010  -- prescaler 8

  // częstotliwość 4e6/(8*(1+255)) ~=~ 1953 Hz
  OCR0A = 1;
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS01);

  // wlacz interrupt na overflow i compare match
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
}

void nextMask(){//moze ustawiac kody greya?
  cli();//poczatek sekcji krytycznej
  counter = COUNTER;
  PORT &= ~ledMask;
  ledMask = (ledMask+1) & 0b1111;
  ledMask += (ledMask == 0);//nie chce miec sytuacji gdzie wszystkie diody zgaszone
  sei();//koniec sekcji krytycznej
}

void nextState(){
  if(state==0){//dol licznika
    ud = 1;//chce liczyc w gore
    nextMask();
  }
  if(state==7){//gora licznika
    ud = 0;//chce liczyc w dol
  }
  if(ud == 1)//up
    state++;
  else//down
    state--;
  counter = COUNTER;
  // OCR0A = (state ? 1<<state : 1);
  OCR0A = (2<<state)-2;
}

ISR (TIM0_COMPA_vect){
  PORT &= ~ledMask;
}
ISR (TIM0_OVF_vect){
  PORT |= ledMask;
  counter--;
}

int main(){
  timer_init(); // uruchom licznik
  sei();
  
  //Piny B 4321
  DDR |= 0b00001111;

  set_sleep_mode(SLEEP_MODE_IDLE);
  while(1) {
    if(counter)
      sleep_mode();
    else{
      nextState();
    }
  }
}

