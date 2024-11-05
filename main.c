#include <stm32f031x6.h>
#include <stdlib.h>
#include "display.h"

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void makeBackground();



/*------------------
	game variables
-------------------*/
// function signatures for game mechanics
void updateDinoPos();
void updateObstaclePos(int speed, int random);
int collisionCheck();
void startMenu();
void multiplayer();
// game mechanics
int jump_velocity = 0;
int is_jumping = 0;
int gravity = 1;
int jumpUp = -11; // how far the dino jumps (strength)
// characters positions 
float star_x = 100;
float obstacle_air = 74; 
float obstacle_ground = 94;
float gordo_x = 100;
float dino_y = 90;

// game over 
int score = 0;
int gameover = 0;
int highScore = 0;
const uint16_t backgroundColour = 0x8ABC;
const uint16_t bottomColour = 0x3506;
volatile uint32_t milliseconds;

// game sprites
const uint16_t run1[]= 
{
35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,7042,7042,64676,64676,64676,7042,7042,0,0,35516,35516,35516,35516,35516,35516,35516,35516,0,7042,7042,64676,64676,64676,64676,64676,64676,64676,7042,7042,0,35516,35516,35516,35516,35516,35516,0,7042,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,7042,0,35516,35516,35516,35516,35516,58136,7042,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,7042,0,35516,35516,35516,35516,0,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,0,64676,64676,0,35516,35516,35516,0,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,0,64676,64676,0,35516,35516,35516,0,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,64676,0,64676,64676,0,35516,35516,35516,0,64676,64676,64676,64676,64676,7042,0,64676,64676,64676,64676,64676,0,64676,64676,0,35516,35516,35516,0,64676,64676,64676,64676,64676,64676,64676,0,64676,7042,7042,64676,64676,64676,64676,0,35516,35516,35516,0,7042,64676,64676,64676,64676,64676,64676,0,64676,64676,64676,64676,64676,64676,64676,0,35516,35516,35516,0,7042,64676,64676,64676,64676,64676,0,64676,64676,64676,64676,64676,0,64676,64676,0,35516,35516,35516,0,0,7042,7042,64676,64676,64676,0,64676,64676,64676,64676,64676,64676,64676,7042,0,35516,35516,0,0,0,0,0,0,0,0,64676,64676,64676,64676,64676,64676,64676,64676,7042,0,35516,35516,0,2568,2568,0,0,7042,64676,64676,64676,64676,64676,64676,64676,64676,64676,7042,0,35516,35516,35516,0,11536,11536,2568,2568,0,7042,64676,64676,64676,64676,64676,64676,64676,7042,0,0,0,35516,35516,0,11536,11536,11536,2568,0,7042,7042,64676,64676,64676,64676,7042,7042,0,2568,2568,11536,0,35516,0,11536,11536,11536,11536,0,0,0,0,0,0,0,0,0,2568,11536,11536,11536,0,35516,35516,0,0,0,0,35516,35516,35516,35516,35516,35516,0,2568,2568,11536,11536,11536,11536,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,35516,35516,
};
const uint16_t run2[]= 
{
35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,7042,40100,40100,40100,40100,40100,7042,7042,0,35516,35516,35516,35516,35516,35516,35516,0,7042,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,35516,35516,35516,35516,35516,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,35516,35516,35516,35516,35516,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,0,35516,35516,35516,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,0,35516,35516,35516,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,40100,0,0,35516,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,40100,0,7042,0,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,7042,0,0,7042,40100,40100,40100,40100,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,0,7042,40100,40100,40100,40100,0,40100,40100,40100,40100,40100,40100,0,40100,40100,40100,0,40100,0,35516,0,7042,40100,40100,40100,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,7042,0,35516,35516,0,7042,7042,40100,0,40100,40100,40100,40100,40100,40100,40100,40100,7042,7042,0,0,35516,35516,35516,35516,0,0,0,0,7042,40100,40100,40100,40100,40100,40100,7042,0,0,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,7042,40100,40100,40100,40100,7042,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,2568,2568,0,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,11536,11536,11536,11536,11536,2568,2568,0,2568,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,2568,11536,11536,11536,2568,0,2568,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,
};
const uint16_t run3[]= 
{
35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,7042,7042,40100,40100,40100,7042,7042,0,0,0,35516,35516,35516,35516,35516,35516,0,7042,7042,7042,40100,40100,40100,40100,40100,40100,40100,40100,7042,7042,0,35516,35516,35516,35516,0,0,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,0,0,0,35516,0,7042,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,7042,0,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,40100,40100,0,7042,0,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,40100,40100,0,7042,0,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,40100,40100,0,40100,0,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,0,40100,40100,40100,0,40100,0,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,7042,0,0,7042,40100,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,7042,0,35516,0,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,40100,40100,40100,0,0,35516,35516,35516,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,0,0,35516,35516,35516,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,0,35516,35516,35516,0,0,7042,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,0,2568,0,35516,35516,0,11536,0,0,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,7042,0,2568,0,35516,35516,0,11536,11536,11536,0,7042,40100,40100,40100,40100,40100,7042,7042,0,0,2568,2568,0,35516,35516,35516,0,11536,2568,2568,0,0,0,0,0,0,0,0,11536,11536,11536,2568,0,35516,35516,35516,35516,0,2568,2568,2568,2568,0,35516,35516,0,2568,2568,11536,11536,11536,0,35516,35516,35516,35516,35516,35516,0,0,0,0,35516,35516,35516,35516,0,0,0,0,0,35516,35516,35516,
};
const uint16_t star1[]= 
{
35516,35516,35516,35516,35516,35516,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,0,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,47622,0,0,0,35516,35516,0,0,0,0,35516,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,47622,47622,0,0,47622,47622,47622,47622,0,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,0,0,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,0,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,0,0,47622,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,0,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,47622,47622,0,0,0,0,47622,47622,47622,0,35516,35516,35516,35516,35516,0,47622,16135,16135,47622,0,0,35516,35516,35516,35516,0,0,0,0,35516,35516,35516,35516,35516,0,47622,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,47622,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,
};
const uint16_t star2[]= 
{
35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,47622,16135,16135,47622,0,0,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,0,47622,47622,16135,16135,16135,16135,47622,47622,0,0,0,0,0,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,47622,0,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,0,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,0,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,47622,47622,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,0,47622,16135,16135,16135,47622,47622,0,0,47622,47622,16135,16135,16135,47622,0,35516,35516,35516,35516,0,47622,47622,16135,0,0,0,35516,35516,0,0,0,16135,16135,47622,0,35516,35516,35516,35516,35516,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,35516,35516,35516,
};
const uint16_t star3[]= 
{
35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,47622,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,47622,0,35516,35516,35516,35516,35516,0,0,0,0,35516,35516,35516,35516,0,0,47622,16135,16135,47622,0,35516,35516,35516,35516,35516,0,47622,47622,47622,0,0,0,0,47622,47622,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,0,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,47622,0,0,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,0,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,0,0,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,0,47622,47622,47622,47622,0,0,47622,47622,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,35516,0,0,0,0,35516,35516,0,0,0,47622,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,0,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,35516,35516,35516,35516,35516,35516,
};
const uint16_t star4[]= 
{
35516,35516,35516,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,35516,35516,35516,35516,35516,0,47622,16135,16135,0,0,0,35516,35516,0,0,0,16135,47622,47622,0,35516,35516,35516,35516,0,47622,16135,16135,16135,47622,47622,0,0,47622,47622,16135,16135,16135,47622,0,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,47622,47622,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,35516,35516,0,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,35516,35516,35516,0,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,0,35516,0,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,0,47622,47622,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,16135,47622,0,35516,0,0,0,0,0,47622,47622,16135,16135,16135,16135,47622,47622,0,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,0,0,47622,16135,16135,47622,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,16135,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,16135,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,47622,47622,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,
};
const uint16_t fall[]= 
{
35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,35516,35516,0,0,35516,0,0,0,7042,7042,40100,40100,7042,7042,0,0,35516,0,0,35516,35516,0,7042,7042,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,0,40100,7042,0,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,40100,0,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,40100,0,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,40100,7042,7042,0,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,0,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,0,40100,40100,0,40100,40100,40100,40100,0,0,35516,0,7042,40100,40100,40100,40100,40100,40100,40100,0,40100,40100,0,40100,40100,40100,0,35516,35516,35516,0,0,40100,40100,40100,40100,7042,7042,40100,0,40100,40100,0,40100,7042,40100,0,35516,35516,35516,35516,0,40100,40100,40100,40100,40100,40100,40100,0,40100,40100,0,40100,40100,40100,0,35516,35516,35516,35516,0,7042,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,40100,7042,0,35516,35516,35516,35516,0,7042,7042,0,0,40100,40100,40100,40100,40100,40100,40100,40100,7042,7042,0,35516,35516,35516,35516,0,7042,0,2568,2568,0,40100,40100,40100,40100,0,7042,0,0,0,35516,35516,35516,35516,35516,35516,0,11536,2568,2568,0,7042,40100,40100,40100,7042,0,11536,2568,2568,0,35516,35516,35516,35516,35516,0,11536,11536,11536,11536,0,40100,40100,40100,0,11536,11536,2568,2568,0,35516,35516,35516,35516,35516,0,2568,11536,11536,11536,0,40100,40100,0,2568,11536,11536,11536,11536,0,35516,35516,35516,35516,35516,0,2568,11536,2568,2568,0,7042,7042,0,2568,2568,11536,11536,0,35516,35516,35516,35516,35516,35516,35516,0,2568,2568,2568,0,0,0,0,0,2568,2568,0,35516,35516,35516,35516,35516,35516,35516,35516,35516,0,0,0,0,35516,35516,35516,0,0,0,0,35516,35516,35516,35516,
};
const uint16_t gordo1[]= 
{
0,0,35516,35516,35516,35516,35516,0,35516,35516,35516,35516,35516,35516,35516,35516,0,16119,0,0,35516,35516,0,46509,0,35516,35516,35516,35516,35516,35516,35516,35516,0,16119,46509,0,35516,0,16119,0,35516,35516,35516,0,0,35516,35516,35516,0,46509,16119,16119,0,0,0,0,0,0,0,16119,0,35516,35516,35516,35516,0,16119,0,0,0,0,0,0,0,46509,0,35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,0,0,16119,16119,0,0,16119,16119,0,0,0,0,35516,35516,0,0,0,16119,16119,0,16119,16119,0,16119,16119,0,16119,46509,0,0,46509,16119,0,16119,16119,0,16119,16119,0,16119,16119,0,0,0,35516,35516,0,0,0,0,16119,16119,0,0,16119,16119,0,0,35516,35516,35516,35516,35516,35516,0,0,0,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,35516,0,46509,0,0,0,0,0,0,0,16119,0,35516,35516,35516,35516,0,16119,0,0,0,0,0,0,0,16119,16119,46509,0,35516,35516,35516,0,0,35516,35516,35516,0,16119,0,35516,0,46509,16119,0,35516,35516,35516,35516,35516,35516,35516,35516,0,46509,0,35516,35516,0,0,16119,0,35516,35516,35516,35516,35516,35516,35516,35516,0,35516,35516,35516,35516,35516,0,0,
};
const uint16_t gordo2[]= 
{
35516,35516,35516,35516,35516,35516,35516,35516,0,35516,35516,35516,35516,35516,0,0,35516,35516,35516,35516,35516,35516,35516,0,46509,0,35516,35516,0,0,16119,0,35516,35516,0,0,35516,35516,35516,0,16119,0,35516,0,46509,16119,0,35516,35516,35516,0,16119,0,0,0,0,0,0,0,16119,16119,46509,0,35516,35516,35516,35516,0,46509,0,0,0,0,0,0,0,16119,0,35516,35516,35516,35516,35516,0,0,0,0,0,0,0,0,0,0,35516,35516,35516,35516,0,0,0,0,16119,16119,0,0,16119,16119,0,0,35516,35516,35516,0,46509,16119,0,16119,16119,0,16119,16119,0,16119,16119,0,0,0,35516,35516,0,0,0,16119,16119,0,16119,16119,0,16119,16119,0,16119,46509,0,35516,35516,35516,0,0,16119,16119,0,0,16119,16119,0,0,0,0,35516,35516,35516,35516,0,0,0,0,0,0,0,0,0,0,35516,35516,35516,35516,35516,0,16119,0,0,0,0,0,0,0,46509,0,35516,35516,35516,35516,0,46509,16119,16119,0,0,0,0,0,0,0,16119,0,35516,35516,35516,0,16119,46509,0,35516,0,16119,0,35516,35516,35516,0,0,35516,35516,0,16119,0,0,35516,35516,0,46509,0,35516,35516,35516,35516,35516,35516,35516,0,0,35516,35516,35516,35516,35516,0,35516,35516,35516,35516,35516,35516,35516,35516,
};
const uint16_t sun[]=
{
35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,57159,57159,35516,35516,35516,35516,35516,35516,65383,57159,24327,24327,57159,65383,35516,35516,35516,35516,24327,57159,24327,24327,48951,24327,35516,35516,35516,65383,24327,48951,24327,24327,24327,24327,65383,35516,35516,40743,24327,32535,24327,24327,24327,24327,40743,35516,35516,35516,24327,57159,24327,24327,32535,24327,35516,35516,35516,35516,32535,24327,32519,24327,57159,40743,35516,35516,35516,35516,35516,35516,57159,57159,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516
};
const uint16_t back[]=
{
35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,35516,
};

