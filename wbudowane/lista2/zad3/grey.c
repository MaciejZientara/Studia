#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
#define BTN_DDR DDRC
#define BTN_PORT PORTC
#define BTN_PIN PINC

uint8_t COUNTER = 0;

uint8_t BinaryToGray(uint8_t num){ // z wiki
    return num ^ (num >> 1);
}

void increaseCounter(){
    COUNTER++;
}

void decreaseCounter(){
    COUNTER--;
}

void resetCounter(){
    COUNTER=0;
}

void setLED(){
    LED_PORT = BinaryToGray(COUNTER);
}

int8_t isButtonPressed(uint8_t pin, uint8_t btn){
    static const int8_t waitTime = 10, tests = 100;
    int8_t count = 0;
    while(!(pin & btn) && tests > count){
        _delay_us(waitTime);
        count++;
    }
    return count==tests;
}


int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    LED_DDR = (int8_t)(-1);
    BTN_DDR = 0b00000000;
    // BTN_PORT = 0b00011100;
    
    while (1) {
        setLED();
        if(isButtonPressed(BTN_PIN,0x04)){
            resetCounter();
            while(isButtonPressed(BTN_PIN,0x04));
        }
        if(isButtonPressed(BTN_PIN,0x08)){
            increaseCounter();
            while(isButtonPressed(BTN_PIN,0x08));
        }
        if(isButtonPressed(BTN_PIN,0x10)){
            decreaseCounter();
            while(isButtonPressed(BTN_PIN,0x10));
        }
    }
}
