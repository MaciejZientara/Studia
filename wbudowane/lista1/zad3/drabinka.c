#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

#define WAIT 150

void CyclonEye(){
    static uint8_t direction = 0, counter = 1;
    if(direction)
        counter >>= 1;
    else
        counter <<= 1;
    LED_PORT = counter;
    if(counter == 1 || counter == 0b10000000){
        direction ^= 1;
        _delay_ms(WAIT);
    }
}

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    LED_DDR = (int8_t)(-1);
    while (1) {
        CyclonEye();
        _delay_ms(WAIT);
    }
}