// main function
int main()
{
	initClock();
	initSysTick();
	setupIO();
	displayScore(score, highScore);
	startMenu(); //displays a start menu at the start
	
    while (1)
    {
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{		
			resetGame();  // Reset game state before starting the game
			runGame();
			delay(2000);
			startMenu();
			continue;
		} 
		else if ((GPIOB->IDR & (1 << 4))==0) // right pressed
       	{
			resetGame();  // Reset game state before starting the game
			multiplayer();
			continue;
	   	}
    }
	
	delay(50); // Maintain overall game speed
	return 0;
}

//start menu that prompts the players
void startMenu(){
	fillRectangle(0,0,128,160,backgroundColour);
	printTextX2("Kirby Jump", 6,20,RGBToWord(255,255,255),0x8abc);
	printText("Press Left",30,50,RGBToWord(255,255,255),0x8abc);
	printText("to Start",37,60,RGBToWord(255,255,255),0x8abc);

	printText("Press Right",30,90,RGBToWord(255,255,255),0x8abc);
	printText("for Multiplayer",15,100,RGBToWord(255,255,255),0x8abc);
}

void resetGame(void)
{
    // Reset game variables and state
    score = 0; // Reset score
    dino_y = 90; // Reset Dino position
    is_jumping = 0; // Reset jump state
    jump_velocity = 0; // Reset jump velocity
    star_x = obstacle_air;// Resets the star's position
	gordo_x = obstacle_ground;
    displayScore(score, highScore); // Display initial score
	makeBackground();
}

