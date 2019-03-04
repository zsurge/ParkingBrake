/****************************************************************************
* File:			OUTPUT.h													*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__OUTPUT_H
#define	__OUTPUT_H

#include "reg52.h"
#include "MOTOR.H"


/**************************************************************************** 
* 宏定义
****************************************************************************/
#define	OPUT_UP		1			//抬闸
#define	OPUT_DW		3			//落闸



#define	DZP_TIME	10			//道闸反馈信号时间

#define	DLYBRAKETIME	20			// 延迟刹车时间
#define	BRAKETIME		300			//刹车时间



#define	DZP_ON		0			//道闸反馈信号状态
#define	DZP_NA		1			//道闸反馈信号状态


#define	DZP_UP		{ DZP1=DZP_ON; DZP0= DZP_NA; }			//道闸抬闸反馈
#define	DZP_DW		{ DZP1=DZP_NA; DZP0= DZP_ON; }			//道闸落闸反馈
#define	DZP_CL		{ DZP1=DZP_NA; DZP0= DZP_NA; }			//道闸反馈清零



#define	KK_GRE		0			//继电器状态
#define	KK_RED		1			//继电器状态


/**************************************************************************** 
* 使用端口定义
****************************************************************************/
#define	PORT_KK				P1
#define	B_KK				5
sbit	KK					= PORT_KK ^ B_KK;

#define	PORT_DZP0			P3
#define	B_DZP0				6
sbit	DZP0				= PORT_DZP0 ^ B_DZP0;

#define	PORT_DZP1			P3
#define	B_DZP1				7
sbit	DZP1				= PORT_DZP1 ^ B_DZP1;

#define	PORT_RG				P1
#define	B_RG				0
sbit	RG					= PORT_RG ^ B_RG;






/**************************************************************************** 
* 变量定义/声明
****************************************************************************/
extern volatile	u8 data DZPnum;
extern volatile	u8 data DlyBrakeNum;
extern volatile	u16 data BrakeNum;
extern volatile	u8 data BrakeSta;







/**************************************************************************** 
* 函数原型
****************************************************************************/
void intOut();
void DZP_clk();
void BrakeClk();
void OPutTask();
void BrakeClr();







#endif


