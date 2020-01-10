/****************************************************************************
* File:			run.h														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__RUN_H
#define	__RUN_H

#include "reg52.h"
#include "clk.h"
#include "BUZZ.H"
#include "INPUT.H"
#include "MOTOR.H"
#include "AUTORUN.H"
#include "USART.h"
#include "ROMTEST.h"



/**************************************************************************** 
* 宏定义
****************************************************************************/
#define		ENABLE		1
#define		DISABLE		0

#define     MAIN_Fosc   11059200UL    //定义主时钟HZ  


//主副机通信周期
#define COM_TIME		20		


//执行的任务
#define TASK_NA		0		//无任务
#define TASK_UP		1		//抬闸
#define TASK_US		2		//抬闸缓冲
#define TASK_DW		3		//落闸
#define TASK_DS		4		//落闸缓冲



//事件对应的优先级(数字越大，优先级越高)
#define ITASK_INT			255	//	外部任务初始化
#define ITASK_STR			254	//	外部任务初始化
#define ITASK_NA			0	//	无任务
#define ITASK_DG_UP			8	//	地感抬闸
#define ITASK_FZ_UP			9	//	外部防砸抬闸
#define ITASK_DRAG_UP		10	//	遇阻抬闸
#define ITASK_AUTO_UP		11	//	内部控制抬闸
#define ITASK_COM_UP		12	//	通信抬闸
#define ITASK_HAN_UP		14	//	手动按钮抬闸
#define ITASK_VDO_UP		13	//	视频抬闸
#define ITASK_DG_DW			4	//	地感落闸
#define ITASK_AUTO_DW		2	//	内部控制落闸
#define ITASK_COM_DW		3	//	通信落闸
#define ITASK_HAN_DW		1	//	手动按钮落闸
#define ITASK_INER_DW		16	//	垂直位惯性掉杆
#define ITASK_COER_UP		6	//	水平位强制抬杆
#define ITASK_REACH_V		15	//	到达垂直位
#define ITASK_REACH_H		5	//	到达水平位
#define ITASK_INER_OK		17	//	到达垂直位
#define ITASK_COER_OK		7	//	到达水平位


//副机任务

#define VICE_STA			0x30

#define VICE_STA_EN			1
#define VICE_STA_DGON		2
#define VICE_STA_DGOF		3
#define VICE_STA_DW			4
#define VICE_STA_DRAG		5


#define UPCTTIME			150			//抬闸计数防抖时间
#define UPDCTIME			60			//抬闸计数超时减数




/**************************************************************************** 
* 使用端口定义
****************************************************************************/
#define	PORT_LED			P2
#define	B_LED				4
sbit	LED					= PORT_LED ^ B_LED;




/**************************************************************************** 
* 变量定义/声明
****************************************************************************/
extern volatile u8 xdata iTask;			//当前执行的任务
extern volatile u8 xdata iTask2;		//当前执行的任务
extern volatile u8 xdata ViceDo;		//副机执行任务
extern volatile u8 xdata ViceRal;		//副机实际任务状态
extern volatile u8 xdata ViceRal2;		//副机实际任务状态
extern volatile u8 xdata ViceSta;		//副机任务状态
extern volatile u8 data  ComNum;		//主副机通信计时
extern volatile u8 xdata UpCt;  		//抬闸计数
extern volatile u8  data UpCtNum;  		//抬闸计数防抖
extern volatile u8  data UpDcNum;  		//抬闸计数超时减数

/**************************************************************************** 
* 函数原型
****************************************************************************/
void intMcu();
void intRun();
void sysint();
void iTaskCtr();
void iTaskControl();
void ComClk();
void UpCtClk();
void UpDcClk();
void delay(u8 cnt);
//void delay_ms(u8 ms); 
#endif

