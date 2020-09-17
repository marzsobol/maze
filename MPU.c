#include "MPU.h"

#define I2C_TIMEOUT_VALUE              1000
I2C_HandleTypeDef I2CMPU;

MPU6050_Result MPUInit (I2C_HandleTypeDef* MPU_I2C, MPU6050_Accelerometer accelerometr_sensitivity,MPU6050_Gyroscope gyroscope_sensitivity,MPU6050_ReadAxisAccelerometr* Axis){
	uint8_t temp;
	//LCD - test
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	char tempchar[20];
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);

	if (HAL_I2C_IsDeviceReady(MPU_I2C,MPU_I2C_ADDRESS, 1,5)!= HAL_OK){
		BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"not Ready");
		return MPU6050_Result_DeviceNotConnected;
	}
	else{
	BSP_LCD_DisplayStringAtLine(1, (uint8_t*)"Ready");}
		
	/* Check who am I */
	ReadRegisterI2C(&hi2c3, MPU_I2C_ADDRESS, MPU_REG_WHO_AM_I, &temp);
	
	if (temp != MPU_I2C_ADDRESS_NS) {
		/* Return error */
		//BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"InCorrect");
		return MPU6050_Result_Error;
		
	}
	
		//BSP_LCD_DisplayStringAtLine(2, (uint8_t*)"Correct");
	
	/* Wakeup MPU6050 */
	WriteRegisterI2C(MPU_I2C, MPU_I2C_ADDRESS, MPU_REG_PWR_MGMT_1, 0x00);
	//ReadRegisterI2C(MPU_I2C, MPU_I2C_ADDRESS, MPU_REG_CONFIG, &temp);
	WriteRegisterI2C(MPU_I2C, MPU_I2C_ADDRESS, MPU_REG_CONFIG, 0x04);
	/* Set sample rate to 1kHz */
	MPU6050_SetDataRate(MPU_I2C,MPU_I2C_ADDRESS, MPU6050_DataRate_1KHz);
	
	/* Config accelerometer */
	MPU6050_SetAccelerometer(MPU_I2C,MPU_I2C_ADDRESS, accelerometr_sensitivity,Axis);
	/* Config gyroscope */
	MPU6050_SetGyroscope(MPU_I2C,MPU_I2C_ADDRESS, gyroscope_sensitivity);
	
	//WriteRegisterI2C(MPU_I2C, MPU_I2C_ADDRESS, MPU_REG_SIGNAL_PATH_RESET, 0x07);
		
	/* Return OK */
	return MPU6050_Result_Ok;
}
		
MPU6050_Result MPU6050_SetDataRate(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address, uint8_t rate){
	if(WriteRegisterI2C(MPU_I2C, device_address, MPU_REG_SMPLRT_DIV, rate) != MPU6050_Result_Ok){
		return MPU6050_Result_Error;
	}
	return MPU6050_Result_Ok;
}

MPU6050_Result MPU6050_SetAccelerometer(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,MPU6050_Accelerometer accelerometr_sensitivity,MPU6050_ReadAxisAccelerometr* Axis){
	uint8_t conf;
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_CONFIG,&conf);
	conf = (conf & 0xE7) | (uint8_t)accelerometr_sensitivity<<3;
	WriteRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_CONFIG,conf);
	switch (accelerometr_sensitivity){
		case 0:
			Axis->Multi = MPU6050_ACCE_SENS_2;
			break;
		case 1:
			Axis->Multi = MPU6050_ACCE_SENS_4;
			break;
		case 2:
			Axis->Multi = MPU6050_ACCE_SENS_8;
			break;
		case 3:
			Axis->Multi = MPU6050_ACCE_SENS_16;
			break;
	}
	return MPU6050_Result_Ok;
}
MPU6050_Result MPU6050_SetGyroscope(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,MPU6050_Gyroscope gyroscope_sensitivity){
	uint8_t conf;
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_GYRO_CONFIG,&conf);
	conf = (conf & 0xE7) | (uint8_t)gyroscope_sensitivity<<3;
	WriteRegisterI2C(MPU_I2C, device_address, MPU_REG_GYRO_CONFIG,conf);
	
	return MPU6050_Result_Ok;
}


