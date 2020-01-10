/****************************************************************************
* File:			RUN.c														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:	��������ָʾ & �����ļ�ʱ									*
****************************************************************************/
#include "RUN.h"


u8 xdata iTask;			//��ǰִ�е�����
u8 xdata iTask2;		//��ǰִ�е�����
u8 xdata ViceDo;		//����ִ������
u8 xdata ViceRal;		//����ʵ������״̬
u8 xdata ViceRal2;		//����ʵ������״̬
u8 xdata ViceSta;		//����ʵ������״̬
u8 data  ComNum;		//������ͨ�ż�ʱ
u8 xdata UpCt;  		//̧բ����
u8  data UpCtNum;  		//̧բ��������
u8  data UpDcNum;  		//̧բ������ʱ����
volatile u8 idata gSpe_num;      //�����õ��ٶȼ�����




u8 code iTask_buf[7][18]= 	//�����
{
	/*  ���¼�  �ֶ���բ �ڲ���բ ͨ����բ �ظ���բ ��ˮƽλ ǿ��̧�� ǿ̧��λ �ظ�̧բ ����̧բ ����̧բ �ڲ�̧բ ͨ��̧բ ��Ƶ̧բ �ֶ�̧բ ����ֱλ ���Ե��� ���˸�λ        �¼�	*///
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* ״̬δ��ʼ�� */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_NA, TASK_UP, TASK_US, TASK_UP, TASK_NA,	/* ��ʼδ֪״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_US,	/* ����̧բ״̬ */
	TASK_NA, TASK_DW, TASK_DW, TASK_DW, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* ��ֱ״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_DS, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA, 	/* ������բ״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA,	/* ˮƽ״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_NA,	/* ����״̬ */
};


#ifdef _20190419_Debug
void intMcu()
{
	EA=0;	    //��ֹȫ���ж�

	SCON=0x50;  //���нӿڹ�����ģʽ1�������������SM0 SM1 SM2 REN TB8 RB8 TI RI
	TMOD=0x21;  //��ʱ��1������ģʽ2 | GATE	C/T M1 M0 | GATE C/T M1	M0 |
	//��ʱ��0������ģʽ1 |	Timer1	  |    Timer0	   |a
//���ڲ�������
//  TH1=0xF4;   /*�趨������Ϊ2400,	����11.0592MHz*/
//  TH1=0xE8;   /*�趨������Ϊ2400, ����22.1184MHz*/
//	TH1=0xFD;   /*�趨������Ϊ9600������11.0592MHz*/
//	TH1=0xFA;   /*�趨������Ϊ9600������22.1184MHz*/
	TH1=0xFD;   /*�趨������Ϊ19200������22.1184MHz*/
	TR1=1;		//������ʱ��1��ʼ����
	ES=1;	    //�������ж�
//��ʱ��0����
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xF8;		//���ö�ʱ��ֵ
	TR0=1;   	//������ʱ��0
	ET0=1;	    //����T0�ж�
//�ж�0����
//	IT0=1;		//�ⲿ�ж�0��ƽ��������ж�
	EX0=0;	  	//��ֹ�ⲿ�ж�0
//�ж�1����
//	IT1=1;      //�ⲿ�ж�1��ƽ��������ж�
	EX1=0;   	//��ֹ�ⲿ�ж�1
//���ȼ�����
	PS=1;	    //�趨�����ж�����
//�ж��ܿ�������
	EA=1;
}

#endif

void intMcu()
{
	EA=0;	    //��ֹȫ���ж�

	SCON=0x50;  //���нӿڹ�����ģʽ1�������������SM0 SM1 SM2 REN TB8 RB8 TI RI
	TMOD=0x21;  //��ʱ��1������ģʽ2 | GATE	C/T M1 M0 | GATE C/T M1	M0 |
	//��ʱ��0������ģʽ1 |	Timer1	  |    Timer0	   |a
//���ڲ�������
	TH1=0xFD;   /*�趨������Ϊ19200������22.1184MHz*/
	TR1=1;		//������ʱ��1��ʼ����
	ES=1;	    //�������ж�
//��ʱ��0����
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xF8;		//���ö�ʱ��ֵ
	TR0=1;   	//������ʱ��0
	ET0=1;	    //����T0�ж�
//�ж�0����
	EX0=1;	  	//ʹ���ⲿ�ж�0
	IT0=1;		//�ⲿ�ж�0��ƽ��������ж�

//�ж�1����
//	IT1=1;      //�ⲿ�ж�1��ƽ��������ж�
	EX1=0;   	//��ֹ�ⲿ�ж�1
//���ȼ�����
	PS=1;	    //�趨�����ж�����
//�ж��ܿ�������
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

void delay ( u8 cnt ) //   ��ʱ��λ:
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
		iTask2= iTask_buf[mPosD][Even];		//�����¼���״̬�����ȡ����
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


	//***********************����״̬����***********************//

}

void iTaskControl()			//�ⲿ������
{
	ViceCtr();		//��������
	switch ( iTask )	//��ǰִ������
	{
		case ITASK_INT:
			BmRed();		//���������ȡ
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
			break;		//�����ر�����ʼ��
	}
}

void t0_interrupt ( void )  interrupt 1 //using 1
{
	TF0= 0;
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xF8;		//���ö�ʱ��ֵ


	T1Ms_clk();
	mCtrClk();
	ComClk();
}

/*****************************************************************************
 �� �� ��  : int0_interrupt
 ��������  : �ⲿ�ж�0�������ٶȴ�����
 �������  : void
             
 �������  : ��
 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��18��
    ��    ��   : �Ŷ�

    �޸�����   : �����ɺ���

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
