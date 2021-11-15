#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 0000 -- normal
  // CS1   = 001  -- prescaler 1
  TCCR1B = _BV(CS10);
}
FILE uart_file;

//int8 t, int16 t, int32 t, int64 t, float
void int8(){
    printf("int8\r\n");
    volatile int8_t a, b, c;
    // scanf("%"SCNd8"%"SCNd8, &a, &b);
    volatile uint16_t start, end;
    start = TCNT1; // wartość licznika przed czekaniem
    c = a + b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli +: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a * b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli *: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a / b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli /: %"PRIu16"\r\n", end - start);
    a = c; // to fix warning: variable ‘c’ set but not used [-Wunused-but-set-variable]
}
void int16(){
    printf("int16\r\n");
    volatile int16_t a, b, c;
    // scanf("%"SCNd16"%"SCNd16, &a, &b);
    volatile uint16_t start, end;
    start = TCNT1; // wartość licznika przed czekaniem
    c = a + b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli +: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a * b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli *: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a / b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli /: %"PRIu16"\r\n", end - start);
    a = c; // to fix warning: variable ‘c’ set but not used [-Wunused-but-set-variable]
}
void int32(){
    printf("int32\r\n");
    volatile int32_t a, b, c;
    // scanf("%"SCNd32"%"SCNd32, &a, &b);
    volatile uint16_t start, end;
    start = TCNT1; // wartość licznika przed czekaniem
    c = a + b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli +: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a * b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli *: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a / b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli /: %"PRIu16"\r\n", end - start);
    a = c; // to fix warning: variable ‘c’ set but not used [-Wunused-but-set-variable]
}
void int64(){
    printf("int64\r\n");
    volatile int64_t a, b, c;
    // scanf("%lld %lld", &a, &b);
    volatile uint16_t start, end;
    start = TCNT1; // wartość licznika przed czekaniem
    c = a + b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli +: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a * b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli *: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a / b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli /: %"PRIu16"\r\n", end - start);
    a = c; // to fix warning: variable ‘c’ set but not used [-Wunused-but-set-variable]
}
void flo(){
    printf("flo\r\n");
    volatile float a, b, c;
    // scanf("%f %f", &a, &b);
    volatile uint16_t start, end;
    start = TCNT1; // wartość licznika przed czekaniem
    c = a + b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli +: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a * b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli *: %"PRIu16"\r\n", end - start);
    start = TCNT1; // wartość licznika przed czekaniem
    c = a / b;
    end = TCNT1; // wartość licznika po czekaniu
    printf("Ilosc cykli /: %"PRIu16"\r\n", end - start);
    a = c; // to fix warning: variable ‘c’ set but not used [-Wunused-but-set-variable]
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj licznik
  timer1_init();
  // program testowy
  int8();
  int16();
  int32();
  int64();
  flo();
  while(1) {
  }
}

