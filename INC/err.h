/****************************************************************************
* File:			err.h														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__ERR_H
#define	__ERR_H

#include "reg52.h"
#include "buzz.h"


/**************************************************************************** 
* 宏定义
****************************************************************************/
	//**************错误报警时间*******************//
#define ERRTIME					50			//报警时间

	
	//**************错误信息*******************//
#define ERR_NONE				0			//无错误信息
#define ERR_INT					1			//初始化
#define ERR_CLR					2			//初始化

#define ERR_M1S					5			//1秒机芯提示
#define ERR_M18S				6			//1.8秒机芯提示
#define ERR_M38S				7			//3.8秒机芯提示
#define ERR_SPRI_LIT			8			//弹簧力度偏小提示
#define ERR_SPRI_MID			9			//弹簧力度中等提示
#define ERR_SPRI_BIG			10			//弹簧力度偏大提示
#define ERR_SPRI_ERR			11			//弹簧力度识别异常提示

#define ERR_LEV1				30			//等级1.无实际意义，用作划分等级
#define ERR_UP_DRAG				31			//抬闸遇阻
#define ERR_DW_DRAG				32			//

#define ERR_LEV2				60			//等级2.无实际意义，用作划分等级
#define ERR_ID_CONFLICT			61			//主机设备号冲突
#define ERR_UP_FAIL				62			//抬闸失败
#define ERR_US_FAIL				63			//抬闸缓冲失败
#define ERR_DW_FAIL				64			//落闸失败
#define ERR_DS_FAIL				65			//落闸缓冲失败
#define ERR_POS_FAIL			66			//实际位置错误
#define ERR_EEPROM_WRITE		67			//数据存储器故障
#define ERR_EEPROM_WRITE1		68			//数据存储器故障












/**************************************************************************** 
* 使用端口定义
****************************************************************************/





/**************************************************************************** 
* 变量定义/声明
****************************************************************************/
extern volatile u8 xdata err;			//错误代码
extern volatile u8 xdata errnum;		//时钟
extern volatile u8 xdata errctr;		//错误计数







/**************************************************************************** 
* 函数原型
****************************************************************************/
void err_volu(u8 dat);
void err_clk();
void err_task();
void intErr();



#endif

