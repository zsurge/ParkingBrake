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



u8 code iTask_buf[7][18]= 	//任务表
{/*  无事件  手动落闸 内部落闸 通信落闸 地感落闸 到水平位 强制抬杆 强抬复位 地感抬闸 防砸抬闸 遇阻抬闸 内部抬闸 通信抬闸 视频抬闸 手动抬闸 到垂直位 惯性掉杆 掉杆复位        事件	*///
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* 状态未初始化 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_NA, TASK_UP, TASK_US, TASK_UP, TASK_NA,	/* 初始未知状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_US,	/* 正在抬闸状态 */
	TASK_NA, TASK_DW, TASK_DW, TASK_DW, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* 垂直状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_DS, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA, 	/* 正在落闸状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA,	/* 水平状态 */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_NA,	/* 错误状态 */
};



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


//定时器2初始化
#if TIMER2
void Timer2_Init(void)
{
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	TL2 = (65536 - 50000) / 256;		//设置定时初值 50000--50ms 9216---10ms
	TH2 = (65536 - 50000) % 256;		//设置定时初值
	TR2 = 1;		//定时器2开始计时


	//  TH2 = (65536-9216)/256;
	//  TL2 = (65536-9216)%256;
	EA=1;                      //打开总中断
	ET2=1;                     //打开定时器中断  
	TR2=1;                     //打开定时器开关
}
#endif




void intRun() { iTask= ITASK_INT; iTask2= ITASK_NA; ViceDo= 0; ViceRal= 0; ViceRal2= 0; ViceSta= 0; UpCt=0; UpCtNum=0; }

void delay(u8 cnt)  //   延时单位:
{
 u16 i;
 i=cnt*200; 
 while (--i);
}


void ComClk() 
	{ 
		if(ComNum) ComNum--;
		if(COM1.RX_TimeOut) COM1.RX_TimeOut--;
	}

void UpCtClk() { if(UpCtNum) UpCtNum--; }
void UpDcClk() { if(UpDcNum) UpDcNum--; }

void sysint()
{
	intMcu();	
	intUsart();
	#if TIMER2
	Timer2_Init();
	#endif
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

	if(iTask < Even)
		{
			iTask2= iTask_buf[mPosD][Even];		//根据事件和状态查表，获取任务
			
			if(iTask2!= MTASK_NA) 
				{
					mTask= iTask2;
					iTask= Even;
					if(!JY) if(iTask== ITASK_DG_UP || iTask== ITASK_DRAG_UP)  UpCt++;
				}
		}
}


void ViceCtr()
{
	if(McuId== MACHI_VICE)
		{
			switch(ViceRal)
				{
					case POS_HOR:
					case POS_DW:
						Event(ITASK_COM_DW);
						speak(VICE_STA,mPosD2);
						break;
					case POS_UP:
					case POS_VER:
						HDwSta=STA_NA; HDwNum=HDW_ON_TIME;
						DgSta=STA_NA; DgNum=DG_OFF_TIME;
						Event(ITASK_COM_UP);
						speak(VICE_STA,mPosD2);
						break;
					case POS_INT:
						speak(VICE_STA,mPosD2);
						break;
					default: break;
				}
			ViceRal=0;
		}
	if(McuId== MACHI_MASTER)
		{	
			if(iTask== ITASK_INT || iTask== ITASK_STR) return;
			if(!ComNum) ComNum= COM_TIME;
			else return;
			switch(mPosD2)
				{
					case POS_HOR:
					case POS_DW:
						switch(ViceRal)
							{
								case POS_HOR:
									ViceSta=0;
									speak(VICE_STA,POS_INT);
									break;
								case POS_DW:
									if(!ViceSta) ViceSta=VICE_STA_EN;
									if(ViceSta==VICE_STA_DGON) 				speak(VICE_STA,POS_UP);
									else									speak(VICE_STA,POS_INT);
									break;
								case POS_VER:
									if(!ViceSta) ViceSta=VICE_STA_EN; 
									if(mPosD2==POS_DW)						speak(VICE_STA,POS_DW);
									if(ViceSta==VICE_STA_DW) 				speak(VICE_STA,POS_DW);
									else if(ViceSta==VICE_STA_DGOF) 		{ speak(VICE_STA,POS_DW); ViceSta=0; }
									else 									speak(VICE_STA,POS_INT);
									break;
								case POS_UP:
									if(ViceSta!=VICE_STA_DGON) 				DgStaClr();
									if(!ViceSta) 							ViceSta=VICE_STA_EN;
									if(mPosD2==POS_DW)						Event(ITASK_COM_UP);
									else if(ViceSta==VICE_STA_DGOF) 		speak(VICE_STA,POS_DW);
									else 									speak(VICE_STA,POS_INT);
									break;
								default: 
									speak(VICE_STA,POS_INT);
									break;
							}
						ViceRal=0;
						break;
					case POS_VER:
					case POS_UP:
						speak(VICE_STA,mPosD2);
						break;
					default: 
						speak(VICE_STA,mPosD2);
						break;
				}
		}
	

	//***********************发送状态处理***********************//
	
}

