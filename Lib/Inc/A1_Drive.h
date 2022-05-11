//
// Created by Trisoil on 2022/5/3.
//

#include "main.h"

#ifndef A1_DRIVE_A1_DRIVE_H
#define A1_DRIVE_A1_DRIVE_H

typedef struct{
		uint8_t start[2];
		uint8_t Motor_ID;
		uint8_t reserved_a;
		uint8_t mode;
		uint8_t ModifyBit;
		uint8_t ReadBit;
		uint8_t reserved_b;
		uint32_t Modify;
		uint16_t T;
		uint16_t W;
		uint32_t Pos;
		uint16_t kp;
		uint16_t kw;
		uint8_t LowHzMotorCmdIndex;
		uint8_t LowHzMotorCmdByte;
		uint32_t Res;
		uint32_t CRCdata;
}Send_Data;

extern void Mode_Control(int ID,int Mode);
extern void Usart1_TX_DMA_Init(void);

#endif //A1_DRIVE_A1_DRIVE_H
