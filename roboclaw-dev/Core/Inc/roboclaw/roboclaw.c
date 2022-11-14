#include "roboclaw.h"

/************************************** PRIVATE FUNCTIONS ************************************************/
static void crc_clear(roboclaw_t* roboClawInst)
{
    roboClawInst->crc = 0;
}

static void crc_update (roboclaw_t* roboClawInst, uint8_t data)
{
    uint8_t i;
    roboClawInst->crc = roboClawInst->crc ^ ((uint16_t) data << 8);
    for(i=0; i<8; i++) 
    {
        if(roboClawInst->crc & 0x8000)
            roboClawInst->crc = (roboClawInst->crc << 1) ^ 0x1021;
        else 
            roboClawInst->crc <<= 1;
    }
}

static uint16_t crc_get(roboclaw_t* roboClawInst) 
{
    return roboClawInst->crc;
}

static HAL_StatusTypeDef write(roboclaw_t* roboClawInst, uint8_t byte)
{
    return HAL_UART_Transmit(roboClawInst->hserial, (uint8_t*) &byte, 1, HAL_MAX_DELAY);
}

static uint8_t read(roboclaw_t *roboClawInst)
{
    HAL_UART_Receive(roboClawInst->hserial, roboClawInst->rdata, 1, HAL_MAX_DELAY);
    return roboClawInst->rdata;
}

static _Bool write_n (roboclaw_t* roboClawInst, uint8_t cnt, ...)
{
    uint8_t trys = MAXRETRY;
    HAL_StatusTypeDef status;
    do{
		crc_clear(roboClawInst);

		va_list marker;
		va_start( marker, cnt );     
		for(uint8_t index=0;index<cnt;index++){
			uint8_t data = va_arg(marker, int);
			crc_update(roboClawInst, data);
			write(roboClawInst, data);
		}
		va_end( marker );              

		uint16_t crc = crc_get(roboClawInst);
		write(roboClawInst, crc>>8);
		write(roboClawInst, crc);

		if(read(roboClawInst) == 0xFF)
			return 1;

	} while(trys--);

	return 0;
}

static HAL_StatusTypeDef read_n (roboclaw_t* roboClawInst, uint8_t* data, int len) 
{
    return HAL_UART_Receive(roboClawInst->hserial, data, len, HAL_MAX_DELAY);
}

static uint32_t Read4_1(roboclaw_t *roboClawInst, uint8_t address, uint8_t cmd, uint8_t* status, _Bool *valid)
{
    uint32_t value = 0;
    uint8_t  trys = MAXRETRY;
    int16_t  data;

    if(valid) *valid = 0; 

    do {
        crc_clear(roboClawInst);
        write(roboClawInst, address);
        crc_update(roboClawInst, address);
        write(roboClawInst, cmd);
        crc_update(roboClawInst, cmd);

        data = read(roboClawInst);
        crc_update(roboClawInst, data);
        value = (uint32_t) data << 24;

        if(data != -1) {
            data = read(roboClawInst);
            crc_update(roboClawInst, data);
            value |= (uint32_t) data<<16;
        }

        if(data!=-1){
			data = read(roboClawInst);
			crc_update(roboClawInst, data);
			value|=(uint32_t)data<<8;
		}

		if(data!=-1){
			data = read(roboClawInst);
			crc_update(roboClawInst, data);
			value|=(uint32_t)data;
		}
	
		if(data!=-1){
			data = read(roboClawInst);
			crc_update(roboClawInst, data);
			if(status)
				*status = data;
		}
				
		if(data!=-1){
			uint16_t ccrc;
			data = read(roboClawInst);
			if(data != -1){
				ccrc = data << 8;
				data = read(roboClawInst);
				if(data != -1){
					ccrc |= data;
					if(crc_get(roboClawInst) == ccrc){
						if(valid)
							*valid = 1;
						return value;
					}
				}
			}
		}
	} while(trys--);

	return 0;
}


