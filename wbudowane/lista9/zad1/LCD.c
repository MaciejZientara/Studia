#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"

#define EMPTY "                "

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

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

FILE uart_file;
FILE hd44780_file;



char *bottomLine = EMPTY;

void move_text_up(){
  LCD_Clear();
  LCD_GoTo(0, 0);
  printf("%s", bottomLine);
  LCD_GoTo(0, 1);
  bottomLine = EMPTY;
}

void enter(){
  static int8_t first = 0;
  if(first==0){
    first = 1;
    bottomLine = EMPTY;
  }
  else{
    move_text_up();
  }
  LCD_GoTo(0, 1);
}

int main()
{
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_Clear();
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = &uart_file;
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
  stdout = stderr = &hd44780_file;
  // program testowy

  uint8_t v = 0;
  while(1) {
    char c;
    scanf("%c", &c);
    if(c=='\n' || c=='\r'){
      bottomLine[v]='\0';
      enter();
      if(v==0)
        LCD_GoTo(0, 0);
      v=0;
    }
    else{
      printf("%c",c);
      bottomLine[v]=c;
      v++;
      if(v==16){
        v=0;
        enter();
      }
    }
  }
}