void iTaskControl()			//外部任务处理
{

	ViceCtr();		//副机控制
	switch(iTask)	//当前执行任务
		{
			case ITASK_INT:
				BmRed();		//拨码参数读取
				if(!HUP && !HDW && !HOR) AtNum= 30;
				else AtNum= 6;
				iTask=ITASK_STR;
				break;

			case ITASK_STR:
				if(!HUP && !HDW && !HOR && !AtNum) { aLine= 1;iTask= ITASK_NA; }
				else if(!AtNum) { iTask=ITASK_NA; }
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
				iTaskCtr();
				break;
				
			case ITASK_DG_UP:
			case ITASK_DRAG_UP:
			case ITASK_FZ_UP:
			case ITASK_AUTO_UP:
			case ITASK_COM_UP:
			case ITASK_VDO_UP:
			case ITASK_HAN_UP:
				iTaskCtr();
				break;
				
			case ITASK_HAN_DW:
			case ITASK_COM_DW:
			case ITASK_AUTO_DW:
			case ITASK_DG_DW:
				iTaskCtr();
				break;

				
			default: iTask=ITASK_NA; break;		//电机相关变量初始化
		}
}

void t0_interrupt(void)  interrupt 1 //using 1
{
	TF0= 0;
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xF8;		//设置定时初值


	T1Ms_clk();
	mCtrClk();
	ComClk();
//	LED=!LED;
}




//	落闸时分为5个阶段，分别是
//	1.起步阶段，垂直位到起步位
//	2.运行阶段，起步位到中段
//	3.减速阶段，中段到缓冲位
//	4.缓冲阶段，缓冲到平稳位
//	5.稳定阶段，平稳位到水平位
//	大概思路是，根据测试的数据，得出起步阶段和运行阶段的所需要的转数区间
//	然后从这个区间开始判定，根据进入定时器的次数，得到新的转数，以及总的时间
//	这个时间再根据经验值，判定是不是超出我们预期的转数区间，若是，则抬闸

//预期的落闸时间为5秒，快于5秒，认为就有风险

#if 0 
void testTimer2(void)
{	
	//首先判断是否到了减速阶段
	//30是假设到减速阶段测速环的转数，实际需要去手动的计量
	if(SpeRinN >= 30) //已到减速阶段，之后要根据测速环的圈数和时间判定是否失速
	{
		if(gRepairMotor.CurrentCounts == 0)
		{
			gRepairMotor.InitSpeRin = SpeRinN;//记录开始计算前测速环的圈数
		}
		
		gRepairMotor.CurrentCounts++;//进入中断的次数，根据中断的次数判定所用的时间
		
		//因为是10ms的定时器，3.8的电机每个周期是22.5ms，所有进行判定每50ms有两次
		//这里测试使用，先固定3.8电机，release时会使用宏定义，定义不同的计算方式和判定条件
		if((gRepairMotor.CurrentCounts != 0) && (gRepairMotor.CurrentCounts % 5 == 0))
		{
			if (gRepairMotor.FlagValue >= 4)
			{
				Event(ITASK_DG_UP);//危险，抬杆
			}
			
			if(gRepairMotor.LastSpeRin !=0)
			{
				//这里根据是开5U电机和3U电机判定，空载最快8圈，所以取6圈做为判定条件
				if((SpeRinN - gRepairMotor.LastSpeRin) >= 4  /*&&这里应该根据电压*/)
				{
					//这个时候就比较危险了
					gRepairMotor.FlagValue++;
				}
			}

			gRepairMotor.LastSpeRin = SpeRinN;
			
			//打印当前测速环值
			Uart_Print(3,gRepairMotor.Direction);			

		}
	}
	
}

