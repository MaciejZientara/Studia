/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include "uart.h"

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainKeepTemp_TASK_PRIORITY 2

#define mainSERIAL_TASK_PRIORITY 1

/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

static volatile int8_t temperature = 0;

static void vKeepTemperature(void* pvParameters);

static void vSerial(void* pvParameters);

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
    // Create task.
    xTaskHandle ktemp_handle;
    xTaskHandle serial_handle;

    xTaskCreate
        (
         vKeepTemperature,
         "ktemp",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainKeepTemp_TASK_PRIORITY,
         &ktemp_handle
        );

    xTaskCreate
        (
         vSerial,
         "serial",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainSERIAL_TASK_PRIORITY,
         &serial_handle
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
void vApplicationIdleHook(void)
{

}

/******************************************************************************
 * Private function definitions.
 ******************************************************************************/

static void adc_init()
{
  ADMUX   = _BV(REFS1) | _BV(REFS0); // referencja Vref, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}


static int8_t TempNow(){
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF)))vTaskDelay(1 / portTICK_PERIOD_MS); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    // Vout = Tc * Ta + V_0c
    // Ta = (Vout - V_0c) / Tc
    // v / 1024 = Vout / 1.1
    // Vout = (v * 1.1) * 1000/1024 (Vout w [mV] )
    // datasheet:
    // V_0c = 500 [mV]
    // Tc = 10
    // Ta = ((v * 1.1) * 1000/1024 - 500) / 10 
    // Ta = v * 110 / 1024 - 50 
    return (int32_t)v * 110 / 1024 - 50;
}

/**************************************************************************//**
 * \fn static void vBlinkLed(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vKeepTemperature(void* pvParameters)
{
    DDRB |= _BV(PB5);
    adc_init();

    while(1){
        int8_t now = TempNow();
        if(now < temperature-1)
            PORTB |= _BV(PB5);
        // vTaskDelay(100 / portTICK_PERIOD_MS);
        if(now < temperature)
            PORTB &= ~_BV(PB5);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


/**************************************************************************//**
 * \fn static void vSerial(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vSerial(void* pvParameters)
{
    sei();
    uart_init();
    stdin = stdout = stderr = &uart_file;

    char input;

    // printf("n - (now) aktualna temp., s ... - (set) ustawia nowa temp.\r\n");
    printf("temperatura ustawiona na %hhd\r\n", temperature);
    while(1){
        scanf("%c",&input);
        if(input == 'n')
            printf("aktualna temperatura = %hhd\r\n", TempNow());
        if(input == 's'){
            scanf("%hhd", &temperature);
            printf("temperatura ustawiona na %hhd\r\n", temperature);
        }
    }
}
