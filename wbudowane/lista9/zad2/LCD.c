#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"


int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

FILE hd44780_file;

void setLoadingSymbols(){
  LCD_WriteCommand(0x40);

  //aby zamiast WriteData uzyc Write
  LCD_RS_PORT |= LCD_RS;
  //pusty znak
  _LCD_Write(0x00);
  _LCD_Write(0x00);
  _LCD_Write(0x00);
  _LCD_Write(0x00);
  _LCD_Write(0x00);
  _LCD_Write(0x00);
  _LCD_Write(0x00);
  _LCD_Write(0x00);
  //1 kolumna
  _LCD_Write(0x10);
  _LCD_Write(0x10);
  _LCD_Write(0x10);
  _LCD_Write(0x10);
  _LCD_Write(0x10);
  _LCD_Write(0x10);
  _LCD_Write(0x10);
  _LCD_Write(0x10);
  //2 kolumny
  _LCD_Write(0x18);
  _LCD_Write(0x18);
  _LCD_Write(0x18);
  _LCD_Write(0x18);
  _LCD_Write(0x18);
  _LCD_Write(0x18);
  _LCD_Write(0x18);
  _LCD_Write(0x18);
  //3 kolumny
  _LCD_Write(0x1c);
  _LCD_Write(0x1c);
  _LCD_Write(0x1c);
  _LCD_Write(0x1c);
  _LCD_Write(0x1c);
  _LCD_Write(0x1c);
  _LCD_Write(0x1c);
  _LCD_Write(0x1c);
  //4 kolumny
  _LCD_Write(0x1e);
  _LCD_Write(0x1e);
  _LCD_Write(0x1e);
  _LCD_Write(0x1e);
  _LCD_Write(0x1e);
  _LCD_Write(0x1e);
  _LCD_Write(0x1e);
  _LCD_Write(0x1e);
  //5 kolumn
  _LCD_Write(0x1f);
  _LCD_Write(0x1f);
  _LCD_Write(0x1f);
  _LCD_Write(0x1f);
  _LCD_Write(0x1f);
  _LCD_Write(0x1f);
  _LCD_Write(0x1f);
  _LCD_Write(0x1f);
}

void inline printLoading(int8_t i){
  LCD_WriteData(i);
  // printf("%c", 'a'+i);
}

void loading(){
  LCD_Clear();
  printf("LOADING...");
  LCD_GoTo(0, 1);

  static const int8_t WAIT = 75;
  for(int8_t i=0; i<16; i++){
    for(int8_t j=0; j<=5; j++){
      LCD_GoTo(i, 1);
      printLoading(j);
      _delay_ms(WAIT);
    }
    _delay_ms(WAIT);
  }

  _delay_ms(WAIT);
  _delay_ms(WAIT);
  _delay_ms(WAIT);
  LCD_Clear();
  printf("DONE :)");
}

int main()
{
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_Clear();
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
  stdout = stderr = &hd44780_file;

  setLoadingSymbols();
  LCD_Clear();

  while(1){
    loading();
    _delay_ms(1000);
  }
}

