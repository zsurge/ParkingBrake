/****************************************************************************
* File:			buzz.c														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
 #include "buzz.h"

u8 xdata BuzzMode;
u8 data  BuzzNum;



void Buzz_clk()
{
	if(BuzzNum>10)BuzzNum--;
}


void intBuzz() { BuzzMode=BUZZ_NA; BuzzNum=0; BUZZ=BUZZ_CL;}

//************************************************************************************************************//
//buzz_task蜂鸣器任务
//************************************************************************************************************//
void Buzz_task(u8 mode)
{
	if(BuzzMode!=mode) {BuzzMode=mode; BuzzNum=0;}
	switch(BuzzMode)
		{
			case BUZZ_NA: BuzzNum=0; BUZZ=BUZZ_CL;  break;
			
			case BUZZ_B:			//哔(只响一声，用于按键操作)	
				switch(BuzzNum)
					{
						case 10:			BuzzNum=0;	BuzzMode=BUZZ_NA;	BUZZ=BUZZ_CL;	break;
						case 0:				BuzzNum=16; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;
				
			case BUZZ_BB:			//哔哔(只响两声，用于按键操作)
				switch(BuzzNum)
					{
						case 22:			BUZZ=BUZZ_CL;	break;					
						case 16:			BUZZ=BUZZ_EN;	break;
						case 10:			BuzzNum=0;	BuzzMode=BUZZ_NA;	BUZZ=BUZZ_CL;	break;
						case 0:				BuzzNum=28; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_BB_B:			
				switch(BuzzNum)
					{
						case 30:			BUZZ=BUZZ_CL;	break;					
						case 20:			BUZZ=BUZZ_EN;	break;
						case 10:			BuzzNum=0;	BuzzMode=BUZZ_NA;	BUZZ=BUZZ_CL;	break;
						case 0:				BuzzNum=130; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_BB_B_B:			
				switch(BuzzNum)
					{
						case 50:			BUZZ=BUZZ_CL;	break;	
						case 40:			BUZZ=BUZZ_EN;	break;
						case 30:			BUZZ=BUZZ_CL;	break;					
						case 20:			BUZZ=BUZZ_EN;	break;
						case 10:			BuzzNum=0;	BuzzMode=BUZZ_NA;	BUZZ=BUZZ_CL;	break;
						case 0:				BuzzNum=150; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;
			
			case BUZZ_BB_B_B_B:			
				switch(BuzzNum)
					{
						case 70:			BUZZ=BUZZ_CL;	break;	
						case 60:			BUZZ=BUZZ_EN;	break;
						case 50:			BUZZ=BUZZ_CL;	break;	
						case 40:			BUZZ=BUZZ_EN;	break;
						case 30:			BUZZ=BUZZ_CL;	break;					
						case 20:			BUZZ=BUZZ_EN;	break;
						case 10:			BuzzNum=0;	BuzzMode=BUZZ_NA;	BUZZ=BUZZ_CL;	break;
						case 0: 			BuzzNum=170;		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_BB_BB:			
				switch(BuzzNum)
					{
						case 122:			BUZZ=BUZZ_CL;	break;					
						case 110:			BUZZ=BUZZ_EN;	break;
						case 10:			BuzzNum=0;	BuzzMode=BUZZ_NA;	BUZZ=BUZZ_CL;	break;
						case 0:				BuzzNum=222; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_BBB:			//哔哔(只响三声，用于按键操作)
				switch(BuzzNum)
					{
						case 34:			BUZZ=BUZZ_CL;	break;
						case 28:			BUZZ=BUZZ_EN;	break;
						case 22:			BUZZ=BUZZ_CL;	break;					
						case 16:			BUZZ=BUZZ_EN;	break;
						case 10:			BuzzNum=0;	BuzzMode=BUZZ_NA;	BUZZ=BUZZ_CL;	break;
						case 0:				BuzzNum=40; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;
				
			case BUZZ_B02:			//哔~~~200毫秒	
				switch(BuzzNum)
					{
						case 16:			BUZZ=BUZZ_CL;						break;
						case 10:
						case 1:
						case 0:				BuzzNum=22; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;
				
			case BUZZ_B2:			//哔~~~~2秒频率
				switch(BuzzNum)
					{
						case 195:			BUZZ=BUZZ_CL;						break;
						case 10:
						case 1:
						case 0:				BuzzNum=200; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;
				
			case BUZZ_BB2:			//哔哔~~~2秒频率
				switch(BuzzNum)
					{
						case 196:			BUZZ=BUZZ_CL;						break;
						case 192:			BUZZ=BUZZ_EN;						break;
						case 188:			BUZZ=BUZZ_CL;						break;
						case 10:
						case 1:
						case 0:				BuzzNum=200; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_BBB2:			//哔哔哔~~~~2秒频率
				switch(BuzzNum)
					{
						case 196:			BUZZ=BUZZ_CL;						break;
						case 192:			BUZZ=BUZZ_EN;						break;
						case 188:			BUZZ=BUZZ_CL;						break;
						case 184:			BUZZ=BUZZ_EN;						break;
						case 180:			BUZZ=BUZZ_CL;						break;
						case 10:
						case 1:
						case 0:				BuzzNum=200; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_B_B:			//哔(长鸣1秒)~~~~哔
				switch(BuzzNum)
					{
						case 180:			BUZZ=BUZZ_CL;						break;
						case 160:			BUZZ=BUZZ_EN;						break;
						case 155:			BUZZ=BUZZ_CL;						break;
						case 10:
						case 1:
						case 0:				BuzzNum=250; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_B_BB:			//哔(长鸣1秒)~~~~哔哔
				switch(BuzzNum)
					{
						case 180:			BUZZ=BUZZ_CL;						break;
						case 160:			BUZZ=BUZZ_EN;						break;
						case 155:			BUZZ=BUZZ_CL;						break;
						case 150:			BUZZ=BUZZ_EN;						break;
						case 145:			BUZZ=BUZZ_CL;						break;
						case 10:	
						case 1:
						case 0:				BuzzNum=250; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;

			case BUZZ_B_BBB:		//哔(长鸣1秒)~~~~哔哔哔
				switch(BuzzNum)
					{
						case 180:			BUZZ=BUZZ_CL;						break;
						case 160:			BUZZ=BUZZ_EN;						break;
						case 155:			BUZZ=BUZZ_CL;						break;
						case 150:			BUZZ=BUZZ_EN;						break;
						case 145:			BUZZ=BUZZ_CL;						break;
						case 140:			BUZZ=BUZZ_EN;						break;
						case 135:			BUZZ=BUZZ_CL;						break;
						case 10:
						case 1:
						case 0:				BuzzNum=250; 		BUZZ=BUZZ_EN; break;
						default: break;
					}
				break;
				
			default:  BuzzMode=BUZZ_NA; BuzzNum=0; break;	//蜂鸣器相关变量初始化
				
		}
}
