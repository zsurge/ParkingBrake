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



u8 code iTask_buf[7][18]= 	//�����
{/*  ���¼�  �ֶ���բ �ڲ���բ ͨ����բ �ظ���բ ��ˮƽλ ǿ��̧�� ǿ̧��λ �ظ�̧բ ����̧բ ����̧բ �ڲ�̧բ ͨ��̧բ ��Ƶ̧բ �ֶ�̧բ ����ֱλ ���Ե��� ���˸�λ        �¼�	*///
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* ״̬δ��ʼ�� */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_NA, TASK_UP, TASK_US, TASK_UP, TASK_NA,	/* ��ʼδ֪״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_US,	/* ����̧բ״̬ */
	TASK_NA, TASK_DW, TASK_DW, TASK_DW, TASK_DW, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA,	/* ��ֱ״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_DS, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA, 	/* ������բ״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_UP, TASK_UP, TASK_UP, TASK_UP, TASK_NA, TASK_NA, TASK_NA,	/* ˮƽ״̬ */
	TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_DS, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_NA, TASK_US, TASK_NA, TASK_NA,	/* ����״̬ */
};



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


//��ʱ��2��ʼ��
#if TIMER2
void Timer2_Init(void)
{
	T2MOD = 0;		//��ʼ��ģʽ�Ĵ���
	T2CON = 0;		//��ʼ�����ƼĴ���
	TL2 = (65536 - 50000) / 256;		//���ö�ʱ��ֵ 50000--50ms 9216---10ms
	TH2 = (65536 - 50000) % 256;		//���ö�ʱ��ֵ
	TR2 = 1;		//��ʱ��2��ʼ��ʱ


	//  TH2 = (65536-9216)/256;
	//  TL2 = (65536-9216)%256;
	EA=1;                      //�����ж�
	ET2=1;                     //�򿪶�ʱ���ж�  
	TR2=1;                     //�򿪶�ʱ������
}
#endif




void intRun() { iTask= ITASK_INT; iTask2= ITASK_NA; ViceDo= 0; ViceRal= 0; ViceRal2= 0; ViceSta= 0; UpCt=0; UpCtNum=0; }

void delay(u8 cnt)  //   ��ʱ��λ:
{
 u16 i;
 i=cnt*200; 
 while (--i);
}


void ComClk() 
	{ 
		if(ComNum) ComNum--;
		if(COM1.RX_TimeOut) COM1.RX_TimeOut--;
	}

void UpCtClk() { if(UpCtNum) UpCtNum--; }
void UpDcClk() { if(UpDcNum) UpDcNum--; }

void sysint()
{
	intMcu();	
	intUsart();
	#if TIMER2
	Timer2_Init();
	#endif
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

	if(iTask < Even)
		{
			iTask2= iTask_buf[mPosD][Even];		//�����¼���״̬�����ȡ����
			
			if(iTask2!= MTASK_NA) 
				{
					mTask= iTask2;
					iTask= Even;
					if(!JY) if(iTask== ITASK_DG_UP || iTask== ITASK_DRAG_UP)  UpCt++;
				}
		}
}


void ViceCtr()
{
	if(McuId== MACHI_VICE)
		{
			switch(ViceRal)
				{
					case POS_HOR:
					case POS_DW:
						Event(ITASK_COM_DW);
						speak(VICE_STA,mPosD2);
						break;
					case POS_UP:
					case POS_VER:
						HDwSta=STA_NA; HDwNum=HDW_ON_TIME;
						DgSta=STA_NA; DgNum=DG_OFF_TIME;
						Event(ITASK_COM_UP);
						speak(VICE_STA,mPosD2);
						break;
					case POS_INT:
						speak(VICE_STA,mPosD2);
						break;
					default: break;
				}
			ViceRal=0;
		}
	if(McuId== MACHI_MASTER)
		{	
			if(iTask== ITASK_INT || iTask== ITASK_STR) return;
			if(!ComNum) ComNum= COM_TIME;
			else return;
			switch(mPosD2)
				{
					case POS_HOR:
					case POS_DW:
						switch(ViceRal)
							{
								case POS_HOR:
									ViceSta=0;
									speak(VICE_STA,POS_INT);
									break;
								case POS_DW:
									if(!ViceSta) ViceSta=VICE_STA_EN;
									if(ViceSta==VICE_STA_DGON) 				speak(VICE_STA,POS_UP);
									else									speak(VICE_STA,POS_INT);
									break;
								case POS_VER:
									if(!ViceSta) ViceSta=VICE_STA_EN; 
									if(mPosD2==POS_DW)						speak(VICE_STA,POS_DW);
									if(ViceSta==VICE_STA_DW) 				speak(VICE_STA,POS_DW);
									else if(ViceSta==VICE_STA_DGOF) 		{ speak(VICE_STA,POS_DW); ViceSta=0; }
									else 									speak(VICE_STA,POS_INT);
									break;
								case POS_UP:
									if(ViceSta!=VICE_STA_DGON) 				DgStaClr();
									if(!ViceSta) 							ViceSta=VICE_STA_EN;
									if(mPosD2==POS_DW)						Event(ITASK_COM_UP);
									else if(ViceSta==VICE_STA_DGOF) 		speak(VICE_STA,POS_DW);
									else 									speak(VICE_STA,POS_INT);
									break;
								default: 
									speak(VICE_STA,POS_INT);
									break;
							}
						ViceRal=0;
						break;
					case POS_VER:
					case POS_UP:
						speak(VICE_STA,mPosD2);
						break;
					default: 
						speak(VICE_STA,mPosD2);
						break;
				}
		}
	

	//***********************����״̬����***********************//
	
}

