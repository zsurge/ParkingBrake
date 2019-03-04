/****************************************************************************
* File:			Output.c													*
* Created:		2015-07-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:	输出控制													*
****************************************************************************/
#include "Output.h"



u8 data DZPnum;
u8 data DlyBrakeNum;
u16 data BrakeNum;
u8 data BrakeSta;





void intOut()	{ DZPnum=0; DlyBrakeNum=0; BrakeNum=0; BrakeSta=0; }
void DZP_clk()	{ if(DZPnum)	DZPnum--; }		//道闸反馈信号时钟
void BrakeClk()	{ if(BrakeNum) BrakeNum--;  if(DlyBrakeNum) DlyBrakeNum--; }


void BrakeClr() { KK=1; BrakeNum=0; DlyBrakeNum=0; BrakeSta=0; }

void OPutTask()
{
	if(mTask== MTASK_UP1) 
		{
			RG= KK_GRE;
			if(!BM3 && (iTask==ITASK_DG_UP || iTask==ITASK_FZ_UP || iTask==ITASK_DRAG_UP || iTask== ITASK_INER_DW)) { DZP_CL; }
			else { DZP_UP;}
			DZPnum=DZP_TIME;
		}
	else if(mTask== MTASK_DW1)
		{
			RG= KK_RED;
			DZP_DW;
			DZPnum= DZP_TIME;
		}
	else if(!DZPnum) DZP_CL;

	switch(BrakeSta)
		{
			case 0:
				if(mTask/10==MTASK_DS || mTask/10==MTASK_US){ DlyBrakeNum=DLYBRAKETIME; BrakeSta++; }
				break;
				
			case 1:
				if(!DlyBrakeNum) { KK=0; BrakeNum=BRAKETIME; BrakeSta++; }
				break;
				
			case 2:
				if(!BrakeNum) { KK=1; BrakeSta++; }
				break;
				
			case 3: break;
				
			default: break;
		}
}

