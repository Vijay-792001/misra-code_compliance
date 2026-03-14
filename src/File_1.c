
#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>

#define UNUSED_MACRO 42U

// I2C address of LCD
#define LCD_ADDR (0x27 << 1)   // left-shifted for write
// I2C Pins
#define SCL_PIN 8              // PB8
#define SDA_PIN 9              // PB9
// Sensor and LED pins
#define IR_PIN 2               // PD2
#define VIB_PIN 3              // PD3
#define TRIG_PIN 3             // PA3
#define ECHO_PIN 4             // PA4
#define LED 1                  // PA1
#define LED_PIN 12             // PD12
#define EXT_LED_PIN 13         // PD13


int r7_1_octal = 012;                  

unsigned int r7_2_unsigned = 10;       

long r7_3_lowerell = 10l;              

char *r7_4_str = "hello";

int r8_2_add(int, int);             

extern const int r8_3_global;          
int r8_3_global = 0;                   


int r8_7_helper(void) { return 1; } 

inline int r8_10_inline(void) { return 0; }

int r1_3_ub = (1 << 31);

int r1_1_syntax_error = ;          

/* ------------------------------- Delays ------------------------------- */
void delay_us(uint32_t us) {
    SysTick->LOAD = 16U * us - 1U;
    SysTick->VAL  = 0U;
    SysTick->CTRL = 5U;
    while ((SysTick->CTRL & 0x10000U) == 0U) { /* wait */ }
    SysTick->CTRL = 0U;
}
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0U; i < ms; i++) { delay_us(1000U); }
}

/* ------------------------------- I2C ---------------------------------- */
void i2c1_init(void) {
    RCC->AHB1ENR = RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER &= ~((3U << (SCL_PIN * 2)) | (3U << (SDA_PIN * 2)));
    GPIOB->MODER  =  (2U << (SCL_PIN * 2)) | (2U << (SDA_PIN * 2));   // AF
    GPIOB->AFR[1] =  (4U << ((SCL_PIN - 8) * 4)) | (4U << ((SDA_PIN - 8) * 4)); // AF4
    RCC->APB1ENR  = RCC_APB1ENR_I2C1EN;
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16U;  // 16 MHz APB1
    I2C1->CCR = 80U;  // 100 kHz
    I2C1->TRISE = 17U;
    I2C1->CR1 = I2C_CR1_PE;
}
void i2c1_start(void) {
    I2C1->CR1 = I2C_CR1_START;
    while ((I2C1->SR1 & I2C_SR1_SB) == 0U) { }
}
void i2c1_addr(uint8_t addr) {
    I2C1->DR = addr;
    while ((I2C1->SR1 & I2C_SR1_ADDR) == 0U) { }
    volatile uint32_t temp = I2C1->SR2;
    (void)temp;
}
void i2c1_write(uint8_t data) {
    while ((I2C1->SR1 & I2C_SR1_TXE) == 0U) { }
    I2C1->DR = data;
    while ((I2C1->SR1 & I2C_SR1_BTF) == 0U) { }
}
void i2c1_stop(void) {
    I2C1->CR1 = I2C_CR1_STOP;
}

/* ----------------------------- LCD over I2C ---------------------------- */
void lcd_i2c_send(uint8_t data, uint8_t control) {
    uint8_t high = data & 0xF0U;
    uint8_t low  = (data << 4) & 0xF0U;
    uint8_t data_arr[6] = {
        (uint8_t)(high | control | 0x08U),
        (uint8_t)(high | control | 0x0CU),
        (uint8_t)(high | control | 0x08U),
        (uint8_t)(low  | control | 0x08U),
        (uint8_t)(low  | control | 0x0CU),
        (uint8_t)(low  | control | 0x08U)
    };
    for (int i = 0; i < 6; i++) {
        i2c1_start();
        i2c1_addr(LCD_ADDR);
        i2c1_write(data_arr[i]);
        i2c1_stop();
        delay_us(50U);
    }
}
void lcd_cmd(uint8_t cmd)  { lcd_i2c_send(cmd, 0x00U); delay_ms(2U); }
void lcd_data(uint8_t dt)  { lcd_i2c_send(dt,  0x01U); delay_ms(2U); }
void lcd_clear(void)       { lcd_cmd(0x01U);            delay_ms(2U); }
void lcd_set_cursor(uint8_t row, uint8_t col) {
    lcd_cmd((uint8_t)(0x80U + (row == 0U ? 0x00U : 0x40U) + col));
}
void lcd_print(char *str) { while (*str != '\0') { lcd_data((uint8_t)*str++); } }

