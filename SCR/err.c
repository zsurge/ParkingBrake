/****************************************************************************
* File:			ERR.c														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "ERR.h"

u8 xdata err;		//错误代码
u8 xdata errnum;	//时钟
u8 xdata errctr;	//错误计数


void intErr() { err=ERR_INT; errnum=0; errctr=0; }
void err_clk() { if(errnum) errnum--; }

void err_volu(u8 dat)
{
	if(dat>=err) { err = dat; errctr++; errnum= ERRTIME; }
}

//************************************************************************************************************/
//err_task
//************************************************************************************************************/
void err_task()
{
	switch(err)
		{
			case ERR_NONE:
				break;
			case ERR_CLR:
				Buzz_task(BUZZ_NA);
				err=ERR_NONE;
				break;
//************************************************************************//

//************************************************单次提示鸣响******************//
			case ERR_INT:
				Buzz_task(BUZZ_B);
				err=ERR_NONE;
				break;
			case ERR_M1S:
				Buzz_task(BUZZ_B);
				err=ERR_NONE;
				break;
			case ERR_M18S:
			case ERR_EEPROM_WRITE:
				Buzz_task(BUZZ_BB);
				err=ERR_NONE;
				break;
			case ERR_M38S:
			case ERR_EEPROM_WRITE1:
				Buzz_task(BUZZ_BBB);
				err=ERR_NONE;
				break;
			case ERR_SPRI_BIG:
				Buzz_task(BUZZ_BB_B_B_B);
				err=ERR_NONE;
				break;
			case ERR_SPRI_ERR:
				Buzz_task(BUZZ_BB_BB);
				err=ERR_NONE;
				break;
			case ERR_SPRI_LIT:
				Buzz_task(BUZZ_BB_B);
				err=ERR_NONE;
				break;
			case ERR_SPRI_MID:
				Buzz_task(BUZZ_BB_B_B);
				err=ERR_NONE;
				break;
//************************************************************************//

//*************************************************5秒内提示鸣响******************//			
			case ERR_UP_DRAG:				//抬闸遇阻
				Buzz_task(BUZZ_B2);
				if(!errnum) err= ERR_CLR; 	//定时清除报警
				break;
			case ERR_DW_DRAG:				//落闸遇阻
				Buzz_task(BUZZ_B02);
				if(!errnum) err= ERR_CLR;
				break;
//************************************************************************//

//*************************************************故障持续提示鸣响*******//	
			case ERR_ID_CONFLICT:
				Buzz_task(BUZZ_B2);
				break;
			case ERR_UP_FAIL:		//抬闸失败
				Buzz_task(BUZZ_B_BB);
				break;
			case ERR_DW_FAIL:
				Buzz_task(BUZZ_B_BBB);
				break;
			case ERR_US_FAIL:
				Buzz_task(BUZZ_BB2);
				break;
			case ERR_DS_FAIL:
				Buzz_task(BUZZ_BBB2);
				break;
			case ERR_POS_FAIL:
				Buzz_task(BUZZ_B_B);
				break;
			default:
				err=ERR_CLR;
				break;
		}
	if(err> ERR_LEV2)
		{
			if(!errctr) err= ERR_CLR; 	//无错误累加时，自动清除错误
			else errctr=0;			
		}
	
}

