#ifndef __START_NEW_GAME_H
#define __START_NEW_GAME_H

#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery_ts.h"
#include "LCD_maze.h"



void NewGame();
void Welcoming();
void Levels();
void DrawLevel(uint16_t x_pos, uint16_t y_pos, char* level);


#endif //__START_NEW_GAME_H