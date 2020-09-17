#include "LCD_maze.h"
//Nazwy funkcji duzymi literami bez podkreslnikow nazwy zmienny i nie XD malymi literami z podkreslnikiem 

//frame
static uint16_t x_pos_frame = 10;
static uint16_t y_pos_frame = 40;
static uint16_t width_frame = 220;
static uint16_t height_frame = 260;
//rng
extern RNG_HandleTypeDef hrng;
MPU6050_ReadAxisAccelerometr Axis;
MPU_InitialValues Values;
extern I2C_HandleTypeDef hi2c3;
BallLocation BallLoc;



//ball
static uint16_t radius[3] = {4,3,2};
//maze
static uint8_t maze_thickness[3] = {12,10,6};
static uint16_t size_hole[3] = {12,10,6}; //easy, medium, insane



void DrawMaze(difficulty_level level){
	
	//BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
	BSP_LCD_SetBackColor(BackgroundGame);
	BSP_LCD_Clear(BackgroundGame);
	RandomInit();
	SizeofLCD();
	
	if(MPUInit(&hi2c3,MPU6050_Accelerometer_16G,MPU6050_Gyroscope_500s,&Axis) == MPU6050_Result_Ok){
		BSP_LCD_ClearStringLine(1);
		BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"You can play.");
	}
	MPU6050_ReadAcelerometr(&hi2c3,MPU_I2C_ADDRESS,&Axis);
	
	DrawFrame(level);
	DrawInsideMaze(level);
	Values.X_Axis=Axis.MPU6050_Accelerometr_X;
	Values.Y_Axis=Axis.MPU6050_Accelerometr_Y;
	BallLoc.x = x_pos_frame+maze_thickness[level]+radius[level];
	BallLoc.y = y_pos_frame+maze_thickness[level]+radius[level];
	DrawBall(BallLoc.x, BallLoc.y, level);

		
	uint16_t x_temp = 10;
	uint16_t y_temp = 40;
	while(1){
	ReadNewValuesMaybeMoveOrNot(level);
	HAL_Delay(speed);
//		while(x_temp != 220){
//		if(BSP_LCD_ReadPixel(x_temp,y_temp)==LCD_COLOR_BLUE){
//		BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"BLUE");
//		
//	char Size[20]; //used to see the size of LCD
//	sprintf(Size,"%d x %d",x_temp, y_temp);
//	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
//	BSP_LCD_ClearStringLine(1);
//	BSP_LCD_DisplayStringAtLine(1, (uint8_t*)Size);
//			HAL_Delay(30);
//		}
//		x_temp++;
//		}
//		x_temp =10;
//		y_temp++;
//	if(y_temp == 320)
//		break;

	}
}


void RandomInit(){
	__HAL_RCC_RNG_CLK_ENABLE();
	__HAL_RCC_RNG_IS_CLK_ENABLED()
	
	HAL_RNG_MODULE_ENABLED;
	
	HAL_RNG_Init(&hrng);
	hrng.Instance = RNG;
}
void DrawInsideMaze(difficulty_level level){
	uint8_t quantity_loop = 1; 
	uint16_t x_pos_maze = x_pos_frame + maze_thickness[level]*2 ;
	uint16_t y_pos_maze = y_pos_frame + maze_thickness[level]*2;
	uint16_t width_maze = width_frame - maze_thickness[level]*4;
	uint16_t height_maze = height_frame - maze_thickness[level]*4;
	uint8_t maximum = width_frame/4/maze_thickness[level];
	//random number generator
	
	uint32_t random_number = 0;
	char random_number_string[10];
	uint32_t random_number_tab [5];
	
	
	while(quantity_loop < maximum){
		ThickenRect(x_pos_maze, y_pos_maze, width_maze, height_maze,maze_thickness[level]);
		random_number = RandomNumberGenerator(1,4);
		random_number_tab[quantity_loop-1] = random_number;
		switch (random_number){
			case 4:
				GenerateHole(width_maze,x_pos_maze, y_pos_maze, x_axis,level);
			case 3:
				GenerateHole(width_maze,x_pos_maze, y_pos_maze + height_maze - maze_thickness[level] + 1, x_axis,level);
			case 2:
				GenerateHole(height_maze,x_pos_maze, y_pos_maze, y_axis,level);
			case 1:
				GenerateHole(height_maze,x_pos_maze + width_maze - maze_thickness[level] +1, y_pos_maze, y_axis,level);
				break;
		}
		quantity_loop++;
		x_pos_maze+= maze_thickness[level]*2;
		y_pos_maze+= maze_thickness[level]*2;
		width_maze -= maze_thickness[level]*4;
		height_maze -= maze_thickness[level]*4;
		
	}
	sprintf(random_number_string,"%d,%d,%d,%d",random_number_tab[0],random_number_tab[1],random_number_tab[2],random_number_tab[3]);
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	BSP_LCD_ClearStringLine(2);
	BSP_LCD_DisplayStringAtLine(2, (uint8_t*)random_number_string);
}

uint32_t RandomNumberGenerator(uint32_t from, uint32_t to){
	uint32_t random_number = 0;
	
	HAL_RNG_GenerateRandomNumber(&hrng, &random_number);

	if(from > 0){
	random_number = abs(random_number)%(to) + from;
	return  random_number;
	}
	return random_number%(to+1);
}
void DrawFrame(difficulty_level level){
	BSP_LCD_Clear(BackgroundGame);
BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
ThickenRect(x_pos_frame, y_pos_frame, width_frame, height_frame, maze_thickness[level]);
}
void ThickenRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t quantity){
	
	for(int i = 0; i < quantity; i++){
		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
		BSP_LCD_DrawRect(x+i, y+i, width-2*i, height-2*i);
	}
}
void SizeofLCD(){
	BSP_LCD_SetFont(&Font12);
	uint32_t SizeX = BSP_LCD_GetXSize();
	uint32_t SizeY = BSP_LCD_GetYSize();
	char Size[20]; //used to see the size of LCD
	sprintf(Size,"%d x %d",SizeX, SizeY);
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	BSP_LCD_DisplayStringAtLine(1, (uint8_t*)Size);
	BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Cos tu jeszcze bedzie");
}


