#include "../../utils.h"
#include "timer.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern const uint8_t anim_0_bin[];
extern const uint8_t anim_1_bin[];
extern const uint8_t anim_eating_bin[];
extern const uint8_t anim_walking_1_bin[];
extern const uint8_t anim_walking_2_bin[];
extern const uint8_t anim_escaping_1_bin[];
extern const uint8_t anim_escaping_2_bin[];
extern const uint8_t food_pizza_bin[];
extern const uint8_t food_choco_bin[];
extern const uint8_t anim_cuddle_bin[];
extern const uint8_t sprite_w;
extern const uint8_t sprite_h;
extern const uint8_t food_w;
extern const uint8_t food_h;
extern volatile Game game;
extern uint32_t se_escape[];

char moves = 0; // For moving animations
uint8_t ageBuffer[12]; // Fixed string lenght of 12

void TIMER0_IRQHandler(void)
{
	// Temporarily disables RIT
	disable_RIT();

	// Checks for MR0 Interrupt
	if (LPC_TIM0->IR & 1)
	{
		if (game.status != STATE_ESCAPED)
		{
			// Updates Age
			game.age.sec += 1;
			if (game.age.sec > 59)
			{
				game.age.min += 1;
				game.age.sec = 0;
				if (game.age.min > 59)
				{
					game.age.hrs += 1;
					game.age.min = 0;
				}
			}
			// Displays Age
			toHHMMSS(ageBuffer, game.age.hrs, game.age.min, game.age.sec);
			GUI_Text((MAX_X/2 - (12*8)/2), 12, ageBuffer, Black, BKG_Color_1);
		}

		// FSM Implementation
		switch (game.status)
		{
		case STATE_IDLE:
			// Toggles Animation
			if (game.animIndex == 0)
			{
				GUI_Draw(anim_0_bin, 72, 130, sprite_w, sprite_h, 6);
			}
			else
			{
				GUI_Draw(anim_1_bin, 72, 130, sprite_w, sprite_h, 6);
			}
			game.animIndex = !game.animIndex;

			// Defaults
			LPC_TIM1->TCR = 1;
			moves = 0;
			break;
		
		case STATE_EAT_SNACK:
			// Stops Stat Timer
			LPC_TIM1->TCR = 0;

			if (moves == 0)
			{
				// Spawns Food
				GUI_Draw(food_choco_bin, 10, 220, food_w, food_h, 6);
			}
			else if (moves == MAX_MOVES)
			{
				// Eating Sprite
				GUI_Draw(anim_eating_bin, 72 - ((MAX_MOVES - 1) * MOVES_MUL), 130, sprite_w, sprite_h, 6);
			}
			else if (moves == MAX_MOVES + MAX_WAIT - 1)
			{
				// Clears Food
				GUI_Rect(10, 226, food_w * 6, food_h * 6 - 6, White);
			}
			else if (moves == MAX_MOVES + MAX_WAIT)
			{
				// Update Stat
				GUI_Rect(72 - ((MAX_MOVES - 1) * MOVES_MUL), 130, sprite_w * 6, sprite_h * 6, White);
				GUI_Rect(5 + (19 * game.happiness), 45, 14, 20, Hap_Color);
				game.happiness++;
				game.status = STATE_IDLE;
				game.animIndex = 0;
				break;
			}
			else if (moves < MAX_MOVES)
			{
				GUI_Rect(72 - ((moves - 1) * MOVES_MUL), 130, sprite_w * 6, sprite_h * 6, White);

				if (moves % 2)
				{
					GUI_Draw(anim_walking_1_bin, 72 - (moves * MOVES_MUL), 130, sprite_w, sprite_h, 6);
				}
				else
				{
					GUI_Draw(anim_walking_2_bin, 72 - (moves * MOVES_MUL), 130, sprite_w, sprite_h, 6);
				}
			}
			moves++;
			break;

		case STATE_EAT_MEAL:
			// Stops Stat Timer
			LPC_TIM1->TCR = 0;

			if (moves == 0)
			{
				// Spawns Food
				GUI_Draw(food_pizza_bin, MAX_X - 10 - (food_w * 6), 220, food_w, food_h, 6);
			}
			else if (moves == MAX_MOVES)
			{
				// Eating Sprite
				GUI_Draw(anim_eating_bin, 72 + ((MAX_MOVES - 1) * MOVES_MUL), 130, sprite_w, sprite_h, 6);
			}
			else if (moves == MAX_MOVES + MAX_WAIT - 1)
			{
				// Clears Food
				GUI_Rect(MAX_X - 10 - (food_w * 6), 226, food_w * 6, food_h * 6 - 6, White);
			}
			else if (moves == MAX_MOVES + MAX_WAIT)
			{
				// Update Stat
				GUI_Rect(72 + ((MAX_MOVES - 1) * MOVES_MUL), 130, sprite_w * 6, sprite_h * 6, White);
				GUI_Rect(MAX_X - 19 - (19 * game.satiety), 45, 14, 20, Sat_Color);
				game.satiety++;
				game.animIndex = 0;
				game.status = STATE_IDLE;
				break;
			}
			else if (moves < MAX_MOVES)
			{
				GUI_Rect(72 + ((moves - 1) * MOVES_MUL), 130, sprite_w * 6, sprite_h * 6, White);

				if (moves % 2)
				{
					GUI_Draw(anim_walking_1_bin, 72 + (moves * MOVES_MUL), 130, sprite_w, sprite_h, 6);
				}
				else
				{
					GUI_Draw(anim_walking_2_bin, 72 + (moves * MOVES_MUL), 130, sprite_w, sprite_h, 6);
				}
			}
			moves++;
			break;

		case STATE_CUDDLE:
			// Stops Stat Timer
			LPC_TIM1->TCR = 0;

			if (moves == 0)
			{
				// Cuddle Animation
				GUI_Draw(anim_cuddle_bin, 72, 130, sprite_w, sprite_h, 6);
			}
			else if (moves == MAX_MOVES_CUDDLE)
			{
				if (game.happiness < MAX_STATS)
				{
					// Adds 1x happiness
					GUI_Rect(5 + (19 * game.happiness), 45, 14, 20, Hap_Color);
					game.happiness++;
				}
				game.animIndex = 0;
				game.status = STATE_IDLE;
				break;
			}
			moves++;
			break;
			

		case STATE_ESCAPING:
			if (moves == MAX_MOVES)
			{
				GUI_Rect(72 + ((MAX_MOVES - 1) * MOVES_MUL), 130, sprite_w * 6, sprite_h * 6, White);
				game.happiness = 0;
				game.satiety = 0;

				// Stop Timers
				LPC_TIM0->TCR = 0;
				LPC_TIM1->TCR = 0;

				// Change Menu
				GUI_HLine(270, 50, BKG_Color_2);
				GUI_HLine(310, 10, Red);
				GUI_Text((MAX_X/2 - (5*8)/2), 282, (uint8_t *)"Reset", Black, BKG_Color_2);
				GUI_Text((MAX_X/2 - (13*8)/2), MAX_Y/2, (uint8_t *)"He escaped :(", Black, White);
				game.status = STATE_ESCAPED;
			}
			else
			{
				if (moves > 0)
				{
					GUI_Rect(72 + ((moves - 1) * MOVES_MUL), 130, sprite_w * 6, sprite_h * 6, White);
				}

				if (moves % 2)
				{
					GUI_Draw(anim_escaping_1_bin, 72 + (moves * MOVES_MUL), 130, sprite_w, sprite_h, 6);
				}
				else
				{
					GUI_Draw(anim_escaping_2_bin, 72 + (moves * MOVES_MUL), 130, sprite_w, sprite_h, 6);
				}
				moves++;
			}
			break;
		
		default:
			break;
		}

		// Clear IR Flag
		LPC_TIM0->IR = 1;

		// Re-enables RIT
		enable_RIT();
	}
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void)
{
	uint8_t i = 0;

	// Checks for MR0 Interrupt
	if (LPC_TIM1->IR & 1)
	{
		if (game.status == STATE_IDLE)
		{
			// Updates Stats
			game.happiness--;
			game.satiety--;

			if (game.happiness == 0 || game.satiety == 0)
			{
				// Game Over
				game.status = STATE_ESCAPING;
				playSong(se_escape, 500);

				// Clears all Bars
				for (i = 0; i < MAX_STATS; i++)
				{
					GUI_Rect(5 + (19 * i), 45, 14, 20, Stat_BKG_Color);
					GUI_Rect(MAX_X - 19 - (19 * i), 45, 14, 20, Stat_BKG_Color);
				}
			}

			// Clears one Bar
			GUI_Rect(5 + (19 * game.happiness), 45, 14, 20, Stat_BKG_Color);
			GUI_Rect(MAX_X - 19 - (19 * game.satiety), 45, 14, 20, Stat_BKG_Color);
		}

		// Clear IR Flag
		LPC_TIM1->IR = 1;
	}
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER2_IRQHandler(void)
{
	static uint8_t count = 0;
	
	// Square Wave	
	if (count)
	{
		// Writes on DAC, directly connected to the Loudspeaker
		LPC_DAC->DACR = game.currentSong.volume << 6;
	}
	else 
	{
		LPC_DAC->DACR = 0;
	}
	count = !count;
	
	LPC_TIM2->IR = 1;
}


/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER3_IRQHandler(void)
{
	if (game.currentSong.notesArray)
	{
		if (game.currentSong.notesArray[game.currentSong.currentNote] == NOTE_STOP)
		{
			// Last Note
			NVIC_DisableIRQ(TIMER2_IRQn);
			disable_timer(3);
			disable_timer(2);
			reset_timer(2);
			reset_timer(3);
			NVIC_EnableIRQ(TIMER2_IRQn);
		}
		else
		{
			// Plays Note
			playNote(game.currentSong.notesArray[game.currentSong.currentNote]);
			// Changes Note
			game.currentSong.currentNote++;
		}
	}
	LPC_TIM3->IR = 1;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
