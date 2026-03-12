#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>
// I2C address of LCD
#define LCD_ADDR 0x27 << 1 // left-shifted for write
// I2C Pins
#define SCL_PIN 8 // PB8
#define SDA_PIN 9 // PB9
// Sensor and LED pins
#define IR_PIN 2 // PD2
#define VIB_PIN 3 // PD3
#define TRIG_PIN 3 // PA3
#define ECHO_PIN 4 // PA4
#define LED 1 // PA1
#define LED_PIN 12 // PD12
#define EXT_LED_PIN 13 // PD13
// Delay functions
void delay_us(uint32_t us) {
SysTick->LOAD = 16 * us - 1;
SysTick->VAL = 0;
SysTick->CTRL = 5;
while ((SysTick->CTRL & 0x10000) == 0);
SysTick->CTRL = 0;
}
void delay_ms(uint32_t ms) {
for (uint32_t i = 0; i < ms; i++) delay_us(1000);
}
// I2C utility
void i2c1_init(void) {
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
GPIOB->MODER &= ~((3 << (SCL_PIN * 2)) | (3 << (SDA_PIN * 2)));
GPIOB->MODER |= (2 << (SCL_PIN * 2)) | (2 << (SDA_PIN * 2)); // Alternate Function
GPIOB->AFR[1] |= (4 << ((SCL_PIN - 8) * 4)) | (4 << ((SDA_PIN - 8) * 4)); // AF4 for I2C1
RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
I2C1->CR1 &= ~I2C_CR1_PE;
I2C1->CR2 = 16; // 16 MHz APB1
I2C1->CCR = 80; // 100 kHz
I2C1->TRISE = 17; // Max rise time
I2C1->CR1 |= I2C_CR1_PE;
}
void i2c1_start(void) {
I2C1->CR1 |= I2C_CR1_START;
while (!(I2C1->SR1 & I2C_SR1_SB));
}
void i2c1_addr(uint8_t addr) {
I2C1->DR = addr;
while (!(I2C1->SR1 & I2C_SR1_ADDR));
volatile uint32_t temp = I2C1->SR2;
(void)temp;
}
void i2c1_write(uint8_t data) {
while (!(I2C1->SR1 & I2C_SR1_TXE));
I2C1->DR = data;
while (!(I2C1->SR1 & I2C_SR1_BTF));
}
void i2c1_stop(void) {
I2C1->CR1 |= I2C_CR1_STOP;
}
// LCD over I2C
void lcd_i2c_send(uint8_t data, uint8_t control) {
uint8_t high = data & 0xF0;
uint8_t low = (data << 4) & 0xF0;
uint8_t data_arr[6] = {
high | control | 0x08,
high | control | 0x0C,
high | control | 0x08,
low | control | 0x08,
low | control | 0x0C,
low | control | 0x08
};
for (int i = 0; i < 6; i++) {
i2c1_start();
i2c1_addr(LCD_ADDR);
i2c1_write(data_arr[i]);
i2c1_stop();
delay_us(50);
}
}
void lcd_cmd(uint8_t cmd) {
lcd_i2c_send(cmd, 0x00);
delay_ms(2);
}
void lcd_data(uint8_t data) {
lcd_i2c_send(data, 0x01);
delay_ms(2);
}
void lcd_clear(void) {
lcd_cmd(0x01);
delay_ms(2);
}
void lcd_set_cursor(uint8_t row, uint8_t col) {
lcd_cmd(0x80 + (row == 0 ? 0x00 : 0x40) + col);
}
void lcd_print(char *str) {
while (*str) lcd_data(*str++);
}
void lcd_init(void) {
delay_ms(50);
lcd_cmd(0x33);
lcd_cmd(0x32);
lcd_cmd(0x28); // 4-bit, 2-line
lcd_cmd(0x0C); // Display ON
lcd_cmd(0x06); // Entry mode
lcd_clear();
}
// GPIO Setup
void gpio_init(void) {
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
GPIOA->MODER |= (1 << (LED * 2)); // PA1 output
GPIOA->MODER &= ~(1 << (LED * 2 + 1));
GPIOA->MODER &= ~((3 << (TRIG_PIN * 2)) | (3 << (ECHO_PIN * 2)));
GPIOA->MODER |= (1 << (TRIG_PIN * 2)); // PA3 Output
GPIOD->MODER &= ~(3 << (IR_PIN * 2));
GPIOD->MODER &= ~(3 << (VIB_PIN * 2));
GPIOD->MODER |= (1 << (LED_PIN * 2)) | (1 << (EXT_LED_PIN * 2));
GPIOD->MODER &= ~((1 << (LED_PIN * 2 + 1)) | (1 << (EXT_LED_PIN * 2 + 1)));
}
uint32_t read_distance_cm(void) {
GPIOA->BSRR = (1 << TRIG_PIN);
delay_us(10);
GPIOA->BSRR = (1 << (TRIG_PIN + 16));
uint32_t timeout = 30000;
while (((GPIOA->IDR >> ECHO_PIN) & 1) == 0 && timeout--);
if (timeout == 0) return 0xFFFFFFFF;
uint32_t count = 0;
while (((GPIOA->IDR >> ECHO_PIN) & 1) && count < 60000) {
count++;
delay_us(1);
}
return count / 58;
}
int main(void) {
gpio_init();
i2c1_init();
lcd_init();
lcd_set_cursor(0, 0);
lcd_print("System Ready");
uint8_t collision_detected = 0;
GPIOA->BSRR = (1 << (LED + 16));
while (1) {
uint32_t dist = read_distance_cm();
uint8_t ir = ((GPIOD->IDR >> IR_PIN) & 1);
uint8_t vibration = ((GPIOD->IDR >> VIB_PIN) & 1);
lcd_clear();
if (collision_detected || vibration == 0) {
lcd_set_cursor(0, 0);
lcd_print("Collision");
lcd_set_cursor(1, 0);
lcd_print("Detected!!!");
GPIOA->BSRR = (1 << LED);
collision_detected = 1;
GPIOD->BSRR = (1 << LED_PIN) | (1 << EXT_LED_PIN);
delay_ms(100);
GPIOD->BSRR = (1 << (LED_PIN + 16)) | (1 << (EXT_LED_PIN + 16));
delay_ms(100);
break;
}
if (dist <= 10 && ir == 0) {
lcd_set_cursor(0, 0);
lcd_print("Caution!");
lcd_set_cursor(1, 0);
lcd_print("Vehicle Closeby!");
GPIOD->BSRR = (1 << LED_PIN) | (1 << EXT_LED_PIN);
delay_ms(200);
GPIOD->BSRR = (1 << (LED_PIN + 16)) | (1 << (EXT_LED_PIN + 16));
delay_ms(200);
} else if (dist <= 10) {
lcd_set_cursor(0, 0);
lcd_print("Vehicle");
lcd_set_cursor(1, 0);
lcd_print("Approaching");
GPIOD->BSRR = (1 << LED_PIN) | (1 << EXT_LED_PIN);
delay_ms(400);
GPIOD->BSRR = (1 << (LED_PIN + 16)) | (1 << (EXT_LED_PIN + 16));
delay_ms(400);
} else {
lcd_set_cursor(0, 0);
lcd_print("Ready");
if (dist != 0xFFFFFFFF)
GPIOD->BSRR = (1 << LED_PIN) | (1 << EXT_LED_PIN);
else
GPIOD->BSRR = (1 << (LED_PIN + 16)) | (1 << (EXT_LED_PIN + 16));
delay_ms(300);
}
}