void runGame()
{
	uint32_t scoreUpdate = 0;
	int frame_counter = 0; // To track which animation frame to display
	unsigned int last_frame_time = 0; // Time when last frame was update
	int speed = 1;
	uint32_t speedTime = milliseconds;
	int random = (rand() % 2) + 1;

	while (1)
	{
		if ((GPIOA->IDR & (1 << 8)) == 0 && !is_jumping) 
		{ // up button is pressed
			is_jumping = 1;
			jump_velocity = jumpUp; // jump starts velocity
		}

		putImage(5, 5, 10, 10, sun, 0, 0);
		updateDinoPos();
		updateObstaclePos(speed, random);

		if ((milliseconds - speedTime) >= 50000) {
			speed++;
			speedTime = milliseconds;
		}

		// Only update the frame every 100 ms (adjust for preferred frame rate)
		if (milliseconds - last_frame_time >= 100) 
		{
			frame_counter = (frame_counter + 1) % 4; // Loop through frames 0 to 3
			last_frame_time = milliseconds;
		}

		// Clear the previous Dino jumping position
		if (dino_y < 90	)
		{
			fillRectangle(20, dino_y + 11, 20, 20, backgroundColour);
		}

		if (dino_y <= 90)
		{
			fillRectangle(20, dino_y - 11, 20, 20, backgroundColour);
		}

		// Display the correct animation based on jumping and falling state
		if (is_jumping && jump_velocity > 0) 
		{ // Jumping up: show animation frames
			putImage(20, dino_y, 20, 20, fall, 0, 0);
		} 
		else if (is_jumping && jump_velocity <= 0) 
		{ // Falling down: show fall frame
			putImage(20, dino_y, 20, 20, fall, 0, 0);
			
		}
		else 
		{ // Default running animation when on the ground
			switch (frame_counter) 
			{
				case 0: putImage(20, dino_y, 20, 20, run1, 0, 0); break;
				case 1: putImage(20, dino_y, 20, 20, run2, 0, 0); break;
				case 2: putImage(20, dino_y, 20, 20, run3, 0, 0); break;
				case 3: putImage(20, dino_y, 20, 20, run2, 0, 0); break;
			}
		}

		if (random == 1)
		{	// Clear previous position and display current frame for obstacle
			fillRectangle(star_x, obstacle_air, 24, 24, backgroundColour);

			switch (frame_counter) 
			{
				case 0: putImage(star_x, obstacle_air, 20, 20, star1, 0, 0); break;
				case 1: putImage(star_x, obstacle_air, 20, 20, star2, 0, 0); break;
				case 2: putImage(star_x, obstacle_air, 20, 20, star3, 0, 0); break;
				case 3: putImage(star_x, obstacle_air, 20, 20, star4, 0, 0); break;
			}
		}
		else if(random == 2)
		{
			fillRectangle(gordo_x+5, obstacle_ground, 16, 16, backgroundColour);

			switch (frame_counter) 
			{
				case 0: putImage(gordo_x, obstacle_ground, 16, 16, gordo1, 0, 0); break;
				case 1: putImage(gordo_x, obstacle_ground, 16, 16, gordo1, 0, 0); break;
				case 2: putImage(gordo_x, obstacle_ground, 16, 16, gordo2, 0, 0); break;
				case 3: putImage(gordo_x, obstacle_ground, 16, 16, gordo2, 0, 0); break;
			}
		}

		// tracks the player's score and updates it accordingly
		if ((milliseconds - scoreUpdate) >= 200) 
		{
			score++;
			if (highScore <= score)
			{
				highScore = score;
			}
			displayScore(score, highScore);
			scoreUpdate = milliseconds;
		}

		// checks whether dino has collided with an obstacle.
		// if yes, then the game stops and game over displays
		if(collisionCheck()){
			fillRectangle(0,0,128,160, 0x0); // black out the screen
			printTextX2("Game Over",10,60,RGBToWord(255,0,0),0);
		
			break;
		}

		delay(50); // Maintain overall game speed
	}
}

