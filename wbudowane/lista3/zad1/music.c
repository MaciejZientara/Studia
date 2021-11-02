#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// #define BUZZ PB5
static const int8_t BUZZ = _BV(PB5);
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define CPU_F 16000000
#define TEMPO 120
static const int BEAT = (60*1000)/TEMPO;
volatile int16_t COUNTER = 0;

//frequency in Hz
static const uint16_t notes[][9]={                    // C D E F G A B : 0-8
  {27, 55, 110, 220, 440, 880, 1760, 3520, 7040},   //A
  {30, 61, 123, 246, 493, 987, 1975, 3951, 7902},   //B
  {16, 32, 65,  130, 261, 523, 1046, 2093, 4186},   //C
  {18, 36, 73,  146, 293, 587, 1174, 2349, 4698},   //D
  {20, 41, 82,  164, 329, 659, 1318, 2637, 5274},   //E
  {22, 43, 87,  174, 349, 698, 1396, 2793, 5587},   //F
  {24, 49, 98,  196, 392, 783, 1567, 3135, 6271}    //G
};// ['A'-note]['0'-octave]

static const char song[] PROGMEM = 
  // "A3wB3wC4wD4wE4wF4wG4w"
"C0wC0wC0wC0wC0w  w  w  wC1wC1wC1wC1wC1w  w  w  wC2wC2wC2wC2wC2w  w  w  wC3wC3wC3wC3wC3w"
// "C0wD0wE0wF0wG0wA0wB0wC6wD6wE6wF6wG6wA6wB6w"
// "G3e   G3e   G3e   G3e   A3e   G3e   G3e   G3e   G3e   A3e   G3e   G3e   G3e   G3e   A3e   G3e   G3e   G3e   G3e   A3e   "
;



void StartTimer(){
  TIMSK1 |= _BV(OCIE1A);// enable the interrupt
}

void StopTimer(){
  TIMSK1 &= ~_BV(OCIE1A);// disable the interrupt
}

int8_t isPlaying(){
  return TIMSK1 & _BV(OCIE1A);// are interrupts enabled?
}



ISR(TIMER1_COMPA_vect){
  if(COUNTER > 0){
    BUZZ_PORT ^= BUZZ;
    COUNTER--;
  }
  else{
    StopTimer();
    BUZZ_PORT &= ~BUZZ;// keep pin low after stop
  }
}



// frequency in hertz, duration in milliseconds
void playNote(uint16_t frequency, uint32_t duration){
  uint8_t prescalarbits = 0b001; //no prescalar
  int32_t toggle_count = 0;
  uint32_t ocr = 0;

  ocr = CPU_F / frequency / 2 - 1;

  if (ocr > 0xffff){
    ocr = CPU_F / frequency / 2 / 64 - 1;
    prescalarbits = 0b011; //prescalar = 64
  }

  TCCR1B = (TCCR1B & 0b11111000) | prescalarbits; //set proper prescalar

  // Calculate the toggle count
  if (duration > 0)
    toggle_count = 2 * frequency * duration / 1000;
  else
    toggle_count = -1;

  OCR1A = ocr;
  COUNTER = toggle_count;
  StartTimer();
}

// https://pages.mtu.edu/~suits/notefreqs.html
// https://newt.phys.unsw.edu.au/jw/graphics/notes.GIF
// https://musescore.com/user/189765/scores/5001579
// https://github.com/bhagman/Tone

void playSong(){
  for(int i=0; i<sizeof(song); i+=3){
    char note = pgm_read_byte(&song[i]), octave = pgm_read_byte(&song[i+1]), duration = pgm_read_byte(&song[i+2]);

    uint32_t dur;
    switch (duration){
      case 'w'://whole note = 4 BEATS
        dur = BEAT << 2;
        break;
      case 'h'://half note = 2 BEATS
        dur = BEAT << 1;
        break;
      case 'q'://quarter note = 1 BEAT
        dur = BEAT;
        break;
      case 'e'://eighth note = BEAT/2
        dur = BEAT >> 1;
        break;
      default:
        dur = BEAT;
        break;
    }

    if(note!=' ' && octave!=' ')//not pause
      playNote(notes['A'-note]['0'-octave], dur);
    else
      _delay_ms(BEAT<<1/*dur*/); // TODO wait for dur, not BEAT<<1

    while(isPlaying());//wait to stop playing current note
  }
  // _delay_ms(10000);
}


int main() {
  BUZZ_DDR |= BUZZ;

  TCCR1A = 0;
	TCCR1B = _BV(CS10) | _BV(WGM12); //no prescalar
  //RCCR1A=0 + WGM12 in TCCR1B = mode4 in table 16-4 page 133 data sheet

	sei(); // Enable global interrupts by setting global interrupt enable bit in SREG

  while (1) {
    playSong();
  }
}