/* ------------------------------- GPIO --------------------------------- */
void gpio_init(void) {
    RCC->AHB1ENR  = RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
    GPIOA->MODER  = (1U << (LED * 2));
    GPIOA->MODER &= ~(1U << (LED * 2 + 1));
    GPIOA->MODER &= ~((3U << (TRIG_PIN * 2)) | (3U << (ECHO_PIN * 2)));
    GPIOA->MODER |=  (1U << (TRIG_PIN * 2)); // PA3 Output
    GPIOD->MODER &= ~(3U << (IR_PIN  * 2));
    GPIOD->MODER &= ~(3U << (VIB_PIN * 2));
    GPIOD->MODER |=  (1U << (LED_PIN * 2)) | (1U << (EXT_LED_PIN * 2));
    GPIOD->MODER &= ~((1U << (LED_PIN * 2 + 1)) | (1U << (EXT_LED_PIN * 2 + 1)));
}

static void r9_1_uninit_read(void)
{
    int x;             
    if (x) {          
        
    }
}

static int r12_3_demo(void)
{
    int b = 0;
    int r = (b = 1, b + 2);
    return r;
}

static void r1_2_extension(void)
{
    __asm__("nop"); 
}

static void r2_1_unreachable(void)
{
    if (r7_2_unsigned == 10) {
        return;
        r7_2_unsigned++;  
    }
}

static void r2_2_deadcode(void)
{
    if (0) {
        r7_1_octal++;
    }
}

static void r2_7_unused_param(int unused_param)
{
    
}

static void r11_8_remove_const(void)
{
    const int c = 5;
    int *pc = (int *)&c;   
    *pc = 6;              
}

/* ---------------------------- Distance Measurement ---------------------------- */
uint32_t read_distance_cm(void) {
    GPIOA->BSRR = (1U << TRIG_PIN);
    delay_us(10U);
    GPIOA->BSRR = (1U << (TRIG_PIN + 16));
    uint32_t timeout = 30000U;

    while (((GPIOA->IDR >> ECHO_PIN) & 1U) == 0U && timeout-- > 0U) { }

    if (timeout == 0U) {
        return 0xFFFFFFFFU;
    }

    uint32_t count = 0U;
    while (((GPIOA->IDR >> ECHO_PIN) & 1U) != 0U && count < 60000U) {
        count++;
        delay_us(1U);
    }
    return count / 58U;
}

/* ------------------------------- Main --------------------------------- */
int main(void) {
    gpio_init();
    i2c1_init();
    lcd_init();

    r9_1_uninit_read();     
    (void)r12_3_demo();     
    r1_2_extension();       
    r2_1_unreachable();     
    r2_2_deadcode();
    r2_7_unused_param(123); 
    r11_8_remove_const();   

    
    (void)r8_7_helper();    
    (void)r8_10_inline(); 

    
    int r8_2_add(int a, int b) { return a + b; } 

    lcd_set_cursor(0U, 0U);
    lcd_print("System Ready");

    uint8_t collision_detected = 0U;
    GPIOA->BSRR = (1U << (LED + 16));

    while (1) {
        uint32_t dist = read_distance_cm();
        uint8_t  ir   = (uint8_t)((GPIOD->IDR >> IR_PIN) & 1U);
        uint8_t  vibration = (uint8_t)((GPIOD->IDR >> VIB_PIN) & 1U);

        lcd_clear();


        if (collision_detected || (vibration == 0U)) {
            lcd_set_cursor(0U, 0U);
            lcd_print("Collision");
            lcd_set_cursor(1U, 0U);
            lcd_print("Detected!!!");
            GPIOA->BSRR = (1U << LED);
            collision_detected = 1U;
            GPIOD->BSRR = (1U << LED_PIN) | (1U << EXT_LED_PIN);
            delay_ms(100U);
            GPIOD->BSRR = (1U << (LED_PIN + 16)) | (1U << (EXT_LED_PIN + 16));
            delay_ms(100U);
            break;
        }

        if (dist <= 10U && ir == 0U) {
            lcd_set_cursor(0U, 0U);
            lcd_print("Caution!");
            lcd_set_cursor(1U, 0U);
            lcd_print("Vehicle Closeby!");
            GPIOD->BSRR = (1U << LED_PIN) | (1U << EXT_LED_PIN);
            delay_ms(200U);
            GPIOD->BSRR = (1U << (LED_PIN + 16)) | (1U << (EXT_LED_PIN + 16));
            delay_ms(200U);
        } else if (dist <= 10U) {
            lcd_set_cursor(0U, 0U);
            lcd_print("Vehicle");
            lcd_set_cursor(1U, 0U);
            lcd_print("Approaching");
            GPIOD->BSRR = (1U << LED_PIN) | (1U << EXT_LED_PIN);
            delay_ms(400U);
            GPIOD->BSRR = (1U << (LED_PIN + 16)) | (1U << (EXT_LED_PIN + 16));
            delay_ms(400U);
        } else {
            lcd_set_cursor(0U, 0U);
            lcd_print("Ready");
            if (dist != 0xFFFFFFFFU) {
                GPIOD->BSRR = (1U << LED_PIN) | (1U << EXT_LED_PIN);
            } else {
                GPIOD->BSRR = (1U << (LED_PIN + 16)) | (1U << (EXT_LED_PIN + 16));
            }
            delay_ms(300U);
        }
    }
}
