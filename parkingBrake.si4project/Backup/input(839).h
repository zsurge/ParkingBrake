/****************************************************************************
* File:			INPUT.h														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__INPUT_H
#define	__INPUT_H

#include "reg52.h"
#include "ERR.h"
#include "MOTOR.H"
#include "RUN.H"
#include "OUTPUT.H"
#include "AUTORUN.H"




/**************************************************************************** 
* �궨��
****************************************************************************/

//*************����ʱ��*******************//
#define HUP_ON_TIME					10			//�ֶ�̧բ�ź���Ч����ʱ��
#define VUP_ON_TIME					10			//��Ƶ̧բ�ź���Ч����ʱ��
#define HDW_ON_TIME					10			//�ֶ���բ�ź���Ч����ʱ��

//�ظ��ź���Ч����ʱ��
#define DG_ON_TIME					20
#define DG_OFF_TIME					5			//�ظ��ź���Ч����ʱ��
#define FZ_OFF_TIME					20			//�����ź���Ч����ʱ��
#define POS_ON_TIME					2			//λ���ź���Ч����ʱ��
#define POS_OF_TIME					20			//λ���ź���Ч����ʱ��


//*************�����ź�״̬******************//
#define	STA_NA						0			//--���ź�
#define	STA_ON						1			//--���ź�
#define	STA_KEEP					2			//--���ź����뱣��
#define	STA_KEEP1					3			//--���ź����뱣��


//***************�豸����******************//
#define	MACHI_VICE					2			//����
#define	MACHI_MASTER				1			//����


//***************�豸���******************//
#define FENCE_4M3                        0
#define FENCE_4M2                        1
#define FENCE_4M1                        2
#define FENCE_4M0                        3
#define FENCE_3M8                        4
#define FENCE_3M5                        5
#define FENCE_3M0                        6




/**************************************************************************** 
* ʹ�ö˿ڶ���
****************************************************************************/
#define	PORT_HUP			P3
#define	B_HUP				3
sbit	HUP 				= PORT_HUP ^ B_HUP;
			
#define	PORT_HDW			P2
#define	B_HDW				0
sbit	HDW					= PORT_HDW^ B_HDW;

#define	PORT_DG				P1
#define	B_DG				7
sbit	DG					= PORT_DG ^ B_DG;

#define	PORT_FZ				P1
#define	B_FZ				3
sbit	FZ					= PORT_FZ ^ B_FZ;

#define	PORT_HOR			P2
#define	B_HOR				2
sbit	HOR					= PORT_HOR ^ B_HOR;

#define	PORT_VER			P3
#define	B_VER				4
sbit	VER					= PORT_VER ^ B_VER;

#define	PORT_VUP			P1
#define	B_VUP				4
sbit	VUP					= PORT_VUP ^ B_VUP;

#define	PORT_JY				P2
#define	B_JY				1
sbit	JY					= PORT_JY ^ B_JY;

#define	PORT_BM1			P0
#define	B_BM1				3
sbit	BM1					= PORT_BM1 ^ B_BM1;

#define	PORT_BM2			P0
#define	B_BM2				4
sbit	BM2					= PORT_BM2 ^ B_BM2;

#define	PORT_BM3			P0
#define	B_BM3				5
sbit	BM3					= PORT_BM3 ^ B_BM3;

#define	PORT_BM4			P0
#define	B_BM4				6
sbit	BM4					= PORT_BM4 ^ B_BM4;




/**************************************************************************** 
* ��������/����
****************************************************************************/
extern volatile u8  data HupNum;			//
extern volatile u8  data VupNum;			//
extern volatile u8  data HDwNum;			//
extern volatile u8  data DgNum_NoSignal;    //DG�����źŷ�����ʱ
extern volatile u8  data DgNum_Signal;   	//DG�����źŷ�����ʱ
extern volatile u8  data FzNum;				//FZ�˷�����ʱ
extern volatile u8  data PosNum;			//λ�ü���ʱ

extern volatile u8 xdata HupSta;			//
extern volatile u8 xdata VupSta;			//
extern volatile u8 xdata HDwSta;			//
extern volatile u8 xdata DgSta;				//�ظ�����״̬
extern volatile u8 xdata FzSta;				//�ⲿ��������״̬

extern volatile u8 xdata PosLin;			//λ�ü��˳��
extern volatile u8 xdata Even;				//�¼�
extern volatile u8 xdata EveNu;				//�¼�����
extern volatile u8 xdata McuId;				//�豸��

typedef struct MOTOR_LIMIT
{
    u8 Two_Spring_Max_Num;//��8�μ�¼�����ֵ����������ֵ��˵�����ɵ�������
    u8 Three_Spring_Offset_Sum;//3������ƫ�����ͣ�������ֵ��˵����������
    u8 Two_Spring_Offset_Sum;//2������ƫ�����ͣ�������ֵ��˵����������   
    u8 OverCnt;              //����һ���������ڵĴ�����Ҳ����ÿһ�μ���ƫ��������6�Ĵ�����������ֵ��˵����������
}xdata MOTOR_LIMIT_STRU;

extern volatile MOTOR_LIMIT_STRU gMotorLimit;  //�ж���ǰ�Ƿ���Ҫ̧բ







/**************************************************************************** 
* ����ԭ��
****************************************************************************/
void intInput();
void Event(u8 evn);
void DgStaClr();
void Dg_clk();
void Fz_clk();
void Pos_clk();
void Hup_clk();
void Hdw_clk();
void Vup_clk();
void BmRed();
void InputTask();













#endif


