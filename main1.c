#include <stm32f031x6.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"
void initClock(void);
void initSound(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void playTune(uint32_t *frequencies, uint32_t length, uint32_t duration);

volatile uint32_t milliseconds;
uint32_t background_tune_notes;
uint32_t background_tune_note_count;
uint32_t background_repeat_tune;
uint32_t background_tune_times;


const uint16_t deco1[]=
{
0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7940,7940,7940,7940,0,0,0,
};
const uint16_t deco2[]= 
	{
0,0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,40224,7936,65535,7936,0,0,0,0,0,0,0,40224,40224,7936,65535,7936,0,0,0,0,0,0,65315,40224,40224,7936,65535,40224,0,0,0,0,0,0,0,65315,0,65315,65315,65315,65315,0,0,
	};
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};
const uint32_t my_notes[]={A4,C3,B5,D3,F2};
const uint32_t my_note_times[] = {200, 100, 300, 200, 500};


int main()
{
	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	uint16_t x = 50;
	uint16_t y = 50;
	uint16_t oldx = x;
	uint16_t oldy = y;
	initClock();
	initSound();
	background_tune_notes=my_notes;
	background_tune_times=my_note_times;
	background_tune_note_count=5;
	background_repeat_tune=1;
	initSysTick();
	setupIO();
	putImage(20,80,12,16,dg1,0,0);

	background_tune_notes = my_notes;
	background_tune_note_count = 5;
	background_repeat_tune = 1;
	background_tune_times = my_note_times;

	while(1)
	{
		playNote(440);
		delay_ms(500);

		uint32_t melody[] = {440, 494, 523, 586, 659};
		playTune(melody, 5, 300);
		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (x < 110)
			{
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}						
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			
			if (x > 10)
			{
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}			
		}
		if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{
			if (y < 140)
			{
				y = y + 1;			
				vmoved = 1;
				vinverted = 0;
			}
		}
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{			
			if (y > 16)
			{
				y = y - 1;
				vmoved = 1;
				vinverted = 1;
			}
		}
		if ((vmoved) || (hmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;					
			if (hmoved)
			{
				if (toggle)
					putImage(x,y,12,16,deco1,hinverted,0);
				else
					putImage(x,y,12,16,deco2,hinverted,0);
				
				toggle = toggle ^ 1;
			}
			else
			{
				putImage(x,y,12,16,deco3,0,vinverted);
			}
			// Now check for an overlap by checking to see if ANY of the 4 corners of deco are within the target area
			if (isInside(20,80,12,16,x,y) || isInside(20,80,12,16,x+12,y) || isInside(20,80,12,16,x,y+16) || isInside(20,80,12,16,x+12,y+16) )
			{
				printTextX2("GLUG!", 10, 20, RGBToWord(0xff,0xff,0), 0);
			}
		}		
		delay(50);

		char serial_char;
		if (serial_available())
			serial_char = egetchar();
		else
			serial_char = 0;

		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		if ( ((GPIOB->IDR & (1 << 4))==0) || (serial_char == 'r'));// right pressed
	}
	return 0;
}

void game() {
	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	uint16_t x = 50;
	uint16_t y = 50;
	uint16_t oldx = x;
	uint16_t oldy = y;
	initClock();
	initSound();
	background_tune_notes=my_notes;
	background_tune_times=my_note_times;
	background_tune_note_count=5;
	background_repeat_tune=1;
	initSysTick();
	setupIO();
	putImage(20,80,12,16,dg1,0,0);

	background_tune_notes = my_notes;
	background_tune_note_count = 5;
	background_repeat_tune = 1;
	background_tune_times = my_note_times;

	while(1)
	{
		playNote(440);
		delay_ms(500);

		uint32_t melody[] = {440, 494, 523, 586, 659};
		playTune(melody, 5, 300);
		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (x < 110)
			{
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}						
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			
			if (x > 10)
			{
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}			
		}
		if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{
			if (y < 140)
			{
				y = y + 1;			
				vmoved = 1;
				vinverted = 0;
			}
		}
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{			
			if (y > 16)
			{
				y = y - 1;
				vmoved = 1;
				vinverted = 1;
			}
		}
		if ((vmoved) || (hmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;					
			if (hmoved)
			{
				if (toggle)
					putImage(x,y,12,16,deco1,hinverted,0);
				else
					putImage(x,y,12,16,deco2,hinverted,0);
				
				toggle = toggle ^ 1;
			}
			else
			{
				putImage(x,y,12,16,deco3,0,vinverted);
			}
			// Now check for an overlap by checking to see if ANY of the 4 corners of deco are within the target area
			if (isInside(20,80,12,16,x,y) || isInside(20,80,12,16,x+12,y) || isInside(20,80,12,16,x,y+16) || isInside(20,80,12,16,x+12,y+16) )
			{
				printTextX2("GLUG!", 10, 20, RGBToWord(0xff,0xff,0), 0);
			}
		}		
		delay(50);

		char serial_char;
		if (serial_available())
			serial_char = egetchar();
		else
			serial_char = 0;

		hmoved = vmoved = 0;
		hinverted = vinverted = 0;
		if ( ((GPIOB->IDR & (1 << 4))==0) || (serial_char == 'r'));// right pressed
	}
}
void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
/*void SysTick_Handler(void)
{
	milliseconds++;
	static int index=0;
	static int current_note_timer;
	// background tune handling
	if (background_tune_notes != 0)
	{
		// if there is a tune...
		if (current_note_timer == 0)
		{
			// .. move on to the next note (if there is one)
			index++;
			if (index >= background_tune_note_count)
			{
				// no more notes.
				if (background_repeat_tune == 0)
				{
					// don't repeat
					background_tune_notes = 0;
				}
				else
				{
					// reset to first note and repeat...
					index = 0;
				}
			}
			else
			{
				//current_note_timer = background_tune_times[index];
				//playNote(background_tune_notes[index]);
			}

		}
		if (current_note_timer != 0)
			current_note_timer-- ;
	}
}*/

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

void playTune(uint32_t *frequencies, uint32_t length, uint32_t duration) {
    for (uint32_t i = 0; i < length; i++) {
        playNote(frequencies[i]);
        // Add a delay for the duration of the note
        delay_ms(duration);
    }
}



void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	pinMode(GPIOB, 0, 1);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}
