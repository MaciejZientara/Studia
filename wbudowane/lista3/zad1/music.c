#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define TEMPO 120
static const int BEAT = (60*1000000)/TEMPO;
volatile int16_t FREQUENCY = 0;

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

static const char song[] PROGMEM = {
"G3e  qG3e  qG3e  qG3e  qA3e  q"
};

ISR (TIMER1_OVF_vect){    // Timer1 ISR
  BUZZ_PORT ^= _BV(BUZZ);
  TCNT1 = FREQUENCY;
}

void playSong(){
  for(int i=0; i<sizeof(song); i+=3){
    BUZZ_PORT &= ~_BV(BUZZ);
    char note = pgm_read_byte(&song[i]), octave = pgm_read_byte(&song[i+1]), duration = pgm_read_byte(&song[i+2]);
    if(note!=' ' && octave!=' '){
      FREQUENCY = 65535 - 15625/(notes['A'-note]['0'-octave]);
      sei();  //turn on interrupts
      TCNT1 = 65535;
    }

    switch (duration){
      case 'w'://whole note = 4 BEATS
        _delay_us(BEAT << 2);
        cli();  //turn off interrupts
        _delay_us(BEAT << 2);
        break;
      case 'h'://half note = 2 BEATS
        _delay_us(BEAT << 1);
        cli();  //turn off interrupts
        _delay_us(BEAT << 1);
        break;
      case 'q'://quarter note = 1 BEAT
        _delay_us(BEAT);
        cli();  //turn off interrupts
        _delay_us(BEAT);
        break;
      case 'e'://eighth note = BEAT/2
        _delay_us(BEAT >> 1);
        cli();  //turn off interrupts
        _delay_us(BEAT >> 1);
        break;
      default:
        _delay_us(BEAT);
        cli();  //turn off interrupts
        _delay_us(BEAT);
        break;
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
  BUZZ_DDR |= _BV(BUZZ);

  TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);  // Timer mode with 1024 prescaler
	TIMSK1 = (1 << TOIE1);   // Enable timer1 overflow interrupt(TOIE1)

	// sei();        // Enable global interrupts by setting global interrupt enable bit in SREG

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
