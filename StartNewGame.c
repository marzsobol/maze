#include "StartNewGame.h"

TS_StateTypeDef Click;

void NewGame(){
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	Welcoming();
	Levels();
	DrawMaze(easy);
	
}

void Welcoming(){
	BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
	BSP_LCD_Clear(BackgroundGame);
	BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
	BSP_LCD_SetBackColor(BackgroundGame);
	
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DisplayStringAt(0,40,(uint8_t*)"Welcome",CENTER_MODE);
	BSP_LCD_DisplayStringAt(0,70,(uint8_t*)"Maze-maniac!",CENTER_MODE);
}
void Levels(){
	//rect params
	uint16_t first_rect = 120;
	uint16_t distance = 70;
	//drawing levels
	DrawLevel(20,first_rect,"EASY");
	DrawLevel(20,first_rect+distance,"MEDIUM");
	DrawLevel(20,first_rect+2*distance,"INSANE");
	BSP_TS_Init(240,320);
	//touching
	while(1){;
		difficulty_level level;
		BSP_TS_GetState(&Click);
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_SetFont(&Font12);
		if(Click.TouchDetected){
			BSP_LCD_ClearStringLine(1);
			if(Click.X >20 & Click.X < 220){
				if(Click.Y > first_rect & Click.Y < first_rect+50){
					BSP_LCD_ClearStringLine(1);	
					BSP_LCD_DisplayStringAtLine(1,(uint8_t*)"EASY MODE CHOSEN");
					level = easy;
				}
				if(Click.Y > first_rect+distance & Click.Y < first_rect+distance+50){
					BSP_LCD_ClearStringLine(1);	
					BSP_LCD_DisplayStringAtLine(1,(uint8_t*)"MEDIUM MODE CHOSEN");
					level = medium;
				}
				if(Click.Y > first_rect+2*distance & Click.Y < first_rect+2*distance+50){
					BSP_LCD_ClearStringLine(1);	
					BSP_LCD_DisplayStringAtLine(1,(uint8_t*)"INSANE MODE CHOSEN");
					level = insane;
				}
			}
			DrawMaze(level);
			BSP_TS_Init(0,0);
			
		}
	}
}

void DrawLevel(uint16_t x_pos,uint16_t y_pos, char* level){
	//easy mode
	BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
	BSP_LCD_FillRect(x_pos, y_pos, 200, 50);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
	BSP_LCD_DisplayStringAt(0,y_pos + 25,(uint8_t*)level, CENTER_MODE);
}