//定时器中断2
void Timer2_ISR(void) interrupt 5 //定时器2中断
{
	TF2=0;
	EXF2 = 0;
	TL2 = (65536 - 9216) / 256;		//设置定时初值
	TH2 = (65536 - 9216) % 256;		//设置定时初值

	testTimer2();  
}
#endif 

#if 1 
#define TIMER_FRE  90 
#define BASIC_SPEED_100MS 8 //每100ms测速环转到的圈数
#define SPRING_PARAM_OFFSET 2//弹簧参数
#define STANDARD_VALUE 72//弹簧标准值


void motorTimer2(void)
{	
# if 1
    //每隔100毫秒进来判定一次
	if(SpeRinN >= 1) //测速环转动才开始进来
	{		
	    //从第900毫秒的时候进来，然后每100毫秒判定一次
    	if(gRepairMotor.CurrentCounts++ >= TIMER_FRE);//进入中断的次数，根据中断的次数判定所用的时间
        {      
    		//这里测试使用，先固定3.8电机，release时会使用宏定义，定义不同的计算方式和判定条件
    		
    		if(gRepairMotor.CurrentCounts % TIMER_FRE == 0)
    		{
                //先判定抬闸
                if(gRepairMotor.Direction == POS_UP)
                {
                    if(gRepairMotor.CurrentCounts == TIMER_FRE)
                    {
                        if (SpeRinN > STANDARD_VALUE + SPRING_PARAM_OFFSET)
                        {
                            //三根弹簧
                            gCurrentSpringNum = 3;
                        }
                        else if(SpeRinN < STANDARD_VALUE + SPRING_PARAM_OFFSET && SpeRinN >= STANDARD_VALUE -SPRING_PARAM_OFFSET)
                        {
                            //两根弹簧，需要报警
                            gCurrentSpringNum = 2;
                        }
                        else
                        {
                            //只有一根弹簧
                            gCurrentSpringNum = 1;
                        }
                    }
                }
                else //落闸
                {
                    if(gCurrentSpringNum == 1 || SpeRinN >= STANDARD_VALUE + SPRING_PARAM_OFFSET*2-1)
                    {
                        //执行抬闸
                        Event(ITASK_DG_UP);
                    }
                
                }
                
    			//打印当前测速环值
    			Uart_Print(3,gRepairMotor.Direction);		
              }

		}
	}
#else 
	g_timer_run_counter++;
	if(g_timer_run_counter % TIMER_FRE == 0)
	{
		if(SpeRinN > FISTER_SPRING_BREAK && SpeRinN <= SECOND_SPRING_BREAK && g_timer_run_counter == 1)
		{
			g_timer_run_counter = 0;
			Event(ITASK_DG_UP);//危险，抬杆
		}
		else if(SpeRinN > SECOND_SPRING_BREAK && SpeRinN <= THIRD_SPRING_BREAK && g_timer_run_counter == 2)
		{
			g_timer_run_counter = 0;
			Event(ITASK_DG_UP);//危险，抬杆
		}
		else if(SpeRinN > THIRD_SPRING_BREAK && SpeRinN <= FORTH_SPRING_BREAK  && g_timer_run_counter == 3)
		{
			g_timer_run_counter = 0;
			Event(ITASK_DG_UP);//危险，抬杆
		}
		else
		{
			g_timer_run_counter = 0;
		}

		//打印当前测速环值
		Uart_Print(3,gRepairMotor.Direction);
	}

	#endif
}

//定时器中断2
void Timer2_ISR(void) interrupt 5 //定时器2中断
{
	TF2=0;
	EXF2 = 0;
	TL2 = (65536 - 9216) / 256;		//设置定时初值
	TH2 = (65536 - 9216) % 256;		//设置定时初值

	motorTimer2();  
}

//void Timer2_Zero(void)
//{	
//	TF2=0;
//	EXF2 = 0;	
//}

#endif


