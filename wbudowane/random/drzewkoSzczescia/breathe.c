#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/*
overflow timera nastepuje w regularnych odstepach czasu
moge zrobic counter zmniejszany przy kazdym overflow
w while usypiam jesli counter >0
else zmieniam ustawienie portu (ktore diody zapalam)
*/

//counter to ile cykli zegara przy okreslonym wypelnieniu
//ledMask to maska do PORT uzywana zapalania okreslonych diod
//state odpowiada za wypelnienie (OCR0A = 1<<state)
//w zaleznosci od ud (1=up,0=down) wykonuje state++/state--
volatile uint8_t counter, state, ledMask, ud;
// przypisuje counter=COUNTER
#define COUNTER 10

#define PORT PORTB
#define DDR DDRB

void timer_init(){
  // ustaw tryb licznika
  // fast PWM mode
  // WGM0  = 011 -- fast PWM top=0xFF
  // CS0   = 100  -- prescaler 256

  // częstotliwość 4e6/(256*(1+255)) = 61 Hz

  OCR0A = 1;
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  TCCR0B = /*_BV(WGM02) |*/ _BV(CS02);

  // wlacz interrupt na overflow i compare match
  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
}

void nextMask(){//moze ustawiac kody greya?
  cli();//poczatek sekcji krytycznej
  counter = COUNTER;
  PORT &= ~ledMask;
  ledMask>>=1;//dopasowanie do pinow kontrolera
  ledMask = (ledMask+1) & 0xf;
  ledMask += (ledMask > 0);//nie chce miec sytuacji gdzie wszystkie diody zgaszone
  ledMask<<=1;//dopasowanie do pinow kontrolera
  sei();//koniec sekcji krytycznej
}

void nextState(){
  if(state==0){//dol licznika
    ud = 1;//chce liczyc w gore
    nextMask();
  }
  if(state==7){//gora licznika
    ud = 0;//chce liczyc w dol
    nextMask();
  }
  if(ud == 1)//up
    state++;
  else//down
    state--;
  // OCR0A = 1<<state;
  OCR0A = (2<<state)-1;
}

ISR (TIM0_COMPA_vect){
  PORT |= ledMask;
}
ISR (TIM0_OVF_vect){
  PORT &= ~ledMask;
  counter--;
}

int main(){
  timer_init(); // uruchom licznik
  sei();
  
  //Piny B 4321
  DDR |= 0b11110;

  set_sleep_mode(SLEEP_MODE_IDLE);
  while(1) {
    if(counter)
      sleep_mode();
    else{
      nextState();
    }
  }
}

