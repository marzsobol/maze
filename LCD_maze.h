#ifndef __LCD_MAZE_H
#define __LCD_MAZE_H

#include "stdint.h"
#include "stdlib.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rng.h"
#include "math.h"
#include "MPU.h"
#include "stdbool.h"
//#include "Pictures.h"

#define BackgroundGame LCD_COLOR_LIGHTGRAY
#define speed 15


typedef enum move_axis{
x_axis = 1,
y_axis = 2}move_axis;

typedef enum side{
	left,
	right
}side;

typedef enum difficulty_level{
	easy = 0,
	medium = 1,
	insane = 2
} difficulty_level;


//Initial MPU values
typedef struct MPU_InitialValues{
	int16_t X_Axis;
	int16_t Y_Axis;
}MPU_InitialValues;

typedef struct BallLocation{
	uint16_t x;
	uint16_t y;
}BallLocation;

//speed



//drawing
void DrawMaze(difficulty_level level);
void DrawFrame(difficulty_level level);
void DrawBall(uint16_t x, uint16_t y,difficulty_level level);
void DrawInsideMaze();

void ThickenRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t quantity);
void GenerateHole(uint16_t line,uint16_t x, uint16_t y, move_axis axis,difficulty_level level);

	//rng
void RandomInit();
uint32_t RandomNumberGenerator(uint32_t from, uint32_t to);
//moveball
void Moveball(move_axis move_ax,difficulty_level level,side leftorright);
void ReadNewValuesMaybeMoveOrNot(difficulty_level level);
bool Thereisnowall(move_axis move_ax,difficulty_level level,side leftorright);

void SizeofLCD();
void Treasure();



#endif //__LCD_MAZE_H