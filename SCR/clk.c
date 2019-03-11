/****************************************************************************
* File:			clk.c														*
* Created:		2017-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "clk.h"

u8 T10MS; 		//10�����������
u8 T100MS;		//100�����������
u8 T1000MS;		//1000�����������

u16 xdata test;			//��������100MS��ѭ����N�Σ��˲����ɹ۲쵥Ƭ������Ч��
u16 xdata test_dly;		//��������100MS��ѭ����N�Σ������������ʾʹ��



void intclk() { T10MS=0; T100MS=0; T1000MS=0;}


void T1Ms_clk()	{ if(T10MS) T10MS--; }	//1����ʱ��
void test_clk()	{ test_dly=test; test=0; }

void clk()							//�豸����ʱ��
{
	if(!T10MS)						//�ɶ�ʱ������1�����жϣ���ʱ10MS
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

	if(!T100MS)						//��ʱ100MS
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
	
	if(!T1000MS)					//��ʱ1000MS
		{
			T1000MS=T1000MS_TIME;
			
		}
	test++;							//��������100MS��ѭ����N�Σ��˲����ɹ۲쵥Ƭ������Ч��
}

