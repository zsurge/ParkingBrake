/****************************************************************************
* File:			main.c														*
* Created:		2017-04-25													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "main.h"

//设备程序版本号			 
u8 const code PG_Version[]={VER0, VER1, VER2, YEAR, MOON, DATE};

//存储器写识别码
u8 const code eight=((YEAR | MOON | DATE | BAIZHA)+DATE+3);



 
 /******************** 主函数**************************/
void main()
{
	sysint();	
	while (1)
	{
		clk();
		InputTask();
		iTaskControl();
		mTaskControl();
        MonitorTask();//监控
		err_task();
		Buzz_task(BuzzMode);		//蜂鸣器任务
		OPutTask();
		uart_aly();
		AtRun();
		WDT_reset(7);
	}
}



