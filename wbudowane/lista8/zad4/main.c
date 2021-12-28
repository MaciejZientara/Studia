/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <avr/interrupt.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include <stdio.h>
#include "uart.h"

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

static void ADCinit();

static void vADCmeasure(uint8_t n);

// Create task.
xTaskHandle Task1, Task2, Task3;
xTaskHandle TaskToStop;

SemaphoreHandle_t taskMTX, adcMTX;


/******************************************************************************
 * Public function definitions.
 ******************************************************************************/

/**************************************************************************//**
 * \fn int main(void)
 *
 * \brief Main function.
 *
 * \return
 ******************************************************************************/
int main(void)
{
    sei();
    uart_init();
    stdin = stdout = stderr = &uart_file;
    ADCinit();
    set_sleep_mode(SLEEP_MODE_IDLE);

    taskMTX = xSemaphoreCreateMutex();
    adcMTX = xSemaphoreCreateMutex();
    xSemaphoreTake(adcMTX,portMAX_DELAY);
    //zeby za pierwszym razem adc zaczekalo na wynik

    xTaskCreate(
         vADCmeasure,
         "vADCmeasure1",
         configMINIMAL_STACK_SIZE + 32,
         1,
         1,
         &Task1
        );
    xTaskCreate(
         vADCmeasure,
         "vADCmeasure2",
         configMINIMAL_STACK_SIZE + 32,
         2,
         1,
         &Task2
        );
    xTaskCreate(
         vADCmeasure,
         "vADCmeasure3",
         configMINIMAL_STACK_SIZE + 32,
         3,
         1,
         &Task3
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

/**************************************************************************//**
 * \fn static vApplicationIdleHook(void)
 *
 * \brief
 ******************************************************************************/
void vApplicationIdleHook(void){
    // printf("sleep\r\n");
    // sleep_mode();
}

/******************************************************************************
 * Private function definitions.
 ******************************************************************************/

static void ADCinit(){
    ADMUX   = _BV(REFS0); // referencja AVcc
    DIDR0   |= _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
    DIDR0   |= _BV(ADC1D); // wyłącz wejście cyfrowe na ADC1
    DIDR0   |= _BV(ADC2D); // wyłącz wejście cyfrowe na ADC2
    // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
    ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
    ADCSRA |= _BV(ADEN); // włącz ADC
    ADCSRA |= _BV(ADIE); // włącz ADC interrupt
}

ISR(ADC_vect){
    xSemaphoreGiveFromISR(adcMTX, NULL);//poinformuj o wykonaniu pomiaru
}

uint16_t readADC(uint8_t mux){
    mux--;
    ADMUX = _BV(REFS0) | mux; // referencja AVcc, wejście ADC(mux-1)
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    xSemaphoreTake(adcMTX,portMAX_DELAY);//poczekaj na wykonanie pomiaru
    return ADC;
}

/**************************************************************************//**
 * \fn static void vBlinkLed(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vADCmeasure(uint8_t n){
    TaskHandle_t my_task;
    if(n==1)
        my_task = Task1;
    if(n==2)
        my_task = Task2;
    if(n==3)
        my_task = Task3;
    
    while(1){
        xSemaphoreTake(taskMTX,portMAX_DELAY);
        TaskToStop = my_task;
        uint16_t res = readADC(n);
        printf("task %hhu read %hu\r\n", n, res);
        // fflush(stdout);
        // _delay_ms(1);
        xSemaphoreGive(taskMTX);
        vTaskDelay((1000 * n) / portTICK_PERIOD_MS);
    }
}