void GenerateHole(uint16_t line,uint16_t x, uint16_t y, move_axis axis, difficulty_level level){
	uint32_t random_number = 0;
	random_number = RandomNumberGenerator(maze_thickness[level], line - maze_thickness[level] - 2*size_hole[level]);
	BSP_LCD_SetTextColor(BackgroundGame);
	if(axis == x_axis){
		for(int j = 0; j < size_hole[level]; j++){
			for(int i = 0; i < maze_thickness[level]; i++){
				BSP_LCD_DrawPixel(x + random_number + j, y + i, BackgroundGame);
			}
			//x++;
		}
	}
	else{
		for(int j = 0; j < size_hole[level]; j++){
			for(int i = 0; i < maze_thickness[level]; i++){
				BSP_LCD_DrawPixel(x + i, y + random_number + j, BackgroundGame);
			}
		}
	}
	
}

void DrawBall(uint16_t x, uint16_t y,difficulty_level level){	
	BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
	BSP_LCD_FillCircle(x, y, radius[level]);	

}
void ClearBall(uint16_t x, uint16_t y,difficulty_level level){
	BSP_LCD_SetTextColor(BackgroundGame);
	BSP_LCD_FillCircle(x, y, radius[level]);	

	
}
void Moveball(move_axis move_ax,difficulty_level level,side leftorright){
	
	ClearBall(BallLoc.x,BallLoc.y,level);
	if(move_ax == x_axis){
		if(leftorright == right){
		BallLoc.x += 1;
	}
		else{
			BallLoc.x -= 1;
		}
	}
	if(move_ax == y_axis){
		if(leftorright == left){
		BallLoc.y -= 1;
	}
		else{
			BallLoc.y += 1;
		}
	}
	//HAL_Delay(500);
	DrawBall(BallLoc.x,BallLoc.y,level);
}
void ReadNewValuesMaybeMoveOrNot(difficulty_level level){
		
	MPU6050_ReadAcelerometr(&hi2c3,MPU_I2C_ADDRESS,&Axis);
	if(Axis.MPU6050_Accelerometr_X < Values.X_Axis-10) {
		if(Thereisnowall(x_axis,level,right)==true) {
		Moveball(x_axis,level,right);
		}
	}
	if(Axis.MPU6050_Accelerometr_X > Values.X_Axis+10){
		if(Thereisnowall(x_axis,level,left)==true){
			Moveball(x_axis,level,left);
		}
	}
	if(Axis.MPU6050_Accelerometr_Y < Values.Y_Axis-10){
		if(Thereisnowall(y_axis,level,left)==true){
			Moveball(y_axis,level,left);
		}
	}
		if(Axis.MPU6050_Accelerometr_Y > Values.Y_Axis+10){
			if(Thereisnowall(y_axis,level,right)==true){
		Moveball(y_axis,level,right);
			}
	}
}
  
bool Thereisnowall(move_axis move_ax,difficulty_level level,side leftorright){
	uint32_t X = 0;
	uint32_t Y = 0;
	char Size[10];
	
	if(move_ax == x_axis){
			if(leftorright == right){
				if(BSP_LCD_ReadPixel(BallLoc.x +radius[level]+1 ,BallLoc.y)==LCD_COLOR_BLUE ||\
					BSP_LCD_ReadPixel(BallLoc.x +radius[level]+1 ,BallLoc.y+radius[level])==LCD_COLOR_BLUE ||\
				BSP_LCD_ReadPixel(BallLoc.x +radius[level]+1 ,BallLoc.y-radius[level])==LCD_COLOR_BLUE){
					return false;
				}
			}
			else{
				if(BSP_LCD_ReadPixel(BallLoc.x -radius[level]-1 ,BallLoc.y)==LCD_COLOR_BLUE||\
					BSP_LCD_ReadPixel(BallLoc.x -radius[level]-1 ,BallLoc.y+radius[level])==LCD_COLOR_BLUE ||\
				BSP_LCD_ReadPixel(BallLoc.x -radius[level]-1 ,BallLoc.y-radius[level])==LCD_COLOR_BLUE){
					return false;
				}
			}
	}	
			
	if(move_ax == y_axis){
			if(leftorright == right){
				if(BSP_LCD_ReadPixel(BallLoc.x ,BallLoc.y+radius[level]+1)==LCD_COLOR_BLUE||\
					BSP_LCD_ReadPixel(BallLoc.x +radius[level] ,BallLoc.y+radius[level]+1)==LCD_COLOR_BLUE||\
				BSP_LCD_ReadPixel(BallLoc.x - radius[level] ,BallLoc.y+radius[level]+1)==LCD_COLOR_BLUE){
					return false;
				}
			}
		else{
			if(BSP_LCD_ReadPixel(BallLoc.x,BallLoc.y -radius[level]-1)==LCD_COLOR_BLUE||\
				BSP_LCD_ReadPixel(BallLoc.x +radius[level] ,BallLoc.y-radius[level]-1)==LCD_COLOR_BLUE||\
				BSP_LCD_ReadPixel(BallLoc.x - radius[level] ,BallLoc.y-radius[level]-1)==LCD_COLOR_BLUE){
				return false;
			}
		}
	}

	
	return true;
}







