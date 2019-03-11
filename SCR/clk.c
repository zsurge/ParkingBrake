/****************************************************************************
* File:			clk.c														*
* Created:		2017-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "clk.h"

u8 T10MS; 		//10毫秒计量变量
u8 T100MS;		//100毫秒计量变量
u8 T1000MS;		//1000毫秒计量变量

u16 xdata test;			//主程序在100MS内循环跑N次，此参数可观察单片机运行效率
u16 xdata test_dly;		//主程序在100MS内循环跑N次，用作数码管显示使用



void intclk() { T10MS=0; T100MS=0; T1000MS=0;}


void T1Ms_clk()	{ if(T10MS) T10MS--; }	//1毫秒时钟
void test_clk()	{ test_dly=test; test=0; }

void clk()							//设备运行时钟
{
	if(!T10MS)						//由定时器产生1毫秒中断，计时10MS
		{
			T10MS=T10MS_TIME;
			if(T100MS)T100MS--;
			
			Buzz_clk();
			Dg_clk();
			Fz_clk();
			Pos_clk();
			Hup_clk();
			Hdw_clk();
			Vup_clk();
			UpCtClk();
			BrakeClk();
		}

	if(!T100MS)						//计时100MS
		{
			T100MS=T100MS_TIME;
			if(T1000MS)T1000MS--;
			
//			LED=!LED;
			AtRun_clk();
			err_clk();
			DZP_clk();
			test_clk();	
			DZP_clk();
			mRunClk();
			UpDcClk();
			
//			speak(2,1);
		}
	
	if(!T1000MS)					//计时1000MS
		{
			T1000MS=T1000MS_TIME;
			
		}
	test++;							//主程序在100MS内循环跑N次，此参数可观察单片机运行效率
}

