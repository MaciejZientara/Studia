/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
#include "uart.h"

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainCYCLE_TASK_PRIORITY   2

#define mainCYCLON_TASK_PRIORITY 1

/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

static void vCycle(void* pvParameters);

static void vCyclonEye(void* pvParameters);

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
    xTaskHandle cycle_handle;
    xTaskHandle cyclon_handle;

    xTaskCreate
        (
         vCycle,
         "cycle",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainCYCLE_TASK_PRIORITY,
         &cycle_handle
        );

    xTaskCreate
        (
         vCyclonEye,
         "cyclonEye",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainCYCLON_TASK_PRIORITY,
         &cyclon_handle
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

static uint64_t cycle[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint16_t read=0, write=1000;


static int8_t getState(uint64_t *tab, uint16_t place){
    // return (tab[place/64] & (uint64_t)(1ULL<<(place%64)))>=1;
    return (tab[place>>6] & (uint64_t)(1ULL<<(place&63)))>=1;
}

static void setState(uint64_t *tab, uint16_t place, uint8_t state){
    if(state){
        // tab[place/64] |= (uint64_t)(1ULL<<(place%64));
        tab[place>>6] |= (uint64_t)(1ULL<<(place&63));
    }
    else{
        // tab[place/64] &= ~(uint64_t)(1ULL<<(place%64));
        tab[place>>6] &= ~(uint64_t)(1ULL<<(place&63));
    }
}

/**************************************************************************//**
 * \fn static void vCycle(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vCycle(void* pvParameters){
    DDRB |= _BV(PB5);
    DDRC &= ~_BV(PC4);
    PORTC |= _BV(PC4);

    while(1){
        if(getState(cycle,read))
            PORTB |= _BV(PB5);
        else
            PORTB &= ~_BV(PB5);
        setState(cycle,write,!(PINC & _BV(PC4)));
        vTaskDelay(1 / portTICK_PERIOD_MS);
        if((++read)==1024)read=0;
        if((++write)==1024)write=0;
    }
}


/**************************************************************************//**
 * \fn static void vSerial(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vCyclonEye(void* pvParameters)
{
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    DDRD = (int8_t)(-1);
    
    uint8_t direction = 0, counter = 1;
    while(1){
        if(direction)
            counter >>= 1;
        else
            counter <<= 1;
        PORTD = counter;
        if(counter == 1 || counter == 0b10000000){
            direction ^= 1;
            vTaskDelay(150 / portTICK_PERIOD_MS);
        }
        vTaskDelay(150 / portTICK_PERIOD_MS);
    }
}
