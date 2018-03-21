#include "LIS3DH.h"

LIS3DH_InitStruct* ConfigStruct_p;

HAL_StatusTypeDef LIS3DH_Init(LIS3DH_InitStruct * InitStruct_p){
	HAL_StatusTypeDef s;

	ConfigStruct_p = InitStruct_p;
	uint8_t WHO_I_AM;
	if(s = REG_ReadAbyte(ADDR_WHO_AM_I_R, &WHO_I_AM)){
		return s; 
	}
	if(WHO_I_AM != WHO_AM_I){
		s = HAL_ERROR;
		return s;
	}

	uint8_t ODR_DATA = ((ConfigStruct_p->OutputDataRate << 4) | LSB_CTRL_REG4);
	if(s = REG_WriteAbyte(ADDR_CTRL_REG4_RW, &ODR_DATA)){
		return s;
	}

	uint8_t FSS_DATA = ConfigStruct_p->FullScaleSelection << 3;
	if(s = REG_WriteAbyte(ADDR_CTRL_REG5_RW, &FSS_DATA)){
		return s;
	}

	int temp = ConfigStruct_p->FullScaleSelection;
	if(temp >> 2){
		ConfigStruct_p->scale = 16;
	}else{
		ConfigStruct_p->scale = 2*(temp+1.0);
	}

	s = HAL_OK;
	return s;
}

HAL_StatusTypeDef LIS3DH_Get_Acc(LIS3DH_AccDataStruct* AccData_p){
	HAL_StatusTypeDef s;

	uint8_t DATA_temp[7];

	if(s = REG_Readibytes(ADDR_STATUS_R, DATA_temp, 7)){
		return s;
	}

	DATA_temp[0] = DATA_temp[0] & 0x0F;
	if(DATA_temp[0] != 0x0F){
		s = HAL_BUSY;
		return s;
	}

	for(int i=0; i<3; i++){
		uint16_t temp = (DATA_temp[2*i+2] << 8) | DATA_temp[2*i+1];
       	if(temp >> 15){
			temp = temp - 1;
			temp = ~temp;
			AccData_p->acc[i] = (-1) * ConfigStruct_p->scale * (float)temp / (float)32767; 
		}else{
			AccData_p->acc[i] = ConfigStruct_p->scale * (float)temp / (float)32767;
		}
	}

	s = HAL_OK;
	return s;
}

HAL_StatusTypeDef REG_ReadAbyte(uint8_t REG_ADDR, uint8_t* BUF_DATA_p){
	uint8_t ADDR_temp[2], DATA_temp[2];
	ADDR_temp[0] = REG_ADDR | (uint8_t)0x80;

	HAL_GPIO_WritePin(ConfigStruct_p->SPI_CS_Port, ConfigStruct_p->SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef s = HAL_SPI_TransmitReceive(ConfigStruct_p->hspi_p, ADDR_temp, DATA_temp, 2, SPI_TIMEOUT);
	HAL_GPIO_WritePin(ConfigStruct_p->SPI_CS_Port, ConfigStruct_p->SPI_CS_Pin, GPIO_PIN_SET);

	BUF_DATA_p[0] = DATA_temp[1];
	return s;
}

HAL_StatusTypeDef REG_Readibytes(uint8_t REG_ADDR, uint8_t * BUF_DATA_p, int length){
	uint8_t ADDR_temp[9], DATA_temp[9];
	ADDR_temp[0] = REG_ADDR | (uint8_t)0x80;

	HAL_GPIO_WritePin(ConfigStruct_p->SPI_CS_Port, ConfigStruct_p->SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef s = HAL_SPI_TransmitReceive(ConfigStruct_p->hspi_p, ADDR_temp, DATA_temp, length+1, SPI_TIMEOUT);
	HAL_GPIO_WritePin(ConfigStruct_p->SPI_CS_Port, ConfigStruct_p->SPI_CS_Pin, GPIO_PIN_SET);

	for(int i=0; i<length; i++)
	BUF_DATA_p[i] = DATA_temp[i+1];

	return s;
}

HAL_StatusTypeDef REG_WriteAbyte(uint8_t REG_ADDR, uint8_t* BUF_DATA_p){
	uint8_t BUF_trans[2];
	BUF_trans[0] = REG_ADDR;
	BUF_trans[1] = BUF_DATA_p[0];

	HAL_GPIO_WritePin(ConfigStruct_p->SPI_CS_Port, ConfigStruct_p->SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef s = HAL_SPI_Transmit(ConfigStruct_p->hspi_p, BUF_trans, 2, SPI_TIMEOUT);
	HAL_GPIO_WritePin(ConfigStruct_p->SPI_CS_Port, ConfigStruct_p->SPI_CS_Pin, GPIO_PIN_SET);
	
	return s;
}

