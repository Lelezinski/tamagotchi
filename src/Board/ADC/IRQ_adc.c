/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../../utils.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

extern volatile Game game;
extern const uint8_t icon_w;
extern const uint8_t icon_h;
extern const uint8_t icon_audio_on[];
extern const uint8_t icon_audio_off[];

unsigned short AD_current;
unsigned short AD_last = 0xFF;

void ADC_IRQHandler(void)
{
	AD_current = ((LPC_ADC->ADGDR >> 4) & 0xFFF); /* Read Conversion Result             */
	if (AD_current != AD_last)
	{
		// Sets Volume [0 - MAX_VOLUME]
		if (AD_current < 200)
		{
			// Audio ON to OFF
			AD_current = 0;
			GUI_Draw(icon_audio_off, 2, 90, icon_w, icon_h, 1);
		}
		else if (AD_last == 0)
		{
			// Audio OFF to ON
			GUI_Draw(icon_audio_on, 2, 90, icon_w, icon_h, 1);
		}
		game.currentSong.volume = AD_current * MAX_VOLUME / 0xFFF;
		AD_last = AD_current;
	}
}
