/****************************************************************************
* File:			OUTPUT.h													*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__OUTPUT_H
#define	__OUTPUT_H

#include "reg52.h"
#include "MOTOR.H"


/**************************************************************************** 
* �궨��
****************************************************************************/
#define	OPUT_UP		1			//̧բ
#define	OPUT_DW		3			//��բ



#define	DZP_TIME	10			//��բ�����ź�ʱ��

#define	DLYBRAKETIME	20			// �ӳ�ɲ��ʱ��
#define	BRAKETIME		300			//ɲ��ʱ��



#define	DZP_ON		0			//��բ�����ź�״̬
#define	DZP_NA		1			//��բ�����ź�״̬


#define	DZP_UP		{ DZP1=DZP_ON; DZP0= DZP_NA; }			//��բ̧բ����
#define	DZP_DW		{ DZP1=DZP_NA; DZP0= DZP_ON; }			//��բ��բ����
#define	DZP_CL		{ DZP1=DZP_NA; DZP0= DZP_NA; }			//��բ��������



#define	KK_GRE		0			//�̵���״̬
#define	KK_RED		1			//�̵���״̬


/**************************************************************************** 
* ʹ�ö˿ڶ���
****************************************************************************/
#define	PORT_KK				P1
#define	B_KK				5
sbit	KK					= PORT_KK ^ B_KK;

#define	PORT_DZP0			P3
#define	B_DZP0				6
sbit	DZP0				= PORT_DZP0 ^ B_DZP0;

#define	PORT_DZP1			P3
#define	B_DZP1				7
sbit	DZP1				= PORT_DZP1 ^ B_DZP1;

#define	PORT_RG				P1
#define	B_RG				0
sbit	RG					= PORT_RG ^ B_RG;






/**************************************************************************** 
* ��������/����
****************************************************************************/
extern volatile	u8 data DZPnum;
extern volatile	u8 data DlyBrakeNum;
extern volatile	u16 data BrakeNum;
extern volatile	u8 data BrakeSta;







/**************************************************************************** 
* ����ԭ��
****************************************************************************/
void intOut();
void DZP_clk();
void BrakeClk();
void OPutTask();
void BrakeClr();







#endif


