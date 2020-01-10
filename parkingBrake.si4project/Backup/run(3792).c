/****************************************************************************
* File:			RUN.c														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:	程序运行指示 & 函数的计时									*
****************************************************************************/
#include "RUN.h"


u8 xdata iTask;			//当前执行的任务
u8 xdata iTask2;		//当前执行的任务
u8 xdata ViceDo;		//副机执行任务
u8 xdata ViceRal;		//副机实际任务状态
u8 xdata ViceRal2;		//副机实际任务状态
u8 xdata ViceSta;		//副机实际任务状态
u8 data  ComNum;		//主副机通信计时
u8 xdata UpCt;  		//抬闸计数
u8  data UpCtNum;  		//抬闸计数防抖
u8  data UpDcNum;  		//抬闸计数超时减数
volatile u8 idata gSpe_num;      //测试用的速度记数器




u8 code iTask_buf[7][18]= 	//任务表
{
	/*  无事件  手动落闸 内部落闸 通信落闸 地感落闸 到水平位 强制抬杆 强抬复位 地感抬闸 防砸抬闸 遇阻抬闸 内部抬闸 通信抬闸 视频抬闸 手动抬闸 到垂直位 惯性掉杆 掉杆复位        事件	*///
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* 状态未初始化 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_NA, TASK_UP, TASK_US, TASK_UP, TASK_NA,	/* 初始未知状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_US,	/* 正在抬闸状态 */
	TASK_NA, TASK_DW, TASK_DW, TASK_DW, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* 垂直状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_DS, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA, 	/* 正在落闸状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA,	/* 水平状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_NA,	/* 错误状态 */
};


#ifdef _20190419_Debug
void intMcu()
{
	EA=0;	    //禁止全局中断

	SCON=0x50;  //串行接口工作于模式1，允许接收数据SM0 SM1 SM2 REN TB8 RB8 TI RI
	TMOD=0x21;  //计时器1工作于模式2 | GATE	C/T M1 M0 | GATE C/T M1	M0 |
	//计时器0工作于模式1 |	Timer1	  |    Timer0	   |a
//串口参数配置
//  TH1=0xF4;   /*设定波特率为2400,	晶振11.0592MHz*/
//  TH1=0xE8;   /*设定波特率为2400, 晶振22.1184MHz*/
//	TH1=0xFD;   /*设定波特率为9600，晶振11.0592MHz*/
//	TH1=0xFA;   /*设定波特率为9600，晶振22.1184MHz*/
	TH1=0xFD;   /*设定波特率为19200，晶振22.1184MHz*/
	TR1=1;		//启动计时器1开始计数
	ES=1;	    //允许串口中断
//定时器0配置
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xF8;		//设置定时初值
	TR0=1;   	//启动定时器0
	ET0=1;	    //允许T0中断
//中断0配置
//	IT0=1;		//外部中断0电平跳变产生中断
	EX0=0;	  	//禁止外部中断0
//中断1配置
//	IT1=1;      //外部中断1电平跳变产生中断
	EX1=0;   	//禁止外部中断1
//优先级配置
	PS=1;	    //设定串口中断优先
//中断总开关配置
	EA=1;
}

#endif

void intMcu()
{
	EA=0;	    //禁止全局中断

	SCON=0x50;  //串行接口工作于模式1，允许接收数据SM0 SM1 SM2 REN TB8 RB8 TI RI
	TMOD=0x21;  //计时器1工作于模式2 | GATE	C/T M1 M0 | GATE C/T M1	M0 |
	//计时器0工作于模式1 |	Timer1	  |    Timer0	   |a
//串口参数配置
	TH1=0xFD;   /*设定波特率为19200，晶振22.1184MHz*/
	TR1=1;		//启动计时器1开始计数
	ES=1;	    //允许串口中断
//定时器0配置
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xF8;		//设置定时初值
	TR0=1;   	//启动定时器0
	ET0=1;	    //允许T0中断
//中断0配置
	EX0=1;	  	//使能外部中断0
	IT0=1;		//外部中断0电平跳变产生中断

//中断1配置
//	IT1=1;      //外部中断1电平跳变产生中断
	EX1=0;   	//禁止外部中断1
//优先级配置
	PS=1;	    //设定串口中断优先
//中断总开关配置
	EA=1;
}


void intRun()
{
	iTask= ITASK_INT;
	iTask2= ITASK_NA;
	ViceDo= 0;
	ViceRal= 0;
	ViceRal2= 0;
	ViceSta= 0;
	UpCt=0;
	UpCtNum=0;
}

void delay ( u8 cnt ) //   延时单位:
{
	u16 i;
	i=cnt*200;
	while ( --i );
}

//void delay_ms(u8 ms)
//{
//    u16 i;
//    do{
//        i=MAIN_Fosc/9600;
//        while(--i);     //96T per loop
//    }while(--ms);       //--ms  ms=ms-1
//}




void ComClk()
{
	if ( ComNum )
	{
		ComNum--;
	}
	if ( COM1.RX_TimeOut )
	{
		COM1.RX_TimeOut--;
	}
}

void UpCtClk()
{
	if ( UpCtNum )
	{
		UpCtNum--;
	}
}
void UpDcClk()
{
	if ( UpDcNum )
	{
		UpDcNum--;
	}
}

void sysint()
{
	intMcu();
	intUsart();
	intRun();
	intBuzz();
	intErr();
	intInput();
	intclk();
	intMotor();
	intOut();
	intAtRun();
	RomTest();
}



