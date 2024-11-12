#include <stm32f031x6.h>
#include "musical_notes.h"
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void playNote(uint32_t Freq)
{	
    if (Freq == 0) {
        TIM14->CR1 &= ~(1 << 0); // Disable counter to stop sound
        return;
    }

    TIM14->ARR = (uint32_t)(65536 / Freq); // Set frequency
    TIM14->CCR1 = TIM14->ARR / 2;          // 50% duty cycle
    TIM14->CNT = 0;                        // Reset counter
    TIM14->CR1 |= (1 << 0);                // Enable counter
}
void initSound()
{
    // Enable TIM14 clock
    RCC->APB1ENR |= (1 << 8);
    pinMode(GPIOB, 1, 2);            // Set GPIOB pin 1 to alternate function
    GPIOB->AFR[0] &= ~(0x0F << 4);   // Set alternate function 0 for GPIOB1

    TIM14->CR1 = 0;                      // Reset Timer 14 control register
    TIM14->CCMR1 = (1 << 6) + (1 << 5);  // Configure PWM mode 1
    TIM14->CCER |= (1 << 0);             // Enable output on channel 1
    TIM14->PSC = 48000000UL / 65536UL;   // Prescaler to set 65536 Hz base frequency
    TIM14->ARR = 65536 / 440;            // Set ARR for 440 Hz frequency
    TIM14->CCR1 = TIM14->ARR / 2;        // 50% duty cycle
    TIM14->CNT = 0;                      // Reset counter
    TIM14->CR1 |= (1 << 0);              // Enable the counter
}
