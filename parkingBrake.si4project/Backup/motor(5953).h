/****************************************************************************
* File:			MOTOR.h														*
* Created:		2015-02-28													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__MOTOR_H
#define	__MOTOR_H


#include "run.H"
#include "INPUT.H"
#include "ERR.h"
#include "reg52.h"



/****************************************************************************
* �궨��
****************************************************************************/



//��ͨ�ǿ���/�������/�������λ��



//�������
#define MNA						5			//10 �������-��
#define MLN_OTH					MNA+1		//11 �������ʶ���쳣
#define MLN_DW					MLN_OTH+1	//12 ��������Զ��������1
#define MLN_UP					MLN_DW+1	//13 ��������Զ��������2
#define MTPYREAD				9			//
#define M1S						10			//14 1����(ת�ٱ�27:1)
#define M18S					18			//15 1.8����(ת�ٱ�53:1)
#define M38S					38			//16 3.8����(ת�ٱ�115:1)

//��������תʱ��
#define M_ON_TIME				10			//��������ӳ�ʱ��
#define M_OF_TIME				2			//����ر��ӳ�ʱ��
#define M_CV_UP_TIME			1000		//�������ת--�л�̧բʱ��
#define M_CV_DW_TIME			800			//�������ת--�л�̧բʱ��

//��������������ֵ
#define STP1S_REDUNDANCY_TIME	30		//
#define STP18S_REDUNDANCY_TIME	20		//
#define STP38S_REDUNDANCY_TIME	20		//
#define SLOWINT_REDUNDANCY_TIME	160		//170627 CZP�޸ģ����ͼ��١�����ʱ����Ϊ�����BUG
#define SLOW1S_REDUNDANCY_TIME	160		//170627 CZP�޸ģ����ͼ��١�����ʱ����Ϊ�����BUG
#define SLOW18S_REDUNDANCY_TIME	160		//170627 CZP�޸ģ����ͼ��١�����ʱ����Ϊ�����BUG
#define SLOW38S_REDUNDANCY_TIME	160		//170627 CZP�޸ģ����ͼ��١�����ʱ����Ϊ�����BUG
#define STMMAXTIME				300		//

//��ȡ���ٻ�ת�ٵ�����ָ��
#define STMBUF_AVER_SPE			1		//ÿ����բʱ���������ٻ�10Ȧ��ƽ�����ڣ�
#define STMBUF_CALC_SPE			2		//�ٶ��жϲ�����
#define STMBUF_CLR1				3		//�����ʱ-��ȡƽ��ֵ��������
#define STMBUF_CLR2				4		//�����ʱ-�ж�������������
#define STMBUF_POST				11		//���ٻ���ǰλ�û���
#define STMBUF_POIN				12		//д�����ָ��
#define STMBUF_DATA				13		//д���������
#define STMBUF_LEN				14		//���鳤��
#define STMBUF_LEN2				3		//���鳤��



//��������ʱ������
#define M1SRUNTIME				600		//1��������ʱ��		----��������4��
#define M18SRUNTIME				900		//1.8��������ʱ��		----��������6��
#define M38SRUNTIME				900		//3.8��������ʱ��		----��������9��
#define MAXRUNTIME				1200	//�������ʱ��			----��������20��

//С�������ʱ������
#define M_LIT_UP_TIME			30		//
#define M_LIT_DW_TIME			20		//



//�����բ��ǰ����Ȧ������
#define SLO_1S_RIN				56		//
#define SLO_18S_RIN				95		//
#define SLO_38S_RIN				106		//
#define SLO_MAX_RIN				106		//



//�������״̬
#define MTASK_NA				0		//
#define MTASK_UP				1		//
#define MTASK_US				2		//
#define MTASK_DW				3		//
#define MTASK_DS				4		//
#define MTASK_UP1				11		//
#define MTASK_UP2				12		//
#define MTASK_UP3				13		//
#define MTASK_UP4				14		//
#define MTASK_US1				21		//
#define MTASK_US2				22		//
#define MTASK_US3				23		//
#define MTASK_US4				24		//
#define MTASK_US5				25		//
#define MTASK_DW1				31		//
#define MTASK_DW2				32		//
#define MTASK_DW3				33		//
#define MTASK_DW4				34		//
#define MTASK_DS1				41		//
#define MTASK_DS2				42		//
#define MTASK_DS3				43		//
#define MTASK_DS4				44		//
#define MTASK_DS5				45		//



