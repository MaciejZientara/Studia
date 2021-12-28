#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

int uart_transmit(char c, FILE *stream);
int uart_receive(FILE *stream);

FILE uart_file = FDEV_SETUP_STREAM(uart_transmit, uart_receive, _FDEV_SETUP_RW);

volatile QueueHandle_t bufferTX, bufferRX;
volatile int8_t readyTX;//bool czy gotowy, ale nie dostal jeszcze znaku

void uart_init() {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */

  bufferTX = xQueueCreate(32,sizeof(char));
  bufferRX = xQueueCreate(32,sizeof(char));
  // wlacz rx i tx interrupt
  UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0) | _BV(UDRIE0);
}

int uart_transmit(char c, FILE *stream) {
  if(readyTX){
    readyTX = 0;
    UDR0 = c;
  }
  else{
    // czekaj aż miejsce do zapisu dostępne
    while(xQueueSend(bufferTX,&c,(TickType_t)10)!=pdTRUE)
      taskYIELD();
  }
}

int uart_receive(FILE *stream) {
  // czekaj aż znak dostępny
  char res;
  while(xQueueReceive(bufferRX,&res,(TickType_t)10)!=pdTRUE)
    taskYIELD();
  return (int)res;
}


ISR(USART_RX_vect){//receive
  char c = UDR0;
  xQueueSendFromISR(bufferRX,&c,pdFALSE);
  // while(xQueueSendFromISR(bufferRX,&c,pdFALSE)!=pdTRUE)
  //   vTaskDelay(10 / portTICK_PERIOD_MS);
    // taskYIELD();
}

ISR(USART_TX_vect){//transmit finished
}

ISR(USART_UDRE_vect){//ready to send new data
  char c;
  xQueueReceiveFromISR(bufferTX,&c,pdFALSE);
  // while(xQueueReceiveFromISR(bufferTX,&c,pdFALSE)!=pdTRUE)
  //   vTaskDelay(10 / portTICK_PERIOD_MS);
    // taskYIELD();
  UDR0 = c;
  readyTX = 0;
}

