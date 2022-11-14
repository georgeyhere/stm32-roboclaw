#ifndef ROBOCLAW_H
#define ROBOCLAW_H

#include "stm32f7xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>


#define MAXRETRY  2

typedef enum {
    M1FORWARD                     = 0,
    M1BACKWARD                    = 1,
    SETMINMB                      = 2,
    SETMAXMB                      = 3,
    M2FORWARD                     = 4,
    M2BACKWARD                    = 5,
    M17BIT                        = 6,
    M27BIT                        = 7,
    MIXEDFORWARD                  = 8,
    MIXEDBACKWARD                 = 9,
    MIXEDRIGHT                    = 10,
    MIXEDLEFT                     = 11,
    MIXEDFB                       = 12,
    MIXEDLR                       = 13,
    GETM1ENC                      = 16,
    GETM2ENC                      = 17,
    GETM1SPEED                    = 18,
    GETM2SPEED                    = 19,
    RESETENC                      = 20,
    GETVERSION                    = 21,
    SETM1ENCCOUNT                 = 22,
    SETM2ENCCOUNT                 = 23,
    GETMBATT                      = 24,
    GETLBATT                      = 25,
    SETMINLB                      = 26,
    SETMAXLB                      = 27,
    SETM1PID                      = 28,
    SETM2PID                      = 29,
    GETM1ISPEED                   = 30,
    GETM2ISPEED                   = 31,
    M1DUTY                        = 32,
    M2DUTY                        = 33,
    MIXEDDUTY                     = 34,
    M1SPEED                       = 35,
    M2SPEED                       = 36,
    MIXEDSPEED                    = 37,
    M1SPEEDACCEL                  = 38,
    M2SPEEDACCEL                  = 39,
    MIXEDSPEEDACCEL               = 40,
    M1SPEEDDIST                   = 41,
    M2SPEEDDIST                   = 42,
    MIXEDSPEEDDIST                = 43,
    M1SPEEDACCELDIST              = 44,
    M2SPEEDACCELDIST              = 45,
    MIXEDSPEEDACCELDIST           = 46,
    GETBUFFERS                    = 47,
    GETPWMS                       = 48,
    GETCURRENTS                   = 49,
    MIXEDSPEED2ACCEL              = 50,
    MIXEDSPEED2ACCELDIST          = 51,
    M1DUTYACCEL                   = 52,
    M2DUTYACCEL                   = 53,
    MIXEDDUTYACCEL                = 54,
    READM1PID                     = 55,
    READM2PID                     = 56,
    SETMAINVOLTAGES               = 57,
    SETLOGICVOLTAGES              = 58,
    GETMINMAXMAINVOLTAGES         = 59,
    GETMINMAXLOGICVOLTAGES        = 60,
    SETM1POSPID                   = 61,
    SETM2POSPID                   = 62,
    READM1POSPID                  = 63,
    READM2POSPID                  = 64,
    M1SPEEDACCELDECCELPOS         = 65,
    M2SPEEDACCELDECCELPOS         = 66,
    MIXEDSPEEDACCELDECCELPOS      = 67,
    SETM1DEFAULTACCEL             = 68,
    SETM2DEFAULTACCEL             = 69,
    SETPINFUNCTIONS               = 74,
    GETPINFUNCTIONS               = 75,
    SETDEADBAND	                  = 76,
    GETDEADBAND	                  = 77,
    GETENCODERS                   = 78,
    GETISPEEDS                    = 79,
    RESTOREDEFAULTS               = 80,
    GETTEMP                       = 82,
    GETTEMP2                      = 83,	//Only valid on some models
    GETERROR                      = 90,
    GETENCODERMODE                = 91,
    SETM1ENCODERMODE              = 92,
    SETM2ENCODERMODE              = 93,
    WRITENVM                      = 94,
    READNVM                       = 95,	//Reloads values from Flash into Ram
    SETCONFIG                     = 98,
    GETCONFIG                     = 99,
    SETM1MAXCURRENT               = 133,
    SETM2MAXCURRENT               = 134,
    GETM1MAXCURRENT               = 135,
    GETM2MAXCURRENT               = 136,
    SETPWMMODE                    = 148,
    GETPWMMODE                    = 149,
    FLAGBOOTLOADER                = 255
} roboclaw_cmd;	//Only available via USB communications

typedef enum 
{
    ROBOCLAW_OK      = 0x0U,
    ROBOCLAW_ERROR   = 0x01U,
    ROBOCLAW_BUSY    = 0x2U,
    ROBOCLAW_TIMEOUT = 0x3U
} roboclaw_status;

typedef struct {
    uint16_t            crc; 
    uint32_t            timeout;
    uint8_t             addr; 

    UART_HandleTypeDef* hserial;
    uint8_t             rdata;  
} roboclaw_t;


HAL_StatusTypeDef roboclaw_begin(roboclaw_t* roboClawInst, UART_HandleTypeDef* huart, long speed);
HAL_StatusTypeDef roboclaw_ForwardM1(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed);
HAL_StatusTypeDef roboclaw_ForwardM2(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed);
HAL_StatusTypeDef roboclaw_BackwardM1(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed);
HAL_StatusTypeDef roboclaw_BackwardM2(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed);
HAL_StatusTypeDef roboclaw_ForwardBackwardM1(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed);
HAL_StatusTypeDef roboclaw_ForwardBackwardM2(roboclaw_t* roboClawInst, uint8_t address, uint8_t speed);

#endif 
