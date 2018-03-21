#ifndef LIS3DH__H_
#define LIS3DH__H_

#include "stm32f4xx_hal.h"

#define SPI_TIMEOUT				500 // written in milliseconds

#define ADDR_OUT_T_R			0x0C
#define ADDR_WHO_AM_I_R			0x0F
#define ADDR_CTRL_REG4_RW		0x20 // DATA FOR ODR
#define ADDR_CTRL_REG5_RW		0x24 // DATA FOR FSS
#define ADDR_STATUS_R			0x27

#define LSB_CTRL_REG4			0x07
#define WHO_AM_I				0x3FU

typedef enum{
	Power_down	= 0x00U,
	ODR_3Hz 	= 0x01U,
	ODR_6Hz 	= 0x02U,	
	ODR_12Hz	= 0x03U,
	ODR_25Hz	= 0x04U,
	ODR_50Hz	= 0x05U,
	ODR_100Hz	= 0x06U,
	ODR_400Hz	= 0x07U,
	ODR_800Hz	= 0x08U,
	ODR_1600Hz	= 0x09U 
}OutputDataRateType;

typedef enum {
	FSS_2g	= 0x0U,
	FSS_4g	= 0x1U,
	FSS_6g	= 0x2U,
	FSS_8g	= 0x3U,
	FSS_16g = 0x4U 
}FullScaleSelectionType;


typedef struct{
	OutputDataRateType		OutputDataRate;
	FullScaleSelectionType 	FullScaleSelection;

	float scale;
	
	SPI_HandleTypeDef* hspi_p;
	GPIO_TypeDef* SPI_CS_Port;
	uint16_t SPI_CS_Pin;
}LIS3DH_InitStruct;

typedef struct{
	float acc[3];
}LIS3DH_AccDataStruct;



HAL_StatusTypeDef LIS3DH_Init(LIS3DH_InitStruct* InitStruct_p);
HAL_StatusTypeDef LIS3DH_Get_Acc(LIS3DH_AccDataStruct* );
HAL_StatusTypeDef REG_ReadAbyte(uint8_t REG_ADDR, uint8_t* BUF_DATA_p);
HAL_StatusTypeDef REG_Readibytes(uint8_t REG_ADDR, uint8_t* BUF_DATA_p, int length);
HAL_StatusTypeDef REG_WriteAbyte(uint8_t REG_ADDR, uint8_t* BUF_DATA_p);


#endif
