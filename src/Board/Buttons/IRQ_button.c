#include "lpc17xx.h"
#include "button.h"
#include "../RIT/RIT.h"

volatile char down_0 = 0;
volatile char down_1 = 0;
volatile char down_2 = 0;

/*----------------------------------------------------------------------------
	Button IRQs
 *----------------------------------------------------------------------------*/

/// @brief INT0 IRQ 
void EINT0_IRQHandler(void)
{
	// Disable other Interrupts while RIT is active
	NVIC_DisableIRQ(EINT0_IRQn);

	// Enables RIT
	reset_RIT();
	down_0 = 1;

	// GPIO Pin Selection
	LPC_PINCON->PINSEL4 &= ~(1 << 20);

	// Clears Interrupt
	LPC_SC->EXTINT &= (1 << 0);
}

/// @brief KEY1 IRQ 
void EINT1_IRQHandler(void)
{
	// Disable other Interrupts while RIT is active
	NVIC_DisableIRQ(EINT1_IRQn);

	// Enables RIT
	reset_RIT();
	down_1 = 1;

	// GPIO Pin Selection
	LPC_PINCON->PINSEL4 &= ~(1 << 22);

	// Clears Interrupt
	LPC_SC->EXTINT &= (1 << 1);
}

/// @brief KEY2 IRQ 
void EINT2_IRQHandler(void)
{
	// Disable other Interrupts while RIT is active
	NVIC_DisableIRQ(EINT2_IRQn);

	// Enables RIT
	reset_RIT();
	down_2 = 1;

	// GPIO Pin Selection
	LPC_PINCON->PINSEL4 &= ~(1 << 24);

	// Clears Interrupt
	LPC_SC->EXTINT &= (1 << 2);
}