MPU6050_Result MPU6050_ReadAcelerometr(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,MPU6050_ReadAxisAccelerometr* Axis){

	uint16_t time_start = HAL_GetTick();
	uint8_t MSB=0, LSB =0;
	uint16_t combined=0;
	uint8_t tempchar[20];
	
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_XOUT_H, &MSB);
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_XOUT_L, &LSB);
	
	Axis->MPU6050_Accelerometr_X=(MSB << 8 ) | (LSB & 0xff);
	Axis->MPU6050_Accelerometr_X =Axis->MPU6050_Accelerometr_X /100;
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_YOUT_H, &MSB);
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_YOUT_L, &LSB);
		
	Axis->MPU6050_Accelerometr_Y=(MSB << 8 ) | (LSB & 0xff);
	Axis->MPU6050_Accelerometr_Y/=100;
	
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_ZOUT_H, &MSB);
	ReadRegisterI2C(MPU_I2C, device_address, MPU_REG_ACCEL_ZOUT_L, &LSB);
		
	Axis->MPU6050_Accelerometr_Z=(MSB << 8 ) | (LSB & 0xff);
	Axis->MPU6050_Accelerometr_Z/=100;
		
//	sprintf(tempchar,"%d",Axis->MPU6050_Accelerometr_X);
//	BSP_LCD_DisplayStringAtLine(4, (uint8_t*)tempchar);	
//	
//	sprintf(tempchar,"%d",Axis->MPU6050_Accelerometr_Y);
//	BSP_LCD_DisplayStringAtLine(5, (uint8_t*)tempchar);
//	
//	sprintf(tempchar,"%d",Axis->MPU6050_Accelerometr_Z);
//	BSP_LCD_DisplayStringAtLine(6, (uint8_t*)tempchar);
	
	//HAL_Delay(500);

	return MPU6050_Result_Ok;
}

MPU6050_Result MPU6050_ReadTemperature(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,uint16_t *temperature){
	uint8_t data[2];
	int16_t temp;
	
	/* Read temperature */
	ReadMultiI2C(MPU_I2C, device_address, MPU_REG_TEMP_OUT_H, data, 2);
	
	/* Format temperature */
	temp = (data[0] << 8 | data[1]);
	temp = (float)((int16_t)temp / (float)340.0 + (float)36.53);
	temperature = &temp;
	/* Return OK */
	return MPU6050_Result_Ok;
}


HAL_StatusTypeDef ReadRegisterI2C(I2C_HandleTypeDef* handle,uint8_t device_address, uint8_t register_address, uint8_t* data){
	/* Send address */
	if (HAL_I2C_Master_Transmit(handle, (uint16_t)device_address, &register_address, 1, I2C_TIMEOUT_VALUE) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return HAL_ERROR;
	}
	
	/* Receive multiple byte */
	if (HAL_I2C_Master_Receive(handle, device_address, data, 1, I2C_TIMEOUT_VALUE) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return HAL_ERROR;
	}
	
	/* Return OK */
	return HAL_OK;
}

HAL_StatusTypeDef WriteRegisterI2C(I2C_HandleTypeDef* handle, uint8_t device_address, uint8_t register_address, uint8_t data){
	uint8_t send_data[2];
	send_data[0] = register_address;
	send_data[1] = data;
	if (HAL_I2C_Master_Transmit(handle, (uint16_t)device_address, (uint8_t *)send_data, 2, I2C_TIMEOUT_VALUE) != HAL_OK) {
		/* Check error */
		if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return HAL_ERROR;
	} 
	
	/* Return OK */
	return HAL_OK;
}
HAL_StatusTypeDef ReadMultiI2C(I2C_HandleTypeDef* handle,uint8_t device_address, uint16_t register_address, uint8_t* data, uint16_t count){
	if(HAL_I2C_Mem_Write(handle, device_address, register_address,register_address > 0xFF ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT,data, count,I2C_TIMEOUT_VALUE) != HAL_OK){
	/* Check error */
		if (HAL_I2C_GetError(handle) != HAL_I2C_ERROR_AF) {
			
		}
		
		/* Return error */
		return HAL_ERROR;
	} 
	
	/* Return OK */
	return HAL_OK;
}


