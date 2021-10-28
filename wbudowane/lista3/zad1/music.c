#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>
#include <avr/pgmspace.h>

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define TEMPO 120
static const int BEAT = 1000000/TEMPO;

#define BUZZER 2
static const int8_t PORTS[BUZZER] = {_BV(PB5),_BV(PB4)};
static int DELAYS[BUZZER];          //TIMER1    set in polling, how long a note for a given buzzer lasts
static int8_t FLAGS;                //TIMER1    set in interrupt, buzzer note done
static int COUNTERS[BUZZER];        //TIMER0    set in polling, -- in TIMER0, if==0 in polling
static int16_t FREQUENCIES[BUZZER]; //TIMER0    set in polling, used to set COUNTERS

//frequency in Hz
static const int16_t notes[][]={                    // C D E F G A B : 0-8
  {27, 55, 110, 220, 440, 880, 1760, 3520, 7040},   //A
  {30, 61, 123, 246, 493, 987, 1975, 3951, 7902},   //B
  {16, 32, 65,  130, 261, 523, 1046, 2093, 4186},   //C
  {18, 36, 73,  146, 293, 587, 1174, 2349, 4698},   //D
  {20, 41, 82,  164, 329, 659, 1318, 2637, 5274},   //E
  {22, 43, 87,  174, 349, 698, 1396, 2793, 5587},   //F
  {24, 49, 98,  196, 392, 783, 1567, 3135, 6271}    //G
};// ['A'-note]['0'-octave]

static const char song[][] PROGMEM = {
  ""
};

void playSong(){
  FLAGS = (1<<BUZZER)-1;//make all buzzers receive first note
  for(int i=0; i<sizeof(song); i+=3){
    char note = pgm_read_byte(&song[i]), octave = pgm_read_byte(&song[i+1]), duration = pgm_read_byte(&song[i+2]);
    int16_t frequency = notes['A'-note]['0'-octave] * 2;

    _delay_us(BEAT * duration/*TO FIX, duration should have switch case!*/);

    /*
      frequency = Hz * 2
      16'000'000 / (preScalar * frequency) = x
      TimerMax - x = timer init counter

      Timer1 max = 65535

      trzeba mieć 2 razy więcej przerwań niż Hz bo w połowie należy wyłączyć brzęczyk
    */

  }


  _delay_ms(5000);
}

#define TONE(step, delay) \
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step) / 2; i++) { \
      BUZZ_PORT |= _BV(BUZZ); \
      _delay_us(step); \
      BUZZ_PORT &= ~_BV(BUZZ); \
      _delay_us(step); \
    }

// https://newt.phys.unsw.edu.au/jw/graphics/notes.GIF
// https://musescore.com/user/189765/scores/5001579

int main() {
  BUZZ_DDR |= _BV(BUZZ);
  while (1) {
    TONE(500, 1000);
    TONE(250, 1000);
  }
}


/*
  static const char helloworld[] PROGMEM = "Hello world!\r\n";
  while(1) {
    for (uint8_t i = 0; i < sizeof(helloworld); i++)
      putchar(  pgm_read_byte(&helloworld[i])   );
  }
*/
