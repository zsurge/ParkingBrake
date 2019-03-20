/****************************************************************************
* File:			romtest.h													*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__ROMTEST_H
#define	__ROMTEST_H


#include "reg52.h"
#include "RUN.h"
#include "ERR.h"
#include "eeprom.h"




/**************************************************************************** 
* 宏定义
****************************************************************************/





/**************************************************************************** 
* 使用端口定义
****************************************************************************/





/**************************************************************************** 
* 变量定义/声明
****************************************************************************/

extern volatile u8 code PG_Version[];
extern volatile u8 code eight;







/**************************************************************************** 
* 函数原型
****************************************************************************/


extern volatile void SectotE(u16 dx, u16 hf, u8 tt);
extern volatile void SectotWT(u16 dx, u8 nu, u8 tt);
extern volatile bit SectotRT(u16 dx, u8 nu, u8 tt);
extern volatile void RomTest();




#endif


