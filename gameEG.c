#include <stm32f031x6.h>
#include "display.h"

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

volatile uint32_t milliseconds;

const uint16_t dino_sprite[] = { /* Dinosaur sprite data */ };
const uint16_t obstacle_sprite[] = { /* Obstacle sprite data */ };

int dino_y = 50;           // Dinosaur y-position
int jump_velocity = 0;     // Dinosaur's jump velocity
int is_jumping = 0;        // Jumping flag
int gravity = -1;          // Gravity constant
int obstacle_x = 110;      // Obstacle x-position
int score = 0;

void updateDinoPosition() {
    if (is_jumping) {
        dino_y += jump_velocity;
        jump_velocity += gravity;

        if (dino_y <= 50) {     // Reset to ground level
            dino_y = 50;
            jump_velocity = 0;
            is_jumping = 0;
        }
    }
}

void updateObstaclePosition() {
    obstacle_x -= 2;         // Move obstacle leftward
    if (obstacle_x < 0) {    // Respawn the obstacle
        obstacle_x = 110;
        score++;
    }
}

int checkCollision() {
    return isInside(obstacle_x, 50, 12, 16, 20, dino_y); // Checks if dinosaur and obstacle collide
}

int main() {
    initClock();
    initSysTick();
    setupIO();
    putImage(20, dino_y, 12, 16, dino_sprite, 0, 0);

    while (1) {
        if ((GPIOA->IDR & (1 << 8)) == 0 && !is_jumping) {  // Up button pressed
            is_jumping = 1;
            jump_velocity = 5;    // Jump start velocity
        }

        updateDinoPosition();
        updateObstaclePosition();

        fillRectangle(20, dino_y + 16, 12, 16, 0);           // Clear previous position
        putImage(20, dino_y, 12, 16, dino_sprite, 0, 0);      // Draw dino
        fillRectangle(obstacle_x + 12, 50, 12, 16, 0);        // Clear old obstacle
        putImage(obstacle_x, 50, 12, 16, obstacle_sprite, 0, 0); // Draw new obstacle

        if (checkCollision()) {
            printTextX2("Game Over", 10, 20, RGBToWord(255, 0, 0), 0);
            break;
        }

        delay(50);
    }
    return 0;
}

void initSysTick(void) {
    SysTick->LOAD = 48000;
    SysTick->CTRL = 7;
    SysTick->VAL = 10;
    __asm(" cpsie i "); // enable interrupts
}

void SysTick_Handler(void) {
    milliseconds++;
}

void initClock(void) {
    RCC->CR &= ~(1u<<24);
    while (RCC->CR & (1 <<25));
    FLASH->ACR |= (1 << 0);
    FLASH->ACR &= ~((1u << 2) | (1u << 1));
    FLASH->ACR |= (1 << 4);
    RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
    RCC->CFGR |= ((1 << 21) | (1 << 19));
    RCC->CFGR |= (1 << 14);
    RCC->CR |= (1 << 24);
    RCC->CFGR |= (1 << 1);
}

void delay(volatile uint32_t dly) {
    uint32_t end_time = dly + milliseconds;
    while (milliseconds != end_time)
        __asm(" wfi ");
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber) {
    Port->PUPDR = Port->PUPDR & ~(3u << BitNumber * 2);
    Port->PUPDR = Port->PUPDR | (1u << BitNumber * 2);
}

void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode) {
    uint32_t mode_value = Port->MODER;
    Mode = Mode << (2 * BitNumber);
    mode_value = mode_value & ~(3u << (BitNumber * 2));
    mode_value = mode_value | Mode;
    Port->MODER = mode_value;
}

int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py) {
    uint16_t x2, y2;
    x2 = x1 + w;
    y2 = y1 + h;
    return ((px >= x1) && (px <= x2) && (py >= y1) && (py <= y2));
}

void setupIO() {
    RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
    display_begin();
    pinMode(GPIOA, 8, 0);
    enablePullUp(GPIOA, 8);
}
