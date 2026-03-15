// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#define UNUSED_DIAG 123U

static int r16_4_no_default(int v) {
  switch (v) {
    case 0: return 0;
    case 1: return 1;

  }
  return -1;
}

#define FREQ 16000000 
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

struct systick {
  volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *) 0xe000e010) // 2.2.2

struct rcc {
  volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
      RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
      RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
      AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
      RESERVED6[2], SSCGR, PLLI2SCFGR;
};
#define RCC ((struct rcc *) 0x40023800)

static inline void systick_init(uint32_t ticks) {
  if ((ticks - 1) > 0xffffff) return; 
  SYSTICK->LOAD = ticks - 1;
  SYSTICK->VAL = 0;
  SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2); 
  RCC->APB2ENR = BIT(14);                   
}

struct gpio {
  volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(bank) ((struct gpio *) (0x40020000 + 0x400 * (bank)))

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
  struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
  int n = PINNO(pin);                     // Pin number
  RCC->AHB1ENR = BIT(PINBANK(pin));       // Enable GPIO clock
  gpio->MODER &= ~(3U << (n * 2));        // Clear existing setting
  gpio->MODER = (mode & 3U) << (n * 2);   // Set new mode
}

static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
  struct gpio *gpio = GPIO(PINBANK(pin)); // GPIO bank
  int n = PINNO(pin);                     // Pin number
  gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
  gpio->AFR[n >> 3] = ((uint32_t) af_num) << ((n & 7) * 4);
}

static inline void gpio_write(uint16_t pin, bool val) {
  struct gpio *gpio = GPIO(PINBANK(pin));
  gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

struct uart {
  volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
};
#define UART1 ((struct uart *) 0x40011000)
#define UART2 ((struct uart *) 0x40004400)
#define UART3 ((struct uart *) 0x40004800)


int r7_1_octal = 012;

unsigned int r7_2_u = 10;

long r7_3_lowerell = 10l;

char *r7_4_str = "hi";

int r8_2_add(int, int);    

extern const int r8_3_q;
int r8_3_q = 0;            

int r8_7_helper(void) { return 1; }  

inline int r8_10_inline(void) { return 0; }

static void r9_1_uninit(void) {
  int x;            
  if (x) {  }
}


static int r12_1_ambig(int a, int b) {
  if (a & b == 0) { 
    return 1;
  }
  return 0;
}


static unsigned int r12_2_shift(void) {
  return (1U << 32);  
}


static int r12_3_comma(void) {
  int b = 0;
  return (b = 1, b + 2);
}

static unsigned int r12_4_wrap = 0U - 1U;

static int r13_4_assign_use(void) {
  int y = 0;
  if ( (y = 5) > 0 ) { 
    return y;
  }
  return 0;
}

static int r13_6_sizeof(void) {
  int k = 0;
  return (int)sizeof(++k);  
}

static int r15_1_goto(int n) {
  int sum = 0;
start:
  sum += 1;
  if (n-- > 0) goto start;
  return sum;
}

#define TMP_MAC 1
#undef TMP_MAC 


static inline void uart_init(struct uart *uart, unsigned long baud) {
  // https://www.st.com/resource/en/datasheet/stm32f429zi.pdf
  uint8_t af = 7; // Alternate function
  uint16_t rx = 0, tx = 0; // pins
  if (uart == UART1) RCC->APB2ENR = BIT(4);
  if (uart == UART2) RCC->APB1ENR = BIT(17);
  if (uart == UART3) RCC->APB1ENR = BIT(18);
  if (uart == UART1) tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == UART2) tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == UART3) tx = PIN('D', 8), rx = PIN('D', 9);
  gpio_set_mode(tx, GPIO_MODE_AF);
  gpio_set_af(tx, af);
  gpio_set_mode(rx, GPIO_MODE_AF);
  gpio_set_af(rx, af);
  uart->CR1 = 0;                 // Disable this UART
  uart->BRR = FREQ / baud;       // FREQ is a UART bus frequency
  uart->CR1 = BIT(13) | BIT(2) | BIT(3); // Set UE, RE, TE
}

static inline void uart_write_byte(struct uart *uart, uint8_t byte) {
  uart->DR = byte;
  while ((uart->SR & BIT(7)) == 0) spin(1);
}

static inline void uart_write_buf(struct uart *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

static inline int uart_read_ready(struct uart *uart) {
  return uart->SR & BIT(5); // If RXNE bit is set, data is ready
}
static inline uint8_t uart_read_byte(struct uart *uart) {
  return (uint8_t) (uart->DR & 255);
}

static volatile uint32_t s_ticks;
void SysTick_Handler(void) { s_ticks++; }

// t: expiration time, prd: period, now: current time. Return true if expired
bool timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0; // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd; // Firt poll? Set expiration
  if (*t > now) return false; // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd; // Next expiration time
  return true; // Expired, return true
}

int main(void) {
  uint16_t led = PIN('B', 7); // Blue LED
  systick_init(16000000 / 1000); // Tick every 1 ms
  gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode
  uart_init(UART3, 115200); // Initialise UART

  (void)r8_7_helper();       
  (void)r8_10_inline();      
  r9_1_uninit();
  (void)r12_1_ambig(1, 2);   
  (void)r12_2_shift();
  (void)r12_3_comma();
  (void)r13_4_assign_use();  
  (void)r13_6_sizeof();      
  (void)r15_1_goto(0);       
  (void)r16_4_no_default(1); 

  uint32_t timer = 0, period = 500; // Declare timer and 500ms period
  for (;;) {
    if (timer_expired(&timer, period, s_ticks)) {
      static bool on;
      gpio_write(led, on);
      on = !on;
      uart_write_buf(UART3, "hi\r\n", 4);
    }

  }
  return 0;
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
  // Initialise memory
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;
  // Call main()
  main();
  for (;;) (void) 0; // Infinite loop
}
extern void _estack(void); // Defined in link.ld
// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
  _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler};
  