static uint32_t Read4(roboclaw_t* roboClawInst, uint8_t address, uint8_t cmd, _Bool *valid)
{
    uint8_t crc;
	
	if(valid) *valid = 0;
	
	uint32_t value = 0;
	uint8_t  trys  = MAXRETRY;
	int16_t  data;
	do{

		crc_clear(roboClawInst);
		write(roboClawInst, address);;
		crc_update(roboClawInst, address);
		write(roboClawInst, address);;
		crc_update(roboClawInst, cmd);

		data = read(roboClawInst);
		crc_update(roboClawInst, data);
		value=(uint32_t)data<<24;

		if(data!=-1){
			data = read(roboClawInst);
			crc_update(roboClawInst, data);
			value|=(uint32_t)data<<16;
		}
		
		if(data!=-1){
			data = read(roboClawInst);
			crc_update(roboClawInst, data);
			value|=(uint32_t)data<<8;
		}

		if(data!=-1){
			data = read(roboClawInst);
			crc_update(roboClawInst, data);
			value|=(uint32_t)data;
		}
		
		if(data!=-1){
			uint16_t ccrc;
			data = read(roboClawInst);
			if(data!=-1){
				ccrc = data << 8;
				data = read(roboClawInst);
				if(data!=-1){
					ccrc |= data;
					if(crc_get(roboClawInst) == ccrc){
						if(valid) *valid = 1;
							
						return value;
					}
				}
			}
		}
	} while(trys--);
	
	return 0;
}

static uint16_t Read2(roboclaw_t* roboClawInst, uint8_t address, uint8_t cmd, _Bool *valid)
{
    uint8_t crc;

	if(valid)
		*valid = 0;
	
	uint16_t value=0;
	uint8_t trys=MAXRETRY;
	int16_t data;
	do{
		crc_clear(roboClawInst);
		write(roboClawInst, address);
		crc_update(roboClawInst, address);
		write(roboClawInst, cmd);
		crc_update(roboClawInst, cmd);
	
		data = read(roboClawInst);
		crc_update(roboClawInst, data);
		value=(uint16_t)data<<8;
		
		if(data!=-1){
			data = read(roboClawInst);
			crc_update(roboClawInst, data);
			value|=(uint16_t)data;
		}
		
		if(data!=-1){
			uint16_t ccrc;
			data = read(roboClawInst);
			if(data!=-1){
				ccrc = data << 8;
				data = read(roboClawInst);
				if(data!=-1){
					ccrc |= data;
					if(crc_get(roboClawInst)==ccrc){
						if(valid)
							*valid = 1;
						return value;
					}
				}
			}
		}
	}while(trys--);
		
	return 0;
}

static uint8_t Read1(roboclaw_t* roboClawInst, uint8_t address, uint8_t cmd, _Bool *valid)
{
    uint8_t crc;

	if(valid)
		*valid = 0;
	
	uint8_t value=0;
	uint8_t trys=MAXRETRY;
	int16_t data;
	do {
		crc_clear(roboClawInst);
		write(roboClawInst, address);
		crc_update(roboClawInst, address);
		write(roboClawInst, cmd);
		crc_update(roboClawInst, cmd);
	
		data = read(roboClawInst);
		crc_update(roboClawInst, data);
		value=data;

		if(data!=-1){
			uint16_t ccrc;
			data = read(roboClawInst);
			if(data!=-1){
				ccrc = data << 8;
				data = read(roboClawInst);
				if(data!=-1){
					ccrc |= data;
					if(crc_get(roboClawInst)==ccrc){
						if(valid)
							*valid = 1;
						return value;
					}
				}
			}
		}
	} while(trys--);
	
	return 0;
}

/**************************************** PUBLIC FUNCTIONS ************************************************/

HAL_StatusTypeDef roboclaw_begin(roboclaw_t* roboClawInst, UART_HandleTypeDef* huart, long speed) 
{
    roboClawInst->hserial = huart;
    roboClawInst->hserial->Init.BaudRate = speed;
    if (HAL_UART_Init(roboClawInst->hserial) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef roboclaw_ForwardM1(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed) {
    return write_n(3, address, M1FORWARD, speed);
}

HAL_StatusTypeDef roboclaw_ForwardM2(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed) {
	return write_n(3, address, M2FORWARD, speed);
}

HAL_StatusTypeDef roboclaw_BackwardM1(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed) {
	return write_n(3, address, M1BACKWARD, speed);
}

HAL_StatusTypeDef roboclaw_BackwardM2(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed) {
	return write_n(3, address, M2BACKWARD, speed);
}

HAL_StatusTypeDef roboclaw_ForwardBackwardM1(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed){
	return write_n(3,address,M17BIT,speed);
}

HAL_StatusTypeDef roboclaw_ForwardBackwardM2(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed){
	return write_n(3,address,M27BIT,speed);
}