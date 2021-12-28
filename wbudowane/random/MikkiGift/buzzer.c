#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

static int8_t BUZZ = _BV(PB3);
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

// tempo 120, octave 4/5
// static const uint8_t OCRA[] PROGMEM = {124, 238, 178, 158, 141, 125, 118, 105};
// static const uint8_t TOGGLE[] PROGMEM = {62, 32, 43, 48, 55, 61, 65, 73};
// tempo 120, octave 5/6
// static const uint8_t OCRA[] PROGMEM = {68, 118, 88, 78, 70, 62, 58, 52};
// static const uint8_t TOGGLE[] PROGMEM = {112, 65, 87, 97, 110, 123, 130, 146};
// tempo 140, octave 4/5
// static const uint8_t OCRA[] PROGMEM = {124, 238, 178, 158, 141, 125, 118, 105};
// static const uint8_t TOGGLE[] PROGMEM = {53, 27, 37, 42, 47, 52, 56, 62};
// tempo 140, octave 5/6
// static const uint8_t OCRA[] PROGMEM = {68, 118, 88, 78, 70, 62, 58, 52};
// static const uint8_t TOGGLE[] PROGMEM = {96, 56, 74, 83, 94, 105, 112, 125};
// tempo 160, octave 4/5
// static const uint8_t OCRA[] PROGMEM = {124, 238, 178, 158, 141, 125, 118, 105};
// static const uint8_t TOGGLE[] PROGMEM = {46, 24, 32, 36, 41, 46, 49, 55};
// tempo 160, octave 5/6
static const uint8_t OCRA[] PROGMEM = {68, 118, 88, 78, 70, 62, 58, 52};
static const uint8_t TOGGLE[] PROGMEM = {84, 49, 65, 73, 82, 92, 98, 110};

static const uint8_t song[] PROGMEM = {2, 2, 2, 1, 2, 3, 4, 4, 4, 3, 4, 5, 6, 7, 6, 6, 5, 5, 4, 4, 3, 3, 0, 0, 2, 2, 2, 1, 2, 3, 4, 4, 4, 3, 4, 5, 6, 7, 6, 6, 5, 5, 4, 4, 3, 3, 0, 0, 6, 6, 6, 5, 4, 3, 2, 2, 2, 1, 2, 4, 6, 6, 6, 5, 4, 3, 2, 2, 2, 1, 2, 4, 6, 7, 6, 5, 4, 5, 6, 7, 6, 5, 4, 5, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 6, 6, 6, 5, 4, 3, 2, 2, 2, 1, 2, 4, 6, 6, 6, 5, 4, 3, 2, 2, 2, 1, 2, 4, 6, 7, 6, 5, 4, 5, 6, 7, 6, 5, 4, 5, 6, 6, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2};
// '0' - pausa

// F4 C4 G4 A4 B4 C5 D5 00 czyli 8 możliwości

/*
http://www.religijne.org/galeria/Dzisiaj_w_Betlejem_F-dur_185619_1.pdf
https://jythonmusic.files.wordpress.com/2016/01/pitchesonstaff.png?w=584
https://harmonicalearning.com/assets/images/notation-of-duration-888x799.jpg
*/

void timer0_init(){
  // CTC top=OCR0A
  // prescalar = 64
  /* 
  TCCR0B = 0b001; PRE = 1
  TCCR0B = 0b010; PRE = 8
  TCCR0B = 0b011; PRE = 64
  TCCR0B = 0b100; PRE = 256
  TCCR0B = 0b101; PRE = 1024
  */
  // wzór: datasheet 20.12.3 str. 164
  // częstotliwość FreqCPU/[PRE*(1+OCRA)] ~=~ x Hz

  // FreqCPU/(PRE * Hz) = 1+OCRA
  // FreqCPU = 4e6

  OCR0A = 255;
  TCCR0A = _BV(WGM01);
  TCCR0B = 0b011;
  TIMSK0 = _BV(OCIE0A); //Timer/Counter0, Output Compare A Match Interrupt Enable
  sei();
}

volatile uint8_t toggle_count;

void nextNote(){
  static uint16_t iter = 0;
  if(iter == sizeof(song))
    iter = 0;
  uint8_t note = pgm_read_byte(&song[iter++]);

  OCR0A = pgm_read_byte(&OCRA[note]);
  toggle_count = pgm_read_byte(&TOGGLE[note]);

  if(note == 0)
    BUZZ = 0;
  else
    BUZZ = _BV(PB3);
}

ISR(TIM0_COMPA_vect){
  toggle_count--;
  BUZZ_PORT ^= BUZZ;
}


int main() {
  BUZZ_DDR |= BUZZ;

  BUZZ_PORT ^= BUZZ;
  toggle_count = 1;

  timer0_init();
  set_sleep_mode(SLEEP_MODE_IDLE);

  while (1) {
    sleep_mode();
    if(toggle_count == 0)
      nextNote();
  }
}

