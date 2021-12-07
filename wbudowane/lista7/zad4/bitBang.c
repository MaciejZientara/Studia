#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define DDRCS DDRD
#define PortCS PORTD
#define CS PD5

#define DDRMOSI DDRD
#define PortMOSI PORTD
#define MOSI PD4

#define DDRMISO DDRD
#define PortMISO PORTD
#define PinMISO PIND
#define MISO PD3

#define DDRSCK DDRD
#define PortSCK PORTD
#define SCK PD2


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init(){
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

// inicjalizacja SPI
void spi_init()
{
//     // ustaw piny MOSI, SCK i ~SS jako wyjścia (MASTER)
//        DDRB |= _BV(PB3) | _BV(PB5) | _BV(PB2);
    // ustaw pin MISO jako wyjście (SLAVE)
    DDRB |= _BV(PB4);
    // włącz SPI w trybie slave
    SPCR = _BV(SPE);
    // włącz interrupt
    SPCR |= _BV(SPIE);
    //zmienia kolejnosc wysylanych bitow: zamiast 123 wysyla 321
    SPCR |= _BV(DORD);

    SPCR |= _BV(CPOL) | _BV(CPHA);
}

// // transfer jednego bajtu
// uint8_t spi_transfer(uint8_t data)
// {
//     // rozpocznij transmisję
//     SPDR = data;
//     // czekaj na ukończenie transmisji
//     while (!(SPSR & _BV(SPIF)));
//     // wyczyść flagę przerwania
//     SPSR |= _BV(SPIF);
//     // zwróć otrzymane dane
//     return SPDR;
// }

void timer0_init(){
  TCCR0A |= _BV(WGM01);// CTC top=OCR0A
  OCR0A = 1;
  TCCR0B |= _BV(CS01);//prescaler = 8 _BV(CS02) | _BV(CS00);//prescaler = 1024
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

volatile uint8_t iter = 0, state = 1;//state 1 - SCK high, state 0 - SCK low
volatile uint8_t dataSend, dataRead;

ISR (TIMER0_COMPA_vect){
    //toggle state and SCK
    state = !state;
    PortSCK ^= _BV(SCK);

    if(!state){//high -> low = change pin
        //Changing the nth bit to x, where x = 1 or x = 0:
        //number ^= (-x ^ number) & (1UL << n);
        PortMOSI ^= (-((iter&dataSend) >0) ^ PortMOSI) & _BV(MOSI);
    }
    else{//low -> high = read pin
        dataRead |= ((PinMISO & _BV(MISO)) > 0);
        iter = iter<<1;
        if(iter==0){//overflow = transmited all bits
            TIMSK0 &= ~_BV(OCIE0A); //Timer/Counter0, Output Compare A Match Interrupt Disable
            PortSCK |= _BV(SCK);
            PortCS |= _BV(CS);
            // printf("receive %hhd, %hhd\r\n", SPDR, dataRead);
        }
        else//not last bit to read, make room for next one
            dataRead = dataRead << 1;
    }
}

ISR(SPI_STC_vect){
    printf("receive %hhd\r\n", SPDR);
    _delay_ms(2);
}

void bitBang(){
    dataRead = 0;
    iter = 1;
    state = 1;

    PortMOSI |= _BV(MOSI);
    PortSCK |= _BV(SCK);
    PortCS &= ~_BV(CS);
    _delay_us(5);

    TIMSK0 |= _BV(OCIE0A); //Timer/Counter0, Output Compare A Match Interrupt Enable
}

volatile uint8_t seconds = 0;

ISR (TIMER1_COMPA_vect){
    // spi_init();// Note that the SPI logic will be reset once the SS pin is driven high
    printf("send %hhd\r\n", seconds);
    dataSend = seconds;
    bitBang();
    seconds++;
}

FILE uart_file;


int main(){
  DDRMISO &= ~_BV(MISO);//MISO - input
  DDRMOSI |= _BV(MOSI);//MOSI - output
  DDRSCK |= _BV(SCK);//SCK - output
  DDRCS |= _BV(CS);//CS - output

  PortMISO |= _BV(MISO);
  PortCS |= _BV(CS);
  PortMOSI |= _BV(MOSI);
  PortSCK |= _BV(SCK);

  sei();

  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  timer0_init();
  timer1_init();
  spi_init();

  set_sleep_mode(SLEEP_MODE_IDLE);
  while(1){
    sleep_mode();
  }
}
