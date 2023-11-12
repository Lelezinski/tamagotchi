#ifndef _UTILS_H
#define _UTILS_H

#include "lpc17xx.h"
//#include "Board/Leds/led.h"
//#include "Board/Buttons/button.h"
#include "Board/Timers/timer.h"
#include "Board/RIT/RIT.h"
#include "Board/GLCD/GLCD.h"
#include "Board/Joystick/joystick.h"
#include "Board/TouchPanel/TouchPanel.h"
#include "Board/ADC/adc.h"

/*----------------------------------------------------------------------------
  Utils
 *----------------------------------------------------------------------------*/

#define BKG_Color_1 	0xEC66
#define BKG_Color_2 	0xFE22
#define Hap_Color		Blue
#define Sat_Color		DarkRed
#define Stat_BKG_Color	Grey

#define MAX_STATS		6
#define MAX_MOVES		4
#define MAX_MOVES_CUDDLE 1
#define MAX_WAIT		3
#define MOVES_MUL		20
#define MAX_VOLUME		500

#define STATE_IDLE		0
#define STATE_ESCAPING	1
#define STATE_ESCAPED	2
#define STATE_EAT_MEAL	3
#define STATE_EAT_SNACK	4
#define STATE_CUDDLE	5

#define NOTE_PAUSE		0
#define NOTE_C4			2120
#define NOTE_D4			1890
#define NOTE_E4			1684
#define NOTE_F4			1592
#define NOTE_G4			1417
#define NOTE_A4			1263
#define NOTE_B4			1125
#define NOTE_STOP		1

void SysInit(void);
int toString(uint8_t *, int);
void toHHMMSS(uint8_t * buffer, uint16_t hrs, uint8_t min, uint8_t sec);
void playNote(uint32_t note);
void playSong(uint32_t * notes, uint32_t delay_ms);

/*----------------------------------------------------------------------------
	Game
 *----------------------------------------------------------------------------*/

typedef struct 
{
	uint16_t hrs;
	uint8_t min;
	uint8_t sec;
} Age;

typedef struct 
{
	uint32_t * notesArray;
	uint8_t currentNote;
	uint32_t volume;
} Song;

typedef struct 
{
	uint8_t animIndex;
	Age age;
	uint8_t happiness;
	uint8_t satiety;
	uint8_t status;
	Song currentSong;
} Game;

#endif
