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

//game variables
int dino_y = 50;
int jump_velocity = 0;
int is_jumping = 0;
int gravity = 1;
int jumpUp = -10;
int obstacle_x = 100;
int score = 0;
int gameover = 0;

void updateDinoPos();
void updateObstaclePos();
int collisionCheck();


volatile uint32_t milliseconds;

const uint16_t white[]=
{
65535,65535,0,0,0,65535,0,0,0,0,0,0,65535,0,0,65535,65535,0,24211,65204,24211,0,24211,65204,65204,65204,65204,24211,0,24211,24211,0,0,65204,65204,65204,65204,24211,65204,65204,65204,65204,65204,65204,24211,0,65204,0,0,65204,65204,65204,65204,65204,65204,65204,65204,65204,65204,65204,65204,24211,65204,0,0,24211,65204,65204,65204,65204,65204,65204,65204,0,65204,0,65204,65204,65204,0,65535,0,24211,65204,65204,65204,65204,65204,65204,0,65204,0,65204,65204,24211,0,65535,0,0,24211,65204,65204,65204,65204,65204,0,65204,0,65204,65204,0,65535,65535,0,24211,65204,65204,65204,24211,24211,65204,65204,65204,65204,65204,24211,0,65535,65535,0,24211,65204,65204,65204,65204,65204,65204,65204,65204,65204,65204,65204,0,65535,65535,65535,0,65204,65204,65204,65204,65204,65204,65204,0,65204,65204,24211,0,65535,65535,65535,0,24211,65204,65204,65204,65204,65204,65204,65204,65204,65204,0,65535,65535,65535,0,3600,0,24211,65204,65204,65204,65204,65204,65204,65204,24211,0,65535,65535,65535,0,4368,3600,0,0,24211,65204,65204,65204,24211,24211,0,65535,65535,65535,65535,0,4368,4368,4368,0,0,0,0,0,0,0,65535,65535,65535,65535,65535,0,3600,3600,0,65535,0,3600,3600,0,65535,65535,65535,65535,65535,65535,65535,65535,0,0,65535,65535,65535,0,0,65535,65535,65535,65535,65535,65535,65535,
};

const uint16_t deco2[]= 
	{
65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,
	};
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};

int main()
{
	initClock();
	initSysTick();
	setupIO();
	putImage(20,dino_y,16,16,white,0,0);
	displayScore(score);
	uint32_t scoreUpdate = 0;
	

	while(1)
	{
		if((GPIOA->IDR & (1<<8)) == 0 && !is_jumping){ //up button is pressed
			is_jumping = 1;
			jump_velocity = jumpUp; //jump starts velocity
		} 

		updateDinoPos();
		updateObstaclePos();

		fillRectangle(20,dino_y+16,16,16,0);
		putImage(20, dino_y,16,16,white,0,0);
		fillRectangle(obstacle_x+12,50,16,16,0);
		putImage(obstacle_x,50,16,16,deco2,0,0);

		if ((milliseconds - scoreUpdate) >= 200) {
			score++;
			displayScore(score);
			scoreUpdate = milliseconds;
		}

		if(collisionCheck()){
			printTextX2("game over",10,20,RGBToWord(255,0,0),0);
			break;
		}
		delay(50);
	}
	return 0;
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


//Dino mech functions 2
void updateDinoPos(){
    if(is_jumping){ 
        dino_y += jump_velocity;   // Apply the velocity to dino's Y position
        jump_velocity += gravity;  // Gravity slows the upward movement

        // Check if dino hits the ground
        if(dino_y >= 50){
            dino_y = 50;           // Reset to ground level
            jump_velocity = 0;     // Reset jump velocity
            is_jumping = 0;        // End the jump
        }
    }
}


void updateObstaclePos(){
	obstacle_x -= 2; //moves obstacles left
	if(obstacle_x < 0){ //generates the obstacle
		obstacle_x = 110;
		score++;
	}
}

int collisionCheck(){
	return isInside(obstacle_x, 50,12,16,20, dino_y); //checks to see if dino collides with the obstacle
}
