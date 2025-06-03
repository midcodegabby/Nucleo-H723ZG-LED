//	Author: Gabriel Rodgers
//	Date: 2025-06-02
//	Purpose: To get LD1, LD2, and LD3 on the Nucleo-F446RE to turn on. 

#include <stdint.h>

#define GPIOB 0x58020400
#define GPIOC 0x58020800
#define GPIOE 0x58021000
#define RCC 0x58024400

#define GPIOB_MODER (*((volatile uint32_t *) GPIOB))
#define GPIOB_OTYPER (*((volatile uint32_t *) (GPIOB + 0x04)))
#define GPIOB_OSPEEDR (*((volatile uint32_t *) (GPIOB + 0x08)))
#define GPIOB_PUPDR (*((volatile uint32_t *) (GPIOB + 0x0C)))
#define GPIOB_IDR (*((volatile uint32_t *) (GPIOB + 0x10)))
#define GPIOB_BSRR (*((volatile uint32_t *) (GPIOB + 0x18)))

#define GPIOC_MODER (*((volatile uint32_t *) GPIOC))
#define GPIOC_OTYPER (*((volatile uint32_t *) (GPIOC + 0x04)))
#define GPIOC_OSPEEDR (*((volatile uint32_t *) (GPIOC + 0x08)))
#define GPIOC_PUPDR (*((volatile uint32_t *) (GPIOC + 0x0C)))
#define GPIOC_IDR (*((volatile uint32_t *) (GPIOC + 0x10)))
#define GPIOC_BSRR (*((volatile uint32_t *) (GPIOC + 0x18)))

#define GPIOE_MODER (*((volatile uint32_t *) GPIOE))
#define GPIOE_OTYPER (*((volatile uint32_t *) (GPIOE + 0x04)))
#define GPIOE_OSPEEDR (*((volatile uint32_t *) (GPIOE + 0x08)))
#define GPIOE_PUPDR (*((volatile uint32_t *) (GPIOE + 0x0C)))
#define GPIOE_IDR (*((volatile uint32_t *) (GPIOE + 0x10)))
#define GPIOE_BSRR (*((volatile uint32_t *) (GPIOE + 0x18)))

#define RCC_CR (*((volatile uint32_t *) RCC)) 	//sysclk config
#define RCC_CFGR (*((volatile uint32_t *) (RCC + 0x010)))	//clk config
#define RCC_AHB4ENR (*((volatile uint32_t *) (RCC + 0x0E0)))	//clk enable for gpio peripherals
								
//LD1: PB0
//LD2: PE1
//LD3: PB14
//B1: PC13

void led_init(void){
	RCC_AHB4ENR |= (1 << 1); //enable GPIOB clk
	RCC_AHB4ENR |= (1 << 2); //enable GPIOC clk
	RCC_AHB4ENR |= (1 << 4); //enable GPIOE clk
	
	for (volatile uint8_t i = 0; i < 10; i++);

	GPIOB_MODER &= ~(1 << 1); //output mode for PB0
	GPIOB_MODER &= ~(1 << 29); //output mode for PB14
	GPIOE_MODER &= ~(1 << 3); //output mode for PE1
	GPIOC_MODER &= ~(0x3 << 26); //input mode for PC13

	GPIOC_PUPDR |= (1 << 26); //pull up R for PC13

	
}

int main(void) {
	led_init();

	uint32_t val = 0;

	//turn on all 3 LEDs
	
	GPIOB_BSRR |= (1 << 0);
	GPIOB_BSRR |= (1 << 14); 
	GPIOE_BSRR |= (1 << 1);
	

	while (1) {
		
		if (val == 100000) {

			//GPIOB_ODR ^= (1 << 5); // set led ON
			val = 0;
		}

		val++;
	}

	return 0;
}
