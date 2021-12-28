/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <avr/io.h>


#include <stdio.h>
#include "uart.h"

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainLED_TASK_PRIORITY 2

#define mainREAD_TASK_PRIORITY 1

/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

static void vLed(void* pvParameters);

static void vRead(void* pvParameters);

QueueHandle_t que;


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
    que = xQueueCreate(20,sizeof(uint16_t));

    // Create task.
    xTaskHandle led_handle;
    xTaskHandle read_handle;

    xTaskCreate
        (
         vLed,
         "Led",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLED_TASK_PRIORITY,
         &led_handle
        );

    xTaskCreate
        (
         vRead,
         "Read",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainREAD_TASK_PRIORITY,
         &read_handle
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

/**************************************************************************//**
 * \fn static void vBlinkLed(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vLed(void* pvParameters){
    DDRB |= _BV(PB5);

    while(que == NULL)//jeszcze nie stworzono queue
        vTaskDelay(10 / portTICK_PERIOD_MS);

    while(1){
        uint16_t ms;
        while(xQueueReceive(que,&ms,(TickType_t)10) == pdFALSE)
            vTaskDelay(1 / portTICK_PERIOD_MS);

        PORTB |= _BV(PB5);
        vTaskDelay(ms / portTICK_PERIOD_MS);
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
static void vRead(void* pvParameters){
    uart_init();
    stdin = stdout = stderr = &uart_file;

    while(que == NULL)//jeszcze nie stworzono queue
        vTaskDelay(10 / portTICK_PERIOD_MS);    

    uint16_t ms = 0;
    while(1){
        char c = getchar();

        if(c=='\n' || c=='\r'){
            while(xQueueSend(que,&ms,(TickType_t)10) != pdTRUE)
                vTaskDelay(1 / portTICK_PERIOD_MS);
            printf("\r\n");
            ms = 0;
        }
        else{
            putchar(c);
            ms = ms*10 + (c - '0');
        }
    }
}
