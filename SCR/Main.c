/****************************************************************************
* File:			main.c														*
* Created:		2017-04-25													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "main.h"

//�豸����汾��
u8 const code PG_Version[]= {VER0, VER1, VER2, YEAR, MOON, DATE};

//�洢��дʶ����
u8 const code eight= ( ( YEAR | MOON | DATE | BAIZHA )+DATE+3 );




/******************** ������**************************/
void main()
{
	sysint();
	while ( 1 )
	{
		clk();
		InputTask();
		iTaskControl();
		mTaskControl();
		MonitorTask();//���
		err_task();
		Buzz_task ( BuzzMode );		//����������
		OPutTask();
		uart_aly();
		AtRun();
		WDT_reset ( 7 );
	}

}

//void Test ( void )
//{
//    while (1)
//    {
//        delay_ms(250);
//        DZP_UP;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_CL;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_DW;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_CL;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_ER;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_UP
//        delay_ms(250);
//        delay_ms(250);
//        DZP_DW;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_CL;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_DW;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_UP;
//        delay_ms(250);
//        delay_ms(250);
//        DZP_CL;
//        delay_ms(250);
//    }
//}