//���λ��״̬
#define POS_INT					1			//��ʼδ֪	
#define POS_UP					2			//����̧բ	
#define POS_VER					3			//��ֱλ	
#define POS_DW					4			//������բ	
#define POS_HOR					5			//ˮƽλ
#define POS_ERR					6			//����λ��

//���������������
#define SPRI_RED_PARA			1			//��ȡ���ɲ���
#define SPRI_LOG_PARA			2			//�������������ж�
#define SPRI_LOG_PARA1			3			//�������������ж�
#define SPRI_CLR_PARA			4			//����������������



//��������״̬
#define SPRIFRCSTA_CLR			0			//������������
#define SPRIFRCSTA_ERR			1			//���������쳣
#define SPRIFRCSTA_LIT			2			//��������ƫС
#define SPRIFRCSTA_MID			3			//��������ƫ��
#define SPRIFRCSTA_BIG			4			//��������ƫ��

//������������ָ��
#define SPRIBUF_FST_AVER		12			//ǰ��6�������ƽ��ֵ
#define SPRIBUF_END_AVER		13			//����6�������ƽ��ֵ
#define SPRIBUF_FST_MIN			14			//ǰ��6���������Сֵ
#define SPRIBUF_END_MAX			15			//����6����������ֵ
#define SPRIBUF_DAT				16			//������
#define SPRIBUF_POIN			17			//ָ��
#define SPRIBUF_STA				18			//��������״̬



//���ʹ��
#define M_EN					0			//���
#define M_NA					1			//�����


//�̵�����������
#define BIG_MCR_UP					1			//
#define BIG_MCR_DW					0			//
#define LIT_MCR_UP					0			//
#define LIT_MCR_DW					1			//

#define MCR_UP		{BigMCR=BIG_MCR_UP; LitMCR=LIT_MCR_UP;  }			//
#define MCR_DW		{BigMCR=BIG_MCR_DW; LitMCR=LIT_MCR_DW;  }			//



#define M_CLOS 		{ BigM5u= M_NA; BigM3u= M_NA; LitM= M_NA;}			//
#define M_OPEN 		{ BigM5u= M_EN; BigM3u= M_EN; }						//
#define M_SLOW	 	{ BigM5u= M_NA; BigM3u= M_EN; LitM= M_EN; }			//

//������
#define START_POSITION 35   //�ӵ�35�ν����жϿ�ʼ����
#define END_POSITION (START_POSITION+TIMER_FRE*SPRING_CHECK_NUM ) //�ӵ�50�ν����жϿ�ʼ����
#define TIMER_FRE  5        //��������
#define BASIC_SPRING_NUM 39   //��ʼֵ
#define BASIC_OFFSET_250MS 6 //ÿ250ms���ٻ�ת����Ȧ��
#define SPRING_CHECK_NUM 8//ȡ������

#ifdef GAN_CHANG_4M2
#define TWO_SPRING_MIN_NUM 90   //���������ڵ�8�β�ѯ��ʱ��������90ת
#define THREE_SPRING_LOWER_LIMIT 20 //4.2��Ϊ20 //4.0��Ϊ11//���ͳ���ܵ�Ȧ����������ֵ��˵���Ѿ���������������
#define TWO_SPRINT_LOWER_LIMIT 46//4.2��ֵΪ 46 //4.0��ֵΪ28//���ͳ���ܵ�Ȧ����������ֵ��˵���Ѿ���������������
#else
#define TWO_SPRING_MIN_NUM 86   //���������ڵ�8�β�ѯ��ʱ��������90ת
#define THREE_SPRING_LOWER_LIMIT 11 //4.2��Ϊ20 //4.0��Ϊ11//���ͳ���ܵ�Ȧ����������ֵ��˵���Ѿ���������������
#define TWO_SPRINT_LOWER_LIMIT 28//4.2��ֵΪ 46 //4.0��ֵΪ28//���ͳ���ܵ�Ȧ����������ֵ��˵���Ѿ���������������
#endif