void updateDinoPos(){
    if(is_jumping){ 
        dino_y += jump_velocity; // Applies the velocity to dino's Y position
        jump_velocity += gravity; // Gravity slows the upward movement

        // Check if dino hits the ground
        if(dino_y >= 90){
            dino_y = 90; // Reset to ground level
            jump_velocity = 0;// Reset jump velocity
            is_jumping = 0; // End the jump
        }
    }

}

void updateObstaclePos(int speed, int random)
{
	if(random == 1)
	{
		star_x -= 2 * speed; //moves stars left
		if(star_x < -19)// moves the star offscreen
		{
			putImage(star_x, 88, 22, 22, back, 0, 0);
			star_x = 108;// takes it back the right side of the screen
			score++;
		}
	}
	if(random == 2)
	{
		gordo_x -= 2 * speed;
		if(gordo_x < -10)// moves the star offscreen
		{
			putImage(gordo_x, 88, 22, 22, back, 0, 0);
			gordo_x = 108;// takes it back the right side of the screen
			score++;
		}
	}

}

int collisionCheck(){
    // Check for overlap in both x and y coordinates
    if ((19 < star_x + 19) && (19 + 19 > star_x) && //20 = x-coordinate of dino
        (dino_y < obstacle_air + 19) && (dino_y + 19 > obstacle_air)) {
        return 1; // Collision detected
    }

	if ((15 < gordo_x + 15) && (15 + 15 > gordo_x) && //16 = x-coordinate of dino
    	(dino_y < obstacle_ground + 15) && (dino_y + 15 > obstacle_ground)) {
        return 1; // Collision detected
    }
    return 0; // No collision
}

