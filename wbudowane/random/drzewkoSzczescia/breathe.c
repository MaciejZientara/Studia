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
//state to maska do PORT uzywana zapalania okreslonych diod
volatile uint8_t counter, state;

#define PORT PORTB
#define DDR DDRB

#define N 6
#define WAIT 2

void timer_init(){
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

void breathe(){ //zwiekszamy czas trwania zapalonej diody zmieniając wartość OCR1A
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

//TODO dodac obsluge zmiany wypelnienia PWM
// to ta funkcja powinna korzystac z counter,
// nextState powinien miec wlasny counter


//to powinno byc dopiero po calym jednym oddechu!
void nextState(){//moze ustawiac kody greya?
  cli();//poczatek sekcji krytycznej
  counter = 10;//                                   TODO
  PORT &= ~state;
  state>>=1;//dopasowanie do pinow kontrolera
  state = (state+1) & 0xf;
  state += (state > 0);//nie chce miec sytuacji gdzie wszystkie diody zgaszone
  state<<=1;//dopasowanie do pinow kontrolera
  sei();//koniec sekcji krytycznej
}

ISR (TIM0_COMPA_vect){
  PORT |= state;
}
ISR (TIM0_OVF_vect){
  PORT &= ~state;
  counter--;
}

int main(){
  timer_init(); // uruchom licznik
  
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

