#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>
#include <avr/pgmspace.h>

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define TEMPO 120
static const int BEAT = 937500/TEMPO;
//16'000'000 / (preScalar*BEAT) = (TEMPO/60), where TCNT1 = 65535 - BEAT

#define BUZZER 1
static const int8_t PORTS[BUZZER] = {_BV(PB5)/*,_BV(PB4)*/};
static int DELAYS[BUZZER];          //TIMER1    set in polling, how long a note for a given buzzer lasts
static int8_t FLAGS;                //TIMER1    set in interrupt, buzzer note done
static int COUNTERS[BUZZER];        //TIMER0    set in polling, -- in TIMER0, if==0 in polling
static int16_t FREQUENCIES[BUZZER]; //TIMER0    set in polling, used to set COUNTERS
static int INDEXES[BUZZER];         //          set in polling, which index of song we need to read for given buzzer
static int8_t LAST;                 //TIMER1    used to set flag for the buzzer that note has ended - number of the buzzer

//frequency in Hz
static const int16_t notes[][9]={                    // C D E F G A B : 0-8
  {27, 55, 110, 220, 440, 880, 1760, 3520, 7040},   //A
  {30, 61, 123, 246, 493, 987, 1975, 3951, 7902},   //B
  {16, 32, 65,  130, 261, 523, 1046, 2093, 4186},   //C
  {18, 36, 73,  146, 293, 587, 1174, 2349, 4698},   //D
  {20, 41, 82,  164, 329, 659, 1318, 2637, 5274},   //E
  {22, 43, 87,  174, 349, 698, 1396, 2793, 5587},   //F
  {24, 49, 98,  196, 392, 783, 1567, 3135, 6271}    //G
};// ['A'-note]['0'-octave]

static const char song[BUZZER][30] PROGMEM = {
  "G3eG3eG3eA3eB3hG3eG3eG3eA3eB3h"
};


ISR (TIMER0_OVF_vect){    // Timer0 ISR
  for(int8_t i=0; i<BUZZER; i++){
    COUNTERS[i]--;
  }
}

ISR (TIMER1_OVF_vect){    // Timer1 ISR
  FLAGS|=_BV(LAST);

  for(int i=0; i<BUZZER; i++)
    DELAYS[i]-=DELAYS[LAST];

  int16_t tmp = 10000;
  for(int i=0; i<BUZZER; i++)
    if(DELAYS[i]>0){
      if(DELAYS[i]<tmp){
        LAST = i;
        tmp = DELAYS[i];
      }
    }
    else
      FLAGS|=_BV(i);

  TCNT1 = 65535-DELAYS[LAST];
}

void playSong(){
  FLAGS = (1<<BUZZER)-1;//make all buzzers receive first note
  for(int i=0; i<BUZZER; i++) INDEXES[i]=0;

  while(1){
    if(FLAGS){
      int8_t finished = 1;
      for(int B=0; B<BUZZER; B++){
        if((FLAGS & _BV(B)) && (INDEXES[B]<sizeof(song[B]))){
          char note = pgm_read_byte(&song[B][INDEXES[B]]), octave = pgm_read_byte(&song[B][INDEXES[B]+1]), duration = pgm_read_byte(&song[B][INDEXES[B]+2]);
          FREQUENCIES[B] = notes['A'-note]['0'-octave] * 2;
          INDEXES[B]+=3;
          finished = 0;
          switch (duration){
          case 'w'://whole note = 4 BEATS
            DELAYS[B] = BEAT << 2;
            break;
          case 'h'://half note = 2 BEATS
            DELAYS[B] = BEAT << 1;
            break;
          case 'q'://quarter note = 1 BEAT
            DELAYS[B] = BEAT;
            break;
          case 'e'://eighth note = BEAT/2
            DELAYS[B] = BEAT >> 1;
            break;
          default:
            DELAYS[B] = BEAT;
            break;
          }
        }
      }
      if(finished)
        break;
    }
    for(int B=0; B<BUZZER; B++){
      if(COUNTERS[B]<=0){
        BUZZ_PORT ^= PORTS[B];
        COUNTERS[B] = 62500/FREQUENCIES[B]; //  16'000'000/256 = 62500
      }
    }
  }
  //   /*
  //     frequency = Hz * 2
  //     16'000'000 / (preScalar * frequency) = x
  //     TimerMax - x = timer init counter
  //     Timer1 max = 65535
  //     trzeba mieć 2 razy więcej przerwań niż Hz bo w połowie należy wyłączyć brzęczyk
  //   */
  _delay_ms(5000);
}

// https://newt.phys.unsw.edu.au/jw/graphics/notes.GIF
// https://musescore.com/user/189765/scores/5001579

int main() {
  for(int i=0; i<BUZZER; i++)
    BUZZ_DDR |= PORTS[i];


  //read 108, 132+134 from datasheet
  TCCR0A = 0x00;
  TCCR0B = 0x00; //no prescaler
  TIMSK0 = (1 << TOIE0); // Enable timer0 overflow interrupt(TOIE0)

  TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);  // Timer mode with 1024 prescsler
	TIMSK1 = (1 << TOIE1);   // Enable timer1 overflow interrupt(TOIE1)


	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG

  while (1) {
    playSong();
  }
}


/*
  static const char helloworld[] PROGMEM = "Hello world!\r\n";
  while(1) {
    for (uint8_t i = 0; i < sizeof(helloworld); i++)
      putchar(  pgm_read_byte(&helloworld[i])   );
  }
*/
