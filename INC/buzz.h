/****************************************************************************
* File:			buzz.h														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef _buzz_H
#define _buzz_H

#include "reg52.h"



/**************************************************************************** 
* �궨��
****************************************************************************/		
#define BUZZ_EN			0			//������--��
#define BUZZ_CL			1			//������--��


//**************���������췽ʽ*******************//
#define BUZZ_NA					1			//������
#define BUZZ_B					2			//��(ֻ��һ�������ڰ�������)
#define BUZZ_BB					3			//����(ֻ�����������ڰ�������)
#define BUZZ_BBB				11			//����(ֻ�����������ڰ�������)
#define BUZZ_BB_B				12			//��(����1��)~~~��
#define BUZZ_BB_B_B				13			//��(����1��)~~~��~~~��
#define BUZZ_BB_B_B_B			14			//��(����1��)~~~��~~~��~~~��
#define BUZZ_BB_BB				15			//��(����1��)~~~��(����1��)

#define BUZZ_B02				4			//��~~~200����	
#define BUZZ_B2					5			//��~~~~2��Ƶ��
#define BUZZ_BB2				6			//����~~~2��Ƶ��
#define BUZZ_BBB2				7			//������~~~~2��Ƶ��
#define BUZZ_B_B				8			//��(����1��)~~~~��
#define BUZZ_B_BB				9			//��(����1��)~~~~����
#define BUZZ_B_BBB				10			//��(����1��)~~~~������
//**************����������ʱ��*******************//
#define BUZZ_MOTOR_T			220			//�������������ʱ��
#define BUZZ_OTH_T				220			//������������ʱ��
#define BUZZ_INF_T				50			//δˢ���ڸ�Ӧ������ʱ��
#define BUZZ_FOLOWERME_T		60			//β������ʱ��
#define BUZZ_LOOTRUN_T			20			//��������ʱ��
#define BUZZ_STOP1_T			250			//����ͣ������1ʱ��(���������)
#define BUZZ_STOP2_T			250			//����ͣ������2ʱ��





/**************************************************************************** 
* ʹ�ö˿ڶ���
****************************************************************************/
#define	PORT_BUZZ			P1
#define	B_BUZZ				1
sbit	BUZZ				= PORT_BUZZ ^ B_BUZZ;




/**************************************************************************** 
* ��������/����
****************************************************************************/
extern volatile u8 xdata BuzzMode;
extern volatile u8 data BuzzNum;



/**************************************************************************** 
* ����ԭ��
****************************************************************************/
void Buzz_task(u8 mode);
void Buzz_clk();
void intBuzz();

#endif