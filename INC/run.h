/****************************************************************************
* File:			run.h														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__RUN_H
#define	__RUN_H

#include "reg52.h"
#include "clk.h"
#include "BUZZ.H"
#include "INPUT.H"
#include "MOTOR.H"
#include "AUTORUN.H"
#include "USART.h"
#include "ROMTEST.h"



/**************************************************************************** 
* �궨��
****************************************************************************/
#define		ENABLE		1
#define		DISABLE		0

#define     MAIN_Fosc   11059200UL    //������ʱ��HZ  


//������ͨ������
#define COM_TIME		20		


//ִ�е�����
#define TASK_NA		0		//������
#define TASK_UP		1		//̧բ
#define TASK_US		2		//̧բ����
#define TASK_DW		3		//��բ
#define TASK_DS		4		//��բ����



//�¼���Ӧ�����ȼ�(����Խ�����ȼ�Խ��)
#define ITASK_INT			255	//	�ⲿ�����ʼ��
#define ITASK_STR			254	//	�ⲿ�����ʼ��
#define ITASK_NA			0	//	������
#define ITASK_DG_UP			8	//	�ظ�̧բ
#define ITASK_FZ_UP			9	//	�ⲿ����̧բ
#define ITASK_DRAG_UP		10	//	����̧բ
#define ITASK_AUTO_UP		11	//	�ڲ�����̧բ
#define ITASK_COM_UP		12	//	ͨ��̧բ
#define ITASK_HAN_UP		14	//	�ֶ���ţ̌բ
#define ITASK_VDO_UP		13	//	��Ƶ̧բ
#define ITASK_DG_DW			4	//	�ظ���բ
#define ITASK_AUTO_DW		2	//	�ڲ�������բ
#define ITASK_COM_DW		3	//	ͨ����բ
#define ITASK_HAN_DW		1	//	�ֶ���ť��բ
#define ITASK_INER_DW		16	//	��ֱλ���Ե���
#define ITASK_COER_UP		6	//	ˮƽλǿ��̧��
#define ITASK_REACH_V		15	//	���ﴹֱλ
#define ITASK_REACH_H		5	//	����ˮƽλ
#define ITASK_INER_OK		17	//	���ﴹֱλ
#define ITASK_COER_OK		7	//	����ˮƽλ


//��������

#define VICE_STA			0x30

#define VICE_STA_EN			1
#define VICE_STA_DGON		2
#define VICE_STA_DGOF		3
#define VICE_STA_DW			4
#define VICE_STA_DRAG		5


#define UPCTTIME			150			//̧բ��������ʱ��
#define UPDCTIME			60			//̧բ������ʱ����




/**************************************************************************** 
* ʹ�ö˿ڶ���
****************************************************************************/
#define	PORT_LED			P2
#define	B_LED				4
sbit	LED					= PORT_LED ^ B_LED;




/**************************************************************************** 
* ��������/����
****************************************************************************/
extern volatile u8 xdata iTask;			//��ǰִ�е�����
extern volatile u8 xdata iTask2;		//��ǰִ�е�����
extern volatile u8 xdata ViceDo;		//����ִ������
extern volatile u8 xdata ViceRal;		//����ʵ������״̬
extern volatile u8 xdata ViceRal2;		//����ʵ������״̬
extern volatile u8 xdata ViceSta;		//��������״̬
extern volatile u8 data  ComNum;		//������ͨ�ż�ʱ
extern volatile u8 xdata UpCt;  		//̧բ����
extern volatile u8  data UpCtNum;  		//̧բ��������
extern volatile u8  data UpDcNum;  		//̧բ������ʱ����

/**************************************************************************** 
* ����ԭ��
****************************************************************************/
void intMcu();
void intRun();
void sysint();
void iTaskCtr();
void iTaskControl();
void ComClk();
void UpCtClk();
void UpDcClk();
void delay(u8 cnt);
//void delay_ms(u8 ms); 
#endif

