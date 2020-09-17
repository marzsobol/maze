
#ifndef MPU_H__
#define MPU_H__

// includes
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "MPU_Register_Map.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery.h"

#include "../Components/stmpe811/stmpe811.h" 
// Congiguration structures and enums



#define MPU_MPU_BASE_NUM    		0x4000
#define MPU_BAD_PARAMETER       	(MPU_MPU_BASE_NUM + 0) // An invalid paramameter has been passed to function.
#define MPU_I2C_ADDRESS					0x68<<1 //to write, read register as device address,for ad0 to low 
#define MPU_I2C_ADDRESS_NS			0x68 //not shifted 



extern I2C_HandleTypeDef hi2c3;
//enum for result
typedef enum MPU6050_Result {
	MPU6050_Result_Ok = 0x00,          /*!< Everything OK */
	MPU6050_Result_Error,              /*!< Unknown error */
	MPU6050_Result_DeviceNotConnected, /*!< There is no device with valid slave address */
	MPU6050_Result_DeviceInvalid       /*!< Connected device with address is not MPU9150 */
} MPU6050_Result;

#define MPU6050_DataRate_8KHz       0   /*!< Sample rate set to 8 kHz */
#define MPU6050_DataRate_4KHz       1   /*!< Sample rate set to 4 kHz */
#define MPU6050_DataRate_2KHz       3   /*!< Sample rate set to 2 kHz */
#define MPU6050_DataRate_1KHz       7   /*!< Sample rate set to 1 kHz */
#define MPU6050_DataRate_500Hz      15  /*!< Sample rate set to 500 Hz */
#define MPU6050_DataRate_250Hz      31  /*!< Sample rate set to 250 Hz */
#define MPU6050_DataRate_125Hz      63  /*!< Sample rate set to 125 Hz */
#define MPU6050_DataRate_100Hz      79  /*!< Sample rate set to 100 Hz */

typedef enum MPU6050_Accelerometer {
	MPU6050_Accelerometer_2G = 0x00, /*!< Range is +- 2G */
	MPU6050_Accelerometer_4G = 0x01, /*!< Range is +- 4G */
	MPU6050_Accelerometer_8G = 0x02, /*!< Range is +- 8G */
	MPU6050_Accelerometer_16G = 0x03 /*!< Range is +- 16G */
} MPU6050_Accelerometer;

typedef enum MPU6050_Gyroscope {
	MPU6050_Gyroscope_250s = 0x00,  /*!< Range is +- 250 degrees/s */
	MPU6050_Gyroscope_500s = 0x01,  /*!< Range is +- 500 degrees/s */
	MPU6050_Gyroscope_1000s = 0x02, /*!< Range is +- 1000 degrees/s */
	MPU6050_Gyroscope_2000s = 0x03  /*!< Range is +- 2000 degrees/s */
} MPU6050_Gyroscope;

typedef struct MPU6050_ReadAxisAccelerometr{
	int16_t MPU6050_Accelerometr_X;
	int16_t MPU6050_Accelerometr_Y;
	int16_t MPU6050_Accelerometr_Z;
	uint16_t Multi;
}MPU6050_ReadAxisAccelerometr;


#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)


//MPU
MPU6050_Result MPUInit (I2C_HandleTypeDef* MPU_I2C, MPU6050_Accelerometer accelerometr_sensitivity,MPU6050_Gyroscope gyroscope_sensitivity,MPU6050_ReadAxisAccelerometr* Axis);
MPU6050_Result MPU6050_SetDataRate(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address, uint8_t rate);
MPU6050_Result MPU6050_SetAccelerometer(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,MPU6050_Accelerometer accelerometr_sensitivity,MPU6050_ReadAxisAccelerometr* Axis);
MPU6050_Result MPU6050_SetGyroscope(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,MPU6050_Gyroscope gyroscope_sensitivity);
MPU6050_Result MPU6050_ReadAcelerometr(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,MPU6050_ReadAxisAccelerometr* Axis);
MPU6050_Result MPU6050_ReadTemperature(I2C_HandleTypeDef* MPU_I2C,uint8_t device_address,uint16_t *temperature);
//I2C
HAL_StatusTypeDef ReadRegisterI2C(I2C_HandleTypeDef* handle,uint8_t device_address, uint8_t register_address, uint8_t* data);
HAL_StatusTypeDef WriteRegisterI2C(I2C_HandleTypeDef* handle, uint8_t device_address, uint8_t register_address, uint8_t data);
HAL_StatusTypeDef ReadMultiI2C(I2C_HandleTypeDef* handle,uint8_t device_address, uint16_t register_address, uint8_t* data, uint16_t count);


#endif
