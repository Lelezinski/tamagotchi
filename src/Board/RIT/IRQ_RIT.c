/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "../../utils.h"
#include "RIT.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void b0_routine(void);
void b1_routine(void);
void b2_routine(void);
void joy_left_routine(void);
void joy_right_routine(void);
void joy_up_routine(void);
void joy_down_routine(void);
void joy_select_routine(void);

//extern volatile char down_0;
//extern volatile char down_1;
//extern volatile char down_2;
extern uint32_t se_menu[];
extern uint32_t se_eat[];
extern uint32_t se_cuddle[];
extern volatile Game game;

uint8_t joy = 0;
uint8_t menuSelection = 0;

void RIT_IRQHandler(void)
{
	// Wait for routines to stop
	disable_RIT();

	/* buttons management */
/* 	if (down_0)
	{
		// If button was pressed, re-check the button status
		if ((LPC_GPIO2->FIOPIN & (1 << 10)) == 0)
		{
			// If still pressed, then execute code
			b0_routine();
		}
		// Button released, discard and re-enable interrupts
		down_0 = 0;
		LPC_PINCON->PINSEL4 |= (1 << 20);
		NVIC_EnableIRQ(EINT0_IRQn);
	}

	if (down_1)
	{
		// If button was pressed, re-check the button status
		if ((LPC_GPIO2->FIOPIN & (1 << 11)) == 0)
		{
			// If still pressed, then execute code
			b1_routine();
		}
		// Button released, discard and re-enable interrupts
		LPC_PINCON->PINSEL4 |= (1 << 22);
		NVIC_EnableIRQ(EINT1_IRQn);
		down_1 = 0;
	}

	if (down_2)
	{
		// If button was pressed, re-check the button status
		if ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0)
		{
			// If still pressed, then execute code
			b2_routine();
		}
		// Button released, discard and re-enable interrupts
		down_2 = 0;
		LPC_PINCON->PINSEL4 |= (1 << 24);
		NVIC_EnableIRQ(EINT2_IRQn);
	} */

	/* joystick management */
	if ((LPC_GPIO1->FIOPIN & (1 << 25)) == 0)
	{ /* Joytick Select pressed */
		if (joy != 1)
		{
			joy = 1;
			joy_select_routine();
		}
	}
	else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0)
	{ /* Joytick down pulled */
		if (joy != 2)
		{
			joy = 2;
			joy_down_routine();
		}
	}
	else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0)
	{ /* Joytick left pulled */
		if (joy != 3)
		{
			joy = 3;
			joy_left_routine();
		}
	}
	else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0)
	{ /* Joytick right pulled */
		if (joy != 4)
		{
			joy = 4;
			joy_right_routine();
		}
	}
	else if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0)
	{ /* Joytick up pulled */
		if (joy != 5)
		{
			joy = 5;
			joy_up_routine();
		}
	}
	else
	{
		joy = 0;
	}

	/* Potmeter Read */
	ADC_start_conversion();
	
	/* Touch Read */
	if((game.status == STATE_IDLE) && (getDisplayPoint(&display, Read_Ads7846(), &matrix))){
		// Check for Coords inside the sprite area
		if(((display.x > 72) && (display.x < 72 + 96)) && ((display.y > 130) && (display.y < 130 + 96)))
		{
			// Cuddle
			playSong(se_cuddle, 100);
			game.status = STATE_CUDDLE;
		}
	}

	// Clears Flag
	LPC_RIT->RICTRL |= 1;
	reset_RIT();
	enable_RIT();
}

// BUTTON ROUTINES
void b0_routine(void)
{

}


void b1_routine(void)
{

}

void b2_routine(void)
{

}

// JOYSTICK ROUTINES
void joy_left_routine(void)
{
	if (menuSelection > 0 && game.status == STATE_IDLE)
	{
		playSong(se_menu, 200);
		menuSelection--;
		GUI_Rect(0, MAX_Y - 10, MAX_X/2 - 2, 10, Red);
		GUI_Rect(MAX_X/2 + 2, MAX_Y - 10, MAX_X/2 - 2, 10, BKG_Color_1);
	}
}

void joy_right_routine(void)
{
	if (menuSelection < 1 && game.status == STATE_IDLE)
	{
		playSong(se_menu, 200);
		menuSelection++;
		GUI_Rect(MAX_X/2 + 2, MAX_Y - 10, MAX_X/2 - 2, 10, Red);
		GUI_Rect(0, MAX_Y - 10, MAX_X/2 - 2, 10, BKG_Color_1);
	}
	
}

void joy_up_routine(void)
{
	
}

void joy_down_routine(void)
{

}

void joy_select_routine(void)
{	
	if (game.status == STATE_IDLE)
	{
		switch (menuSelection)
		{
		case 0:
			// Snack
			if (game.happiness < MAX_STATS)
			{
				playSong(se_eat, 500);
				game.status = STATE_EAT_SNACK;
			}
			break;

		case 1:
			// Meal
			if (game.satiety < MAX_STATS)
			{
				playSong(se_eat, 500);
				game.status = STATE_EAT_MEAL;
			}
			break;

		default:
			break;
		}
	}
	else if (game.status == STATE_ESCAPED)
	{
		// Hard Reset trough NVIC Interface
		NVIC_SystemReset();
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
