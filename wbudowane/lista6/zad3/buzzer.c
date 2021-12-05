#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


// inicjalizacja SPI
void spi_init()
{
    // ustaw piny MOSI, SCK i ~SS jako wyjścia
    DDRB |= _BV(DDB3) | _BV(DDB5) | _BV(DDB2);
    // włącz SPI w trybie master z zegarem 250 kHz
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);
    //zmienia kolejnosc wysylanych bitow: zamiast 123 wysyla 321
    // SPCR |= _BV(DORD);
}

// transfer jednego bajtu
uint8_t spi_transfer(uint8_t data)
{
    // rozpocznij transmisję
    SPDR = data;
    // czekaj na ukończenie transmisji
    while (!(SPSR & _BV(SPIF)));
    // wyczyść flagę przerwania
    SPSR |= _BV(SPIF);
    // zwróć otrzymane dane
    return SPDR;
}

void write(uint8_t output) {
    static uint8_t settings = (1 << 6) | (1 << 5);
    static uint16_t previousOutput;
    if (output != previousOutput) {
        uint8_t firstByte = settings | (output >> 4);
        uint8_t secondByte = output << 4;
        PORTB &= ~_BV(PB2);
        spi_transfer(firstByte);
        spi_transfer(secondByte);
        PORTB |= _BV(PB2);
    }
    previousOutput = output;   
}


// #define BUZZ PB5
static const int8_t BUZZ = _BV(PB5);
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define BEAT 468750
// #define TEMPO 128
// BEAT = (60*1000000)/TEMPO;

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

static const char song[] PROGMEM = "gefegedebedeaqgefegedebedeaqgeAeBeAeBegeAegeAefegefegedegqgefegedebedeaqgefegedebedeaqgeAeBeAeBegeAegeAefegefegedegqgefegefegeAeBqDeCeDeBefeBedqDeCeDeBefeBedqDeEeFeEeFeDeEeDeEeCeDeCeDeBeDqDeCeDeBefeBedqDeCeDeBefeBedqDeEeFeEeFeDeEeDeEeCeDeCeDeBeDqDeCeDeCeAeCeDqgefegedebsdsdtasaqgefegedebsdsdtasaqgeAeBeAsBsBtBegsAegsAsAtAefsgefsgsgtgedsgqgefegedebsdsdtasaqgefegedebsdsdtasaqgeAeBeAsBsBtBegsAegsAsAtAefsgefsgsgtgedsgqgefegefsgsgtgeAsBqDeCeDeBefsBsBtdsdqDeCeDeBefsBsBtdsdqDeEeFeEsFsFtFeDsEeDsEsEtEeCsDeCsDsDtDeBsDqDeCeDeBefsBsBtdsdqDeCeDeBefsBsBtdsdqDeEeFeEsFsFtFeDsEeDsEsEtEeCsDeCsDsDtDeBsDqDeCeDeCsAeAsCeDh";

static const uint32_t frequency[] = {1000000/440,1000000/493,1000000/523,1000000/587,1000000/659,1000000/698,1000000/783,1000000/880,1000000/987,1000000/1046,1000000/1174,1000000/1318,1000000/1396,1000000/1567};//A B C D E F G

void Delay(char note){
  switch(note){
  case 'a':
    _delay_us(frequency[0]);
    break;
  case 'b':
    _delay_us(frequency[1]);
    break;
  case 'c':
    _delay_us(frequency[2]);
    break;
  case 'd':
    _delay_us(frequency[3]);
    break;
  case 'e':
    _delay_us(frequency[4]);
    break;
  case 'f':
    _delay_us(frequency[5]);
    break;
  case 'g':
    _delay_us(frequency[6]);
    break;

  case 'A':
    _delay_us(frequency[7]);
    break;
  case 'B':
    _delay_us(frequency[8]);
    break;
  case 'C':
    _delay_us(frequency[9]);
    break;
  case 'D':
    _delay_us(frequency[10]);
    break;
  case 'E':
    _delay_us(frequency[11]);
    break;
  case 'F':
    _delay_us(frequency[12]);
    break;
  case 'G':
    _delay_us(frequency[13]);
    break;
  
  default:
    break;
  }
}

void playNote(char note, uint32_t duration){
  if(note == ' '){    //pause duration
    switch(duration){
    case (BEAT<<2):
      _delay_us(BEAT<<2);
      break;
    case (BEAT<<1):
      _delay_us(BEAT<<1);
      break;
    case (BEAT>>1):
      _delay_us(BEAT>>1);
      break;
    case (BEAT>>2):
      _delay_us(BEAT>>2);
      break;
    case (BEAT>>3):
      _delay_us(BEAT>>3);
      break;
    default:
      _delay_us(BEAT);
      break;
    }
    return;
  }
  uint32_t counter;
  if(note >= 'a')
    counter = (duration/(frequency[note-'a']))>>1;
  else
    counter = (duration/(frequency[note-'A'+7]))>>1;

  while(counter--){
      // BUZZ_PORT |= BUZZ;
      spi_transfer(0xff);
      Delay(note);
      // BUZZ_PORT &= ~BUZZ;
      spi_transfer(0);
      Delay(note);
  }
}

// https://pages.mtu.edu/~suits/notefreqs.html
// https://newt.phys.unsw.edu.au/jw/graphics/notes.GIF
// https://musescore.com/user/28192851/scores/6323087
// https://github.com/bhagman/Tone

void playSong(){
  for(int i=0; i<sizeof(song); i+=2){
    char note = pgm_read_byte(&song[i]), duration = pgm_read_byte(&song[i+1]);

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
      case 's'://sixteenth note = BEAT/4
        dur = BEAT >> 2;
        break;
      case 't'://thirty-second note = BEAT/4
        dur = BEAT >> 3;
        break;
      default:
        dur = BEAT;
        break;
    }

    playNote(note, dur);
  }
  _delay_ms(10000);
}


int main() {
  BUZZ_DDR |= BUZZ;
  sei();
  spi_init();

  PORTB |= _BV(PB2);
  // spi_transfer(500);

  write(0xff);


  nie dziala :(

  while (1) {
    // write(0xff);
    // _delay_us(100);
    // write(0x00);
    // _delay_us(100);
    
    // playSong();
  }
}