typedef struct 
{
    u8 OffsetSum;       //���ߵ�Ȧ����
	u8 Times;           //��ʱ������
	u8 CurrentCounts;   //ͳ�ƴ���
	u8 CalcCounts;      //�������
    u8 ExFlag;          //�Ƿ�ִ�ж�ʱ���ı�־λ
	u8 Direction;       //���ת������	
	u8 FlagValue;       //ͳ�Ƴ��޴���	
	u8 OutFlag;         //��ƽ�ź�ת��
//	u8 BasicSpringNum;  //��һ������λ��
//	u8 CurrentMotorType;//��ǰ�������
//	u8 CurrentSprintType;//��ǰ��������
//	u8 CurrentMotorPosition;//��ǰ���λ��
//  u8 CurrentSpeRin;   //��ǰ���ٻ�ת��Ȧ��
//  u8 AverageValue;    //������Ȧ��ƽ��ֵ    
}RepairMotor_t;



/**************************************************************************** 
* ʹ�ö˿ڶ���
****************************************************************************/
#define	PORT_BTA1			P2
#define	B_BTA1				7
sbit	BTA1				= PORT_BTA1 ^ B_BTA1;

#define	PORT_BTA2			P2
#define	B_BTA2				6
sbit	BTA2				= PORT_BTA2 ^ B_BTA2;

#define	PORT_BTA3			P2
#define	B_BTA3				5
sbit	BTA3				= PORT_BTA3 ^ B_BTA3;

#define	PORT_MCR1			P2
#define	B_MCR1				3
sbit	MCR1				= PORT_MCR1 ^ B_MCR1;

#define	PORT_MCR2			P0
#define	B_MCR2				7
sbit	MCR2				= PORT_MCR2 ^ B_MCR2;

#define	PORT_SPE			P3
#define	B_SPE				2
sbit	SPE					= PORT_SPE ^ B_SPE;


#define BigM5u			BTA1
#define BigM3u			BTA2
#define LitM			BTA3
#define BigMCR			MCR1
#define LitMCR			MCR2


/**************************************************************************** 
* ��������/����
****************************************************************************/
extern volatile u8  xdata mType;	//������� 1��    1.8��    3.8�� 
extern volatile u8  xdata mPosD;	//ִ��λ��״̬
extern volatile u8  xdata mPosD2;	//ִ��λ��״̬����������������
extern volatile u8  xdata PosRal;	//ʵ��λ��״̬
extern volatile u8  xdata mTask;	//�����ǰִ�е�����
extern volatile u8  xdata SpriBuf[19];//���ɼ������
extern volatile u16 data  mCorNum;	//�������ת�л���ʱ
extern volatile u16  data  mRunNum;	//�������ʱ������
extern volatile u16 xdata mRunTe;	//�������ʱ�仺��

extern volatile u16 data  StmSp;	//����������--ȫ����բʱ��ֵ
extern volatile u16 data  StmSp2;	//����������--ȫ����բʱ����
extern volatile u16 xdata StmSlow;	//����������--��բ����ʱ��ֵ
extern volatile u16 xdata StmSlow2;	//����������--��բ����ʱ����
extern volatile u8  xdata Stm;		//����������--����
extern volatile u16 data  StmNum;	//�����ʱ
extern volatile u16 data  StmNum2;	//�����ʱ����
extern volatile u16 data  StmNum3;	//�����ʱ����
extern volatile u8  data  StmNumBuf[STMBUF_LEN];//�����ʱ-��ȡƽ��ֵ����
extern volatile u8  data  StmNumBuf2[STMBUF_LEN2];//�����ʱ-�ж���������

extern volatile u8 data  SpeRinN;	//���ٻ�ת��Ȧ������
extern volatile u8 xdata DwSlo;		//��ʼ����Ȧ������(����ʼ����LitM= M_EN;)
extern volatile u8 xdata DwSloBri;	//��ʼ����Ȧ������(����ʼ����BigM5u=M_NA )

extern volatile bit SpeSta;			//���ٻ���Ƚϱ�־

extern volatile RepairMotor_t gRepairMotor;  //�ж���ǰ�Ƿ���Ҫ̧բ







/**************************************************************************** 
* ����ԭ��
****************************************************************************/
void intMotor();
void mRunClk();
void mCtrClk();
void mTaskControl();	
void InitRepairMotor(void);

void MonitorTask(void);
void Monitor_clk(void);		//��ؼ���




#endif

