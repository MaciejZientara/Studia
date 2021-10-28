#include <avr/io.h>
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

FILE uart_file;

//int8 t, int16 t, int32 t, int64 t, float
void int8(){
    printf("int8\r\n");
    int8_t a, b;
    scanf("%"SCNd8"%"SCNd8, &a, &b);
    printf("%"PRId8" + %"PRId8" = %"PRId8"\r\n", a, b, (int8_t)(a+b));
    printf("%"PRId8" * %"PRId8" = %"PRId8"\r\n", a, b, (int8_t)(a*b));
    printf("%"PRId8" / %"PRId8" = %"PRId8"\r\n", a, b, (int8_t)(a/b));
}
void int16(){
    printf("int16\r\n");
    int16_t a, b;
    scanf("%"SCNd16"%"SCNd16, &a, &b);
    printf("%"PRId16" + %"PRId16" = %"PRId16"\r\n", a, b, (int16_t)(a+b));
    printf("%"PRId16" * %"PRId16" = %"PRId16"\r\n", a, b, (int16_t)(a*b));
    printf("%"PRId16" / %"PRId16" = %"PRId16"\r\n", a, b, (int16_t)(a/b));
}
void int32(){
    printf("int32\r\n");
    int32_t a, b;
    scanf("%"SCNd32"%"SCNd32, &a, &b);
    printf("%"PRId32" + %"PRId32" = %"PRId32"\r\n", a, b, (int32_t)(a+b));
    printf("%"PRId32" * %"PRId32" = %"PRId32"\r\n", a, b, (int32_t)(a*b));
    printf("%"PRId32" / %"PRId32" = %"PRId32"\r\n", a, b, (int32_t)(a/b));
}
void int64(){
    printf("int64\r\n");
    int64_t a, b;
    // scanf("%"SCNd64 "%"SCNd64, &a, &b);
    // printf("%"PRId64 "+ %"PRId64 "= %"PRId64"\r\n", a, b, (int64_t)(a+b));
    // printf("%"PRId64 "* %"PRId64 "= %"PRId64"\r\n", a, b, (int64_t)(a*b));
    // printf("%"PRId64 "/ %"PRId64 "= %"PRId64"\r\n", a, b, (int64_t)(a/b));

    // scanf("%ld %ld", &a, &b);
    // printf("%ld + %ld = %ld\r\n", a, b, (int64_t)(a+b));
    // printf("%ld * %ld = %ld\r\n", a, b, (int64_t)(a*b));
    // printf("%ld / %ld = %ld\r\n", a, b, (int64_t)(a/b));

    scanf("%lld %lld", &a, &b);
    printf("%lld + %lld = %lld\r\n", a, b, (int64_t)(a+b));
    printf("%lld * %lld = %lld\r\n", a, b, (int64_t)(a*b));
    printf("%lld / %lld = %lld\r\n", a, b, (int64_t)(a/b));
}
void flo(){
    printf("flo\r\n");
    float a, b;
    scanf("%f %f", &a, &b);
    printf("%f + %f = %f\r\n", a, b, (float)(a+b));
    printf("%f * %f = %f\r\n", a, b, (float)(a*b));
    printf("%f / %f = %f\r\n", a, b, (float)(a/b));
}


int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // program testowy
  printf("Hello!\r\n");
  while(1) {
      int8();
      int16();
      int32();
      int64();
      flo();
  }
}


/*
scanf
# define SCNd8		"hhd"
# define SCNd16		"hd"
# define SCNd32		"d"
# define SCNd64		__PRI64_PREFIX "d"


printf
# define PRId8		"d"
# define PRId16		"d"
# define PRId32		"d"
# define PRId64		__PRI64_PREFIX "d"
*/

