/****************************************************************************
* File:			INPUT.h														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__INPUT_H
#define	__INPUT_H

#include "reg52.h"
#include "ERR.h"
#include "MOTOR.H"
#include "RUN.H"
#include "OUTPUT.H"
#include "AUTORUN.H"




/**************************************************************************** 
* 宏定义
****************************************************************************/

//*************防抖时间*******************//
#define HUP_ON_TIME					10			//手动抬闸信号有效防抖时间
#define VUP_ON_TIME					10			//视频抬闸信号有效防抖时间
#define HDW_ON_TIME					10			//手动落闸信号有效防抖时间

//地感信号有效防抖时间
#define DG_ON_TIME					20
#define DG_OFF_TIME					5			//地感信号无效防抖时间
#define FZ_OFF_TIME					20			//防砸信号无效防抖时间
#define POS_ON_TIME					2			//位置信号有效防抖时间
#define POS_OF_TIME					20			//位置信号无效防抖时间


//*************输入信号状态******************//
#define	STA_NA						0			//--无信号
#define	STA_ON						1			//--有信号
#define	STA_KEEP					2			//--有信号输入保持
#define	STA_KEEP1					3			//--有信号输入保持


//***************设备类型******************//
#define	MACHI_VICE					2			//副机
#define	MACHI_MASTER				1			//主机


//***************设备规格******************//
#define FENCE_4M3                        0
#define FENCE_4M2                        1
#define FENCE_4M1                        2
#define FENCE_4M0                        3
#define FENCE_3M8                        4
#define FENCE_3M5                        5
#define FENCE_3M0                        6




/**************************************************************************** 
* 使用端口定义
****************************************************************************/
#define	PORT_HUP			P3
#define	B_HUP				3
sbit	HUP 				= PORT_HUP ^ B_HUP;
			
#define	PORT_HDW			P2
#define	B_HDW				0
sbit	HDW					= PORT_HDW^ B_HDW;

#define	PORT_DG				P1
#define	B_DG				7
sbit	DG					= PORT_DG ^ B_DG;

#define	PORT_FZ				P1
#define	B_FZ				3
sbit	FZ					= PORT_FZ ^ B_FZ;

#define	PORT_HOR			P2
#define	B_HOR				2
sbit	HOR					= PORT_HOR ^ B_HOR;

#define	PORT_VER			P3
#define	B_VER				4
sbit	VER					= PORT_VER ^ B_VER;

#define	PORT_VUP			P1
#define	B_VUP				4
sbit	VUP					= PORT_VUP ^ B_VUP;

#define	PORT_JY				P2
#define	B_JY				1
sbit	JY					= PORT_JY ^ B_JY;

#define	PORT_BM1			P0
#define	B_BM1				3
sbit	BM1					= PORT_BM1 ^ B_BM1;

#define	PORT_BM2			P0
#define	B_BM2				4
sbit	BM2					= PORT_BM2 ^ B_BM2;

#define	PORT_BM3			P0
#define	B_BM3				5
sbit	BM3					= PORT_BM3 ^ B_BM3;

#define	PORT_BM4			P0
#define	B_BM4				6
sbit	BM4					= PORT_BM4 ^ B_BM4;




/**************************************************************************** 
* 变量定义/声明
****************************************************************************/
extern volatile u8  data HupNum;			//
extern volatile u8  data VupNum;			//
extern volatile u8  data HDwNum;			//
extern volatile u8  data DgNum_NoSignal;    //DG端无信号防抖计时
extern volatile u8  data DgNum_Signal;   	//DG端有信号防抖计时
extern volatile u8  data FzNum;				//FZ端防抖计时
extern volatile u8  data PosNum;			//位置检测计时

extern volatile u8 xdata HupSta;			//
extern volatile u8 xdata VupSta;			//
extern volatile u8 xdata HDwSta;			//
extern volatile u8 xdata DgSta;				//地感输入状态
extern volatile u8 xdata FzSta;				//外部防砸输入状态

extern volatile u8 xdata PosLin;			//位置检测顺序
extern volatile u8 xdata Even;				//事件
extern volatile u8 xdata EveNu;				//事件个数
extern volatile u8 xdata McuId;				//设备号

typedef struct MOTOR_LIMIT
{
    u8 Two_Spring_Max_Num;//第8次记录的最大值，若超出此值，说明弹簧弹力不足
    u8 Three_Spring_Offset_Sum;//3条弹簧偏移量和，超过此值，说明弹力不足
    u8 Two_Spring_Offset_Sum;//2条弹簧偏称量和，超过此值，说明弹力不足   
    u8 OverCnt;              //超过一个计算周期的次数，也就是每一次计算偏移量大于6的次数，超过此值，说明弹力不足
}xdata MOTOR_LIMIT_STRU;

extern volatile MOTOR_LIMIT_STRU gMotorLimit;  //判定当前是否需要抬闸







/**************************************************************************** 
* 函数原型
****************************************************************************/
void intInput();
void Event(u8 evn);
void DgStaClr();
void Dg_clk();
void Fz_clk();
void Pos_clk();
void Hup_clk();
void Hdw_clk();
void Vup_clk();
void BmRed();
void InputTask();













#endif


