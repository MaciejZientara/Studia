#include <avr/io.h>
#include <util/delay.h>

#define BTN_DDR DDRC
#define BTN_PORT PORTC
#define BTN_PIN PINC

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB


void setLED(int8_t state){
    if(state)
        LED_PORT |= _BV(LED);
    else
        LED_PORT &= ~_BV(LED);
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

int8_t getState(uint64_t *tab, uint16_t place){
    // return (tab[place/64] & (uint64_t)(1ULL<<(place%64)))>=1;
    return (tab[place>>6] & (uint64_t)(1ULL<<(place&63)))>=1;
}

void setState(uint64_t *tab, uint16_t place, uint8_t state){
    if(state){
        // tab[place/64] |= (uint64_t)(1ULL<<(place%64));
        tab[place>>6] |= (uint64_t)(1ULL<<(place&63));
    }
    else{
        // tab[place/64] &= ~(uint64_t)(1ULL<<(place%64));
        tab[place>>6] &= ~(uint64_t)(1ULL<<(place&63));
    }
}

int main() {
    LED_DDR |= _BV(LED);
    BTN_DDR = 0b00000000;
    // BTN_PORT = 0b00011100;
    
    uint64_t cycle[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint16_t read=0, write=1000;

    while (1) {
        // setLED(isButtonPressed(BTN_PIN,0x04));
        setLED(getState(cycle,read));
        // setState(cycle,write,isButtonPressed(BTN_PIN,0x04));
        setState(cycle,write,!(BTN_PIN & 0x04));
        _delay_ms(1);
        if((++read)==1024)read=0;
        if((++write)==1024)write=0;
    }
}