void iTaskControl()			//�ⲿ������
{

	ViceCtr();		//��������
	switch(iTask)	//��ǰִ������
		{
			case ITASK_INT:
				BmRed();		//���������ȡ
				if(!HUP && !HDW && !HOR) AtNum= 30;
				else AtNum= 6;
				iTask=ITASK_STR;
				break;

			case ITASK_STR:
				if(!HUP && !HDW && !HOR && !AtNum) { aLine= 1;iTask= ITASK_NA; }
				else if(!AtNum) { iTask=ITASK_NA; }
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
				iTaskCtr();
				break;
				
			case ITASK_DG_UP:
			case ITASK_DRAG_UP:
			case ITASK_FZ_UP:
			case ITASK_AUTO_UP:
			case ITASK_COM_UP:
			case ITASK_VDO_UP:
			case ITASK_HAN_UP:
				iTaskCtr();
				break;
				
			case ITASK_HAN_DW:
			case ITASK_COM_DW:
			case ITASK_AUTO_DW:
			case ITASK_DG_DW:
				iTaskCtr();
				break;

				
			default: iTask=ITASK_NA; break;		//�����ر�����ʼ��
		}
}

void t0_interrupt(void)  interrupt 1 //using 1
{
	TF0= 0;
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xF8;		//���ö�ʱ��ֵ


	T1Ms_clk();
	mCtrClk();
	ComClk();
//	LED=!LED;
}




//	��բʱ��Ϊ5���׶Σ��ֱ���
//	1.�𲽽׶Σ���ֱλ����λ
//	2.���н׶Σ���λ���ж�
//	3.���ٽ׶Σ��жε�����λ
//	4.����׶Σ����嵽ƽ��λ
//	5.�ȶ��׶Σ�ƽ��λ��ˮƽλ
//	���˼·�ǣ����ݲ��Ե����ݣ��ó��𲽽׶κ����н׶ε�����Ҫ��ת������
//	Ȼ���������俪ʼ�ж������ݽ��붨ʱ���Ĵ������õ��µ�ת�����Լ��ܵ�ʱ��
//	���ʱ���ٸ��ݾ���ֵ���ж��ǲ��ǳ�������Ԥ�ڵ�ת�����䣬���ǣ���̧բ

//Ԥ�ڵ���բʱ��Ϊ5�룬����5�룬��Ϊ���з���

#if 0 
void testTimer2(void)
{	
	//�����ж��Ƿ��˼��ٽ׶�
	//30�Ǽ��赽���ٽ׶β��ٻ���ת����ʵ����Ҫȥ�ֶ��ļ���
	if(SpeRinN >= 30) //�ѵ����ٽ׶Σ�֮��Ҫ���ݲ��ٻ���Ȧ����ʱ���ж��Ƿ�ʧ��
	{
		if(gRepairMotor.CurrentCounts == 0)
		{
			gRepairMotor.InitSpeRin = SpeRinN;//��¼��ʼ����ǰ���ٻ���Ȧ��
		}
		
		gRepairMotor.CurrentCounts++;//�����жϵĴ����������жϵĴ����ж����õ�ʱ��
		
		//��Ϊ��10ms�Ķ�ʱ����3.8�ĵ��ÿ��������22.5ms�����н����ж�ÿ50ms������
		//�������ʹ�ã��ȹ̶�3.8�����releaseʱ��ʹ�ú궨�壬���岻ͬ�ļ��㷽ʽ���ж�����
		if((gRepairMotor.CurrentCounts != 0) && (gRepairMotor.CurrentCounts % 5 == 0))
		{
			if (gRepairMotor.FlagValue >= 4)
			{
				Event(ITASK_DG_UP);//Σ�գ�̧��
			}
			
			if(gRepairMotor.LastSpeRin !=0)
			{
				//��������ǿ�5U�����3U����ж����������8Ȧ������ȡ6Ȧ��Ϊ�ж�����
				if((SpeRinN - gRepairMotor.LastSpeRin) >= 4  /*&&����Ӧ�ø��ݵ�ѹ*/)
				{
					//���ʱ��ͱȽ�Σ����
					gRepairMotor.FlagValue++;
				}
			}

			gRepairMotor.LastSpeRin = SpeRinN;
			
			//��ӡ��ǰ���ٻ�ֵ
			Uart_Print(3,gRepairMotor.Direction);			

		}
	}
	
}

