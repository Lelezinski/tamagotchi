/*----------------------------------------------------------------------------
	Includes & Defs
 *----------------------------------------------------------------------------*/

#include "utils.h"

#ifdef SIMULATOR
	extern uint8_t ScaleFlag;
#endif

extern uint8_t ageBuffer[12];
extern volatile Game game;

/*----------------------------------------------------------------------------
	Main Program
 *----------------------------------------------------------------------------*/

int main(void)
{
	uint8_t i = 0;

	// System Initialization
	SysInit();
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);

	// LCD Background Setup
	LCD_Clear(White);
	GUI_HLine(0, 40, BKG_Color_1);
	GUI_HLine(40, 30, DarkGrey);
	GUI_HLine(270, 50, BKG_Color_2);
	GUI_HLine(310, 10, BKG_Color_1);
	GUI_Text(5, 72, (uint8_t *)"Happiness", Black, White);
	GUI_Text((MAX_X - (7*8) - 5), 72, (uint8_t *)"Satiety", Black, White);
	GUI_Rect(MAX_X/2 - 1, 40, 2, 30, White);
	GUI_Rect(MAX_X/2 - 2, 270, 4, 50, White);
	GUI_Rect(0, MAX_Y - 10, MAX_X/2 - 2, 10, Red);
	GUI_Text((MAX_X/4 - (9*8)/2), 282, (uint8_t *)"Chocolate", Black, BKG_Color_2);
	GUI_Text((MAX_X*3/4 - (5*8)/2), 282, (uint8_t *)"Pizza", Black, BKG_Color_2);

	// Animation Timer Setup
	// 1ms Prescaler, 1s MR0 to toggle animation (Interrupts and Reset the timer)
	LPC_TIM0->PR = 25000;
	LPC_TIM0->MR0 = 1000;
	LPC_TIM0->MCR |= 3;
	NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_SetPriority(TIMER0_IRQn, 2);

	// Stat Timer Setup
	// 1ms Prescaler, 5s MR0 to decrease stats (Interrupts and Reset the timer)
	LPC_TIM1->PR = 25000;
	LPC_TIM1->MR0 = 5000;
	LPC_TIM1->MCR |= 3;
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_SetPriority(TIMER1_IRQn, 3);

	// Note Timer Setup
	LPC_SC->PCONP |= 1 << 22;
	LPC_TIM2->PR = 0;
	LPC_TIM2->MR0 = 25000;
	LPC_TIM2->MCR |= 3;
	NVIC_EnableIRQ(TIMER2_IRQn);
	NVIC_SetPriority(TIMER2_IRQn, 1);
	
	// Note Change Timer Setup
	LPC_SC->PCONP |= 1 << 21;
	LPC_TIM3->PR = 25000;
	LPC_TIM3->MR0 = 1000;
	LPC_TIM3->MCR |= 3;
	NVIC_EnableIRQ(TIMER3_IRQn);
	NVIC_SetPriority(TIMER3_IRQn, 0);

	// Less priority to user input (RIT)
	NVIC_SetPriority(RIT_IRQn, 4);

	// Initial Draw and Start
	toHHMMSS(ageBuffer, game.age.hrs, game.age.min, game.age.sec);
	GUI_Text((MAX_X/2 - (12*8)/2), 12, ageBuffer, Black, BKG_Color_1);
	for (i = 0; i < MAX_STATS; i++)
	{
		GUI_Rect(5 + (19 * i), 45, 14, 20, Hap_Color);
		GUI_Rect(MAX_X - 19 - (19 * i), 45, 14, 20, Sat_Color);
	}
	LPC_TIM0->TCR = 1;
	LPC_TIM1->TCR = 1;
	enable_RIT();

	// Idle loop
	while (1) 
	{
		//__ASM("WFI");
	}
}