void iTaskCtr()
{

	if ( iTask < Even )
	{
		iTask2= iTask_buf[mPosD][Even];		//根据事件和状态查表，获取任务
		if ( iTask2!= MTASK_NA )
		{
			mTask= iTask2;
			iTask= Even;
			if ( !JY )
				if ( iTask== ITASK_DG_UP || iTask== ITASK_DRAG_UP )
				{
					UpCt++;
				}
		}

	}
}


void ViceCtr()
{
	if ( McuId== MACHI_VICE )
	{
		switch ( ViceRal )
		{
			case POS_HOR:
			case POS_DW:
				Event ( ITASK_COM_DW );
				speak ( VICE_STA,mPosD2 );
				break;
			case POS_UP:
			case POS_VER:
				HDwSta=STA_NA;
				HDwNum=HDW_ON_TIME;
				DgSta=STA_NA;
				DgNum_NoSignal=DG_OFF_TIME;
				Event ( ITASK_COM_UP );
				speak ( VICE_STA,mPosD2 );
				break;
			case POS_INT:
				speak ( VICE_STA,mPosD2 );
				break;
			default:
				break;
		}
		ViceRal=0;
	}
	if ( McuId== MACHI_MASTER )
	{
		if ( iTask== ITASK_INT || iTask== ITASK_STR )
		{
			return;
		}
		if ( !ComNum )
		{
			ComNum= COM_TIME;
		}
		else
		{
			return;
		}
		switch ( mPosD2 )
		{
			case POS_HOR:
			case POS_DW:
				switch ( ViceRal )
				{
					case POS_HOR:
						ViceSta=0;
						speak ( VICE_STA,POS_INT );
						break;
					case POS_DW:
						if ( !ViceSta )
						{
							ViceSta=VICE_STA_EN;
						}
						if ( ViceSta==VICE_STA_DGON )
						{
							speak ( VICE_STA,POS_UP );
						}
						else
						{
							speak ( VICE_STA,POS_INT );
						}
						break;
					case POS_VER:
						if ( !ViceSta )
						{
							ViceSta=VICE_STA_EN;
						}
						if ( mPosD2==POS_DW )
						{
							speak ( VICE_STA,POS_DW );
						}
						if ( ViceSta==VICE_STA_DW )
						{
							speak ( VICE_STA,POS_DW );
						}
						else if ( ViceSta==VICE_STA_DGOF )
						{
							speak ( VICE_STA,POS_DW );
							ViceSta=0;
						}
						else
						{
							speak ( VICE_STA,POS_INT );
						}
						break;
					case POS_UP:
						if ( ViceSta!=VICE_STA_DGON )
						{
							DgStaClr();
						}
						if ( !ViceSta )
						{
							ViceSta=VICE_STA_EN;
						}
						if ( mPosD2==POS_DW )
						{
							Event ( ITASK_COM_UP );
						}
						else if ( ViceSta==VICE_STA_DGOF )
						{
							speak ( VICE_STA,POS_DW );
						}
						else
						{
							speak ( VICE_STA,POS_INT );
						}
						break;
					default:
						speak ( VICE_STA,POS_INT );
						break;
				}
				ViceRal=0;
				break;
			case POS_VER:
			case POS_UP:
				speak ( VICE_STA,mPosD2 );
				break;
			default:
				speak ( VICE_STA,mPosD2 );
				break;
		}
	}


	//***********************发送状态处理***********************//

}

void iTaskControl()			//外部任务处理
{
	ViceCtr();		//副机控制
	switch ( iTask )	//当前执行任务
	{
		case ITASK_INT:
			BmRed();		//拨码参数读取
			if ( !HUP && !HDW && !HOR )
			{
				AtNum= 30;
			}
			else
			{
				AtNum= 6;
			}
			iTask=ITASK_STR;
			break;

		case ITASK_STR:
			if ( !HUP && !HDW && !HOR && !AtNum )
			{
				aLine= 1;
				iTask= ITASK_NA;
			}
			else if ( !AtNum )
			{
				iTask=ITASK_NA;
			}
			break;

		case ITASK_REACH_H:
		case ITASK_REACH_V:
		case ITASK_INER_OK:
		case ITASK_COER_OK:
			iTask=ITASK_NA;
			break;


		case ITASK_NA:
		case ITASK_COER_UP:
		case ITASK_INER_DW:
//				iTaskCtr();
//				break;

		case ITASK_DG_UP:
		case ITASK_DRAG_UP:
		case ITASK_FZ_UP:
		case ITASK_AUTO_UP:
		case ITASK_COM_UP:
		case ITASK_VDO_UP:
		case ITASK_HAN_UP:
//				iTaskCtr();
//				break;

		case ITASK_HAN_DW:
		case ITASK_COM_DW:
		case ITASK_AUTO_DW:
		case ITASK_DG_DW:
			iTaskCtr();
			break;


		default:
			iTask=ITASK_NA;
			break;		//电机相关变量初始化
	}
}

void t0_interrupt ( void )  interrupt 1 //using 1
{
	TF0= 0;
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xF8;		//设置定时初值


	T1Ms_clk();
	mCtrClk();
	ComClk();
}

/*****************************************************************************
 函 数 名  : int0_interrupt
 功能描述  : 外部中断0，处理速度传感器
 输入参数  : void
             
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2019年4月18日
    作    者   : 张舵

    修改内容   : 新生成函数

*****************************************************************************/
void int0_interrupt( void )  interrupt 0 //using 0
{
    gSpe_num++;
    if(gSpe_num % 20 == 0)
    {
        LED=!LED;
    }

    IE0 = 0;
}
