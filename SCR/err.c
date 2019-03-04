/****************************************************************************
* File:			ERR.c														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "ERR.h"

u8 xdata err;		//�������
u8 xdata errnum;	//ʱ��
u8 xdata errctr;	//�������


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

//************************************************������ʾ����******************//
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

//*************************************************5������ʾ����******************//			
			case ERR_UP_DRAG:				//̧բ����
				Buzz_task(BUZZ_B2);
				if(!errnum) err= ERR_CLR; 	//��ʱ�������
				break;
			case ERR_DW_DRAG:				//��բ����
				Buzz_task(BUZZ_B02);
				if(!errnum) err= ERR_CLR;
				break;
//************************************************************************//

//*************************************************���ϳ�����ʾ����*******//	
			case ERR_ID_CONFLICT:
				Buzz_task(BUZZ_B2);
				break;
			case ERR_UP_FAIL:		//̧բʧ��
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
			if(!errctr) err= ERR_CLR; 	//�޴����ۼ�ʱ���Զ��������
			else errctr=0;			
		}
	
}

