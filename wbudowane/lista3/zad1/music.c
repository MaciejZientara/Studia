#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// #define BUZZ PB5
static const int8_t BUZZ = _BV(PB5);
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define TEMPO 120
static const int BEAT = (60*1000000)/TEMPO;

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

static const char song[] PROGMEM = "D5q  eF5q  hD5q  e";
// "G3e   G3e   G3e   G3e   A3e   G3e   G3e   G3e   G3e   A3e   G3e   G3e   G3e   G3e   A3e   G3e   G3e   G3e   G3e   A3e   "
// "A2wB2wC3wD3wE3wF3wG3w"
// "A4w  wB4w  wC4w  wD5w  wE5w  wF5w  wG5w  w"
// "A4wB4wC4wD4wE4wF4wG4w"
// "C0wC0wC0wC0wC0w  w  w  wC1wC1wC1wC1wC1w  w  w  wC2wC2wC2wC2wC2w  w  w  wC3wC3wC3wC3wC3w"
// "C0wD0wE0wF0wG0wA0wB0wC6wD6wE6wF6wG6wA6wB6w"

//the best? A4, B, C, D5, F5, G5

void Delay(uint16_t d){
  do{
    _delay_us(1);
  }while(--d);
}

void playNote(uint16_t frequency, uint32_t duration){
  uint16_t counter = (duration/frequency)>>1;

  while(counter--){
      BUZZ_PORT |= BUZZ;
      Delay(frequency);
      BUZZ_PORT &= ~BUZZ;
      Delay(frequency);
  }
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
      playNote(1000000/notes['A'-note]['0'-octave], dur);
    else
      Delay(dur);
  }
  _delay_ms(10000);
}


int main() {
  BUZZ_DDR |= BUZZ;
  while (1) {
    playSong();
  }
}