//��ʱ���ж�2
void Timer2_ISR(void) interrupt 5 //��ʱ��2�ж�
{
	TF2=0;
	EXF2 = 0;
	TL2 = (65536 - 9216) / 256;		//���ö�ʱ��ֵ
	TH2 = (65536 - 9216) % 256;		//���ö�ʱ��ֵ

	testTimer2();  
}
#endif 

#if 1 
#define TIMER_FRE  90 
#define BASIC_SPEED_100MS 8 //ÿ100ms���ٻ�ת����Ȧ��
#define SPRING_PARAM_OFFSET 2//���ɲ���
#define STANDARD_VALUE 72//���ɱ�׼ֵ


void motorTimer2(void)
{	
# if 1
    //ÿ��100��������ж�һ��
	if(SpeRinN >= 1) //���ٻ�ת���ſ�ʼ����
	{		
	    //�ӵ�900�����ʱ�������Ȼ��ÿ100�����ж�һ��
    	if(gRepairMotor.CurrentCounts++ >= TIMER_FRE);//�����жϵĴ����������жϵĴ����ж����õ�ʱ��
        {      
    		//�������ʹ�ã��ȹ̶�3.8�����releaseʱ��ʹ�ú궨�壬���岻ͬ�ļ��㷽ʽ���ж�����
    		
    		if(gRepairMotor.CurrentCounts % TIMER_FRE == 0)
    		{
                //���ж�̧բ
                if(gRepairMotor.Direction == POS_UP)
                {
                    if(gRepairMotor.CurrentCounts == TIMER_FRE)
                    {
                        if (SpeRinN > STANDARD_VALUE + SPRING_PARAM_OFFSET)
                        {
                            //��������
                            gCurrentSpringNum = 3;
                        }
                        else if(SpeRinN < STANDARD_VALUE + SPRING_PARAM_OFFSET && SpeRinN >= STANDARD_VALUE -SPRING_PARAM_OFFSET)
                        {
                            //�������ɣ���Ҫ����
                            gCurrentSpringNum = 2;
                        }
                        else
                        {
                            //ֻ��һ������
                            gCurrentSpringNum = 1;
                        }
                    }
                }
                else //��բ
                {
                    if(gCurrentSpringNum == 1 || SpeRinN >= STANDARD_VALUE + SPRING_PARAM_OFFSET*2-1)
                    {
                        //ִ��̧բ
                        Event(ITASK_DG_UP);
                    }
                
                }
                
    			//��ӡ��ǰ���ٻ�ֵ
    			Uart_Print(3,gRepairMotor.Direction);		
              }

		}
	}
#else 
	g_timer_run_counter++;
	if(g_timer_run_counter % TIMER_FRE == 0)
	{
		if(SpeRinN > FISTER_SPRING_BREAK && SpeRinN <= SECOND_SPRING_BREAK && g_timer_run_counter == 1)
		{
			g_timer_run_counter = 0;
			Event(ITASK_DG_UP);//Σ�գ�̧��
		}
		else if(SpeRinN > SECOND_SPRING_BREAK && SpeRinN <= THIRD_SPRING_BREAK && g_timer_run_counter == 2)
		{
			g_timer_run_counter = 0;
			Event(ITASK_DG_UP);//Σ�գ�̧��
		}
		else if(SpeRinN > THIRD_SPRING_BREAK && SpeRinN <= FORTH_SPRING_BREAK  && g_timer_run_counter == 3)
		{
			g_timer_run_counter = 0;
			Event(ITASK_DG_UP);//Σ�գ�̧��
		}
		else
		{
			g_timer_run_counter = 0;
		}

		//��ӡ��ǰ���ٻ�ֵ
		Uart_Print(3,gRepairMotor.Direction);
	}

	#endif
}

//��ʱ���ж�2
void Timer2_ISR(void) interrupt 5 //��ʱ��2�ж�
{
	TF2=0;
	EXF2 = 0;
	TL2 = (65536 - 9216) / 256;		//���ö�ʱ��ֵ
	TH2 = (65536 - 9216) % 256;		//���ö�ʱ��ֵ

	motorTimer2();  
}

//void Timer2_Zero(void)
//{	
//	TF2=0;
//	EXF2 = 0;	
//}

#endif


