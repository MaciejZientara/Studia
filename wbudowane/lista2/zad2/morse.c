#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB
#define BTN_DDR DDRC
#define BTN_PORT PORTC
#define BTN_PIN PINC

void toggleLED(){
  LED_PORT ^= _BV(LED);
}

static const int8_t morseCode[26][5] = {
  {1,3,0,0,0},  //A
  {3,1,1,1,0},  //B
  {3,1,3,1,0},  //C
  {3,1,1,0,0},  //D
  {1,0,0,0,0},  //E
  {1,1,3,1,0},  //F
  {3,3,1,0,0},  //G
  {1,1,1,1,0},  //H
  {1,1,0,0,0},  //I
  {1,3,3,3,0},  //J
  {3,1,3,0,0},  //K
  {1,3,1,1,0},  //L
  {3,3,0,0,0},  //M
  {3,1,0,0,0},  //N
  {3,3,3,0,0},  //O
  {1,3,3,1,0},  //P
  {3,3,1,3,0},  //Q
  {3,1,3,0,0},  //R
  {1,1,1,0,0},  //S
  {3,0,0,0,0},  //T
  {1,1,3,0,0},  //U
  {1,1,1,3,0},  //V
  {1,3,3,0,0},  //W
  {3,1,1,3,0},  //X
  {3,1,3,3,0},  //Y 
  {3,3,1,1,0}   //Z
};

// https://commons.wikimedia.org/wiki/File:International_Morse_Code.svg


int8_t isButtonPressed(uint8_t pin, uint8_t btn){
    static const int8_t waitTime = 10, tests = 100;
    int8_t count = 0;
    while(!(pin & btn) && tests > count){
        _delay_us(waitTime);
        count++;
    }
    return count==tests;
}

char parse(int8_t code[]){
  // printf("parse\r\n");
  for(int8_t i=0; i<26; i++){
    int8_t flag = 1;
    for(int8_t j=0; j<5; j++)
      if(code[j]!=morseCode[i][j])
        flag=0;
    if(flag){
      // printf("%"PRId8"\r\n",i);
      return (char)('a'+i);
    }
  }
  for(int8_t q=0; q<5; q++)printf("%"PRId8,code[q]);
  printf("\r\n");
  return '?';
}

void readMorse(){
  const static int16_t UNIT = 1000, OFFSET = 300;
  static int16_t time = 0, ledcount = 0;
  static int8_t last = 1, index = 0, code[5] = {0,0,0,0,0};
  int8_t current = isButtonPressed(BTN_PIN,0x04);
  if(current!=last){
    if(last){
      if(time >= UNIT-OFFSET && time <= UNIT+OFFSET)
        code[index]=1;
      if(time >= 3*UNIT-OFFSET/* && time <= 3*UNIT+OFFSET*/)
        code[index]=3;
    }
    else{
      if(time >= UNIT-OFFSET && time < 3*UNIT-OFFSET)
        if((++index)>=5)index=5;
      if(time >= 3*UNIT-OFFSET){
        printf("%c",parse(code));
        for(int8_t q=0; q<5; q++)code[q]=0;
        index=0;
      }
      if(time >= 7*UNIT-OFFSET)
        printf(" ");
            
    }
    last = current;
    time = 0;
  }

  if(!ledcount)
    toggleLED();
  _delay_ms(1);
  if((++time)>8*UNIT)time=7*UNIT;
  if((++ledcount)>=(UNIT>>1))ledcount=0;
}

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
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

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  BTN_DDR = 0b00000000;
    
  printf("Hello!\r\n");
  while(1) {
    readMorse();
  }
}

