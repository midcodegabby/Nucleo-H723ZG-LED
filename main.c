//	Author: Gabriel Rodgers
//	Date: 2025-06-02
//	Purpose: To get LD1, LD2, and LD3 on the Nucleo-F446RE to turn on. 

#include <stdint.h>

#define GPIOB 0x58020400
#define GPIOC 0x58020800
#define GPIOE 0x58021000
#define RCC 0x58024400
#define SYSCFG 0x58000400
#define EXTI 0x58000000

#define NVIC_ISER 0xE000E100
#define NVIC_ICER 0xE000E180
#define NVIC_ICPR 0xE000E280
#define NVIC_IPR 0xE000E400


#define GPIOB_MODER (*((volatile uint32_t *) GPIOB))
#define GPIOB_OTYPER (*((volatile uint32_t *) (GPIOB + 0x04)))
#define GPIOB_OSPEEDR (*((volatile uint32_t *) (GPIOB + 0x08)))
#define GPIOB_PUPDR (*((volatile uint32_t *) (GPIOB + 0x0C)))
#define GPIOB_IDR (*((volatile uint32_t *) (GPIOB + 0x10)))
#define GPIOB_ODR (*((volatile uint32_t *) (GPIOB + 0x14)))
#define GPIOB_BSRR (*((volatile uint32_t *) (GPIOB + 0x18)))

#define GPIOC_MODER (*((volatile uint32_t *) GPIOC))
#define GPIOC_OTYPER (*((volatile uint32_t *) (GPIOC + 0x04)))
#define GPIOC_OSPEEDR (*((volatile uint32_t *) (GPIOC + 0x08)))
#define GPIOC_PUPDR (*((volatile uint32_t *) (GPIOC + 0x0C)))
#define GPIOC_IDR (*((volatile uint32_t *) (GPIOC + 0x10)))
#define GPIOC_ODR (*((volatile uint32_t *) (GPIOC + 0x14)))
#define GPIOC_BSRR (*((volatile uint32_t *) (GPIOC + 0x18)))

#define GPIOE_MODER (*((volatile uint32_t *) GPIOE))
#define GPIOE_OTYPER (*((volatile uint32_t *) (GPIOE + 0x04)))
#define GPIOE_OSPEEDR (*((volatile uint32_t *) (GPIOE + 0x08)))
#define GPIOE_PUPDR (*((volatile uint32_t *) (GPIOE + 0x0C)))
#define GPIOE_IDR (*((volatile uint32_t *) (GPIOE + 0x10)))
#define GPIOE_ODR (*((volatile uint32_t *) (GPIOE + 0x14)))
#define GPIOE_BSRR (*((volatile uint32_t *) (GPIOE + 0x18)))

#define RCC_CR (*((volatile uint32_t *) RCC)) 	//sysclk config
#define RCC_CFGR (*((volatile uint32_t *) (RCC + 0x010)))	//clk config
#define RCC_AHB4ENR (*((volatile uint32_t *) (RCC + 0x0E0)))	//clk enable for gpio peripherals
#define RCC_APB4ENR (*((volatile uint32_t *) (RCC + 0x0F4)))	//clk enable for SYSCFG


#define SYSCFG_EXTICR4 (*((volatile uint32_t *) (SYSCFG + 0x14)))		

#define EXTI_RTSR1 (*((volatile uint32_t *) (EXTI)))		
#define EXTI_FTSR1 (*((volatile uint32_t *) (EXTI + 0x04)))		
#define EXTI_CPUIMR1 (*((volatile uint32_t *) (EXTI + 0x80)))		
#define EXTI_CPUEMR1 (*((volatile uint32_t *) (EXTI + 0x84)))		
#define EXTI_CPUPR1 (*((volatile uint32_t *) (EXTI + 0x88)))		

#define NVIC_ISER1 (*((volatile uint32_t *) (NVIC_ISER + 0x04)))		
#define NVIC_ICER1 (*((volatile uint32_t *) (NVIC_ICER + 0x04)))		
#define NVIC_ICPR1 (*((volatile uint32_t *) (NVIC_ICPR + 0x04)))		
#define NVIC_IPR10 (*((volatile uint32_t *) (NVIC_IPR + 0x28)))		

typedef enum {
	LD1_GREEN,
	LD2_YELLOW,
	LD3_RED,
} led_t;

//LD1: PB0
//LD2: PE1
//LD3: PB14
//B1: PC13

void led_init(void){
	RCC_AHB4ENR |= (1 << 1); //enable GPIOB clk
	RCC_AHB4ENR |= (1 << 4); //enable GPIOE clk
	
	for (volatile uint8_t i = 0; i < 10; i++);

	GPIOB_MODER &= ~(1 << 1); //output mode for PB0
	GPIOB_MODER &= ~(1 << 29); //output mode for PB14
	GPIOE_MODER &= ~(1 << 3); //output mode for PE1
}

void button_init(void) {
	RCC_AHB4ENR |= (1 << 2); //enable GPIOC clk
	RCC_APB4ENR |= (1 << 1); //enable clock for SYSCFG
				 
	for (volatile uint8_t i = 0; i < 10; i++);

	GPIOC_MODER &= ~(0x3 << 26); //input mode for PC13
	GPIOC_PUPDR |= (1 << 27); //pull down R for PC13

	//sets up an interrupt to trigger when user button 1 (PC13) is pushed (line 13 = PC13)
	NVIC_ISER1 |= (1 << 8); //enable interrupt 40
				
	SYSCFG_EXTICR4 |= (0x2 << 4); //set EXTI line 13 to PC13
	EXTI_CPUEMR1 |= (1 << 13); //unmask event line 13
	EXTI_RTSR1 |= (1 << 13); //enable rising edge trigger for line 13
	
	EXTI_CPUIMR1 |= (1 << 13); //unmask interrupt line 13
}


void EXTI15_10_IRQHandler(void) {
	EXTI_CPUIMR1 &= ~(1 << 13); //mask interrupts and events from line 13
	EXTI_CPUEMR1 &= ~(1 << 13);

	GPIOB_ODR ^= (1 << 0);
	GPIOE_ODR ^= (1 << 1);
	GPIOB_ODR ^= (1 << 14); 

	NVIC_ICPR1 |= (1 << 8); //clear pending interrupt 40
	EXTI_CPUPR1 |= (1 << 13); //clear pending interrupts on line 13
	EXTI_CPUIMR1 |= (1 << 13); //unmask interrupt line 13
	EXTI_CPUEMR1 |= (1 << 13); //unmask event line 13
}
	

int main(void) {
	led_init();
	button_init();

	GPIOB_BSRR |= (1 << 0);
	GPIOB_BSRR |= (1 << 14); 
	GPIOE_BSRR |= (1 << 1);

	while (1) {
	}

	return 0;
}
