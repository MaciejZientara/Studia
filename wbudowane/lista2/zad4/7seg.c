#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

int8_t counter = 0;

void CounterInc(){
    counter++;
    if(counter == 60)   //counter%60
        counter=0;
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
    int8_t first=0, second=0;
    // while(10*(second+1)>=counter)
    //     second++;
    // first = counter - second*10;
    second = counter/10;
    first = counter%10;

    PORTC = 0b00000011;
    LED_PORT = ~patterns[first];
    PORTC = 0b00000001;
    // _delay_ms(1);
    _delay_us(5);
    PORTC = 0b00000011;
    LED_PORT = ~patterns[second];
    PORTC = 0b00000010;
    // _delay_ms(1);
    _delay_us(5);
} //pomyliłem zapis do patterns, 0 i 1 powinny być na odwrót, stąd ~

ISR (TIMER1_OVF_vect){    // Timer1 ISR
    CounterInc();
    TCNT1 = 49910;   // for 1 sec at 16 MHz, 65535 - 16'000'000/1024
}

int main() {
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    LED_DDR = (int8_t)(-1);
    DDRC = 0b00000011;

    TCNT1 = 49910;
    TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);  // Timer mode with 1024 prescler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
	
    // int mikro = 0;
    while (1) {
        // for(int i=0; i<500; i++)
            setLED();
        // CounterInc();
    }
}