void makeBackground() {
	fillRectangle(0,0,128, 110, backgroundColour);  // black out the screen
	fillRectangle(0,110,128, 50, bottomColour);  // black out the screen
}

void multiplayer() {
	int player1;
	int player2;
    char player1Text[10];
    char player2Text[10];


	// Player 1's Run Through
	fillRectangle(0,0,128,160,backgroundColour);
	printTextX2("Player 1", 6,20,RGBToWord(255,255,255),0x8abc);
	delay(2000);
	resetGame();
	runGame();
	player1 = score;

	// Player 2's Run Through
	fillRectangle(0,0,128,160,backgroundColour);
	printTextX2("Player 2", 6,20,RGBToWord(255,255,255),0x8abc);
	delay(2000);
	resetGame();
	runGame();
	player2 = score;

	fillRectangle(0,0,128,160,backgroundColour);
	printText("Player 1 Score:",10,20,RGBToWord(255,255,255),0x8abc);
    sprintf(player1Text, "%d", player1);
	printTextX2(player1Text,30,40,RGBToWord(255,255,255),0x8abc);
	delay(2000);

	printText("Player 2 Score: ",10,80,RGBToWord(255,255,255),0x8abc);
    sprintf(player2Text, "%d", player2);
	printTextX2(player2Text,30,100,RGBToWord(255,255,255),0x8abc);
	delay(2000);

	// checks to see the which player won the game
	fillRectangle(0,0,128,160,backgroundColour);
	if (player1 > player2) {
		printTextX2("Player 1", 15, 60, RGBToWord(0, 255, 0), backgroundColour);
		printTextX2("Wins!", 37, 80, RGBToWord(0, 255, 0), backgroundColour);
	} 
	else if (player1 < player2) {
		printTextX2("Player 2", 15, 60, RGBToWord(0, 255, 0), backgroundColour);
		printTextX2("Wins!", 37, 80, RGBToWord(0, 255, 0), backgroundColour);	
	}
	else {
		printTextX2("Its a", 30, 60, RGBToWord(0, 255, 0), backgroundColour);
		printTextX2("Draw!", 37, 80, RGBToWord(0, 255, 0), backgroundColour);	
	}

	delay(2000);
	startMenu();
}



void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void) {
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);    
		while(!(RCC->CR & (1<<25)));    
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}
