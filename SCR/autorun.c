/****************************************************************************
* File:			autorun.c													*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "AUTORUN.h"

u8 xdata aLine;		//自动运行流程
u8  data AtNum;		//自动运行计时
u8 xdata hafTest;	//半坡抬闸测试
u8 xdata Atctr;		//自动控制


void intAtRun() { aLine=0; AtNum=0; hafTest=0; Atctr=0; }
void AtRun_clk() { if(AtNum) AtNum--; }		//自动运行时钟

void AtRun()
{

	switch(aLine)
		{
			case 0:		break;
			case 1:		
				if(iTask== ITASK_NA) { AtNum= GAPTIME; aLine=2; }
				
				break;	
			
			case 2:
				if(AtNum)break;
				Event(ITASK_AUTO_UP); 
				aLine=3;
				break;
				
			case 3:
				if(iTask== ITASK_NA) { AtNum= GAPTIME; aLine=4; }
				break;

			case 4:
				if(AtNum)break;
				Event(ITASK_AUTO_DW);
				hafTest++;
				aLine=5;
				break;
				
			case 5:
				switch(hafTest)
					{
						case 2:	if(SpeRinN > DwSlo/4*1)	{ Event(ITASK_AUTO_UP);  aLine=3; } break;
						case 4: if(SpeRinN > DwSlo/4*2)	{ Event(ITASK_AUTO_UP);  aLine=3; } break;
						case 6: if(SpeRinN > DwSlo/4*3)	{ Event(ITASK_AUTO_UP);  aLine=3; } break;
						
						case 1:
						case 3:
						case 5: if(iTask== ITASK_NA) { AtNum= GAPTIME; aLine=2; } break;

						case 7: if(iTask== ITASK_NA) { AtNum= GAPTIME; aLine=2; hafTest=0;} break;
							
						default:
							aLine=2; hafTest=0; AtNum=0;
							break;
					}
				break;
				
			default:
				aLine=0; hafTest=0; AtNum=0;
				break;
		}
	
}

