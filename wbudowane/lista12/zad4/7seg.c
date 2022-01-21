#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define LATCH PB1
#define LATCH_PORT PORTB
#define LATCH_DDR DDRB

#define ON_OFF PB2
#define ON_OFF_PORT PORTB
#define ON_OFF_DDR DDRB


uint8_t patterns[] = {
//    ABCDEFG.
    0b11111100, //0
    0b01100000, //1
    0b11011010, //2
    0b11110010, //3
    0b01100110, //4
    0b10110110, //5
    0b10111110, //6
    0b11100000, //7
    0b11111110, //8
    0b11110110  //9
};

void Latch(){
  LATCH_PORT |= _BV(LATCH);
  _delay_us(1);
  LATCH_PORT &= ~_BV(LATCH);
}

// inicjalizacja SPI
void spi_init()
{
    // ustaw piny MOSI, SCK i ~SS jako wyjścia
    DDRB |= _BV(DDB3) | _BV(DDB5) | _BV(DDB2);
    // włącz SPI w trybie master z zegarem 250 kHz -> 16e6/4 Hz
    SPCR = _BV(SPE) | _BV(MSTR);// | _BV(SPR1);
    //zmienia kolejnosc wysylanych bitow: zamiast 123 wysyla 321
    SPCR |= _BV(DORD);
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

void timer1_init(){
  // ustaw tryb licznika
  // WGM1  = 0100 -- CTC top=OCR1A
  // CS1   = 100  -- prescalar 256
  // wzór: datasheet 20.12.3 str. 164
  // częstotliwość 16e6/256*(1+62499) = 1 Hz
  // OCR1A  = 62499
  OCR1A = 62499;
  TCCR1B |= _BV(WGM12) | _BV(CS12);
  TIMSK1 |= _BV(OCIE1A); //Timer/Counter1, Output Compare A Match Interrupt Enable
}

volatile uint8_t seconds = 0;

void setLED(){
  spi_transfer(patterns[seconds]);
  Latch();
}

void update(){
  seconds++;
  if(seconds==10)
    seconds=0;
  setLED();
}

void debugBlink(){
  PORTB |= _BV(PB5);
  _delay_ms(100);
  PORTB &= ~_BV(PB5);
  _delay_ms(100);
}

ISR (TIMER1_COMPA_vect){
  update();
  // debugBlink();
}

int main()
{
  DDRB |= _BV(PB5);

  debugBlink();
  debugBlink();
  debugBlink();
  debugBlink();

  sei();

  timer1_init();
  spi_init();

  setLED();

  set_sleep_mode(SLEEP_MODE_IDLE);
  while(1) {
    sleep_mode();
  }
}

