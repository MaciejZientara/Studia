#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

int8_t counter = 0;

void CounterInc(){
    counter = (counter+1)%10;
}

int patterns[] = {
//    FGABCDE.
    0b10111110, //0
    0b00011000, //1
    0b01110110, //2
    0b01111100, //3
    0b11011000, //4
    0b11101100, //5
    0b11101110, //6
    0b00111000, //7
    0b11111110, //8
    0b11111100  //9
};

void setLED(){
    LED_PORT = ~patterns[counter];
} //pomyliłem zapis do patterns, 0 i 1 powinny być na odwrót, stąd ~

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    LED_DDR = (int8_t)(-1);
    while (1) {
        setLED();
        _delay_ms(1000);
        CounterInc();
    }
}
