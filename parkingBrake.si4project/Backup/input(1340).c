/****************************************************************************
* File:			INPUT.c														*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "INPUT.H"

u8  data HupNum;			//
u8  data VupNum;			//
u8  data HDwNum;			//
u8  data DgNum_NoSignal;				//DG�����źŷ�����ʱ
u8  data DgNum_Signal;			//DG�����źŷ�����ʱ
u8  data FzNum;				//FZ�˷�����ʱ
u8  data PosNum;			//λ�ü���ʱ

u8 xdata HupSta;			//
u8 xdata VupSta;			//
u8 xdata HDwSta;			//
u8 xdata DgSta;				//�ظ�����״̬
u8 xdata FzSta;				//�ⲿ��������״̬

u8 xdata PosLin;			//λ�ü��˳��
u8 xdata Even;				//�¼�
u8 xdata EveNu;				//�¼�����

u8 xdata McuId;				//�豸��

volatile MOTOR_LIMIT_STRU gMotorLimit;  //�ж���ǰ�Ƿ���Ҫ̧բ


void intInput()
{
	HupNum=0;
	VupNum=0;
	HDwNum=0;
	DgNum_NoSignal=0;
	DgNum_Signal=0;
	FzNum=0;
	PosNum=0;
	HupSta=0;
	VupSta=0;
	HDwSta=0;
	DgSta=0;
	FzSta=0;
	PosLin=0;
	Even=0;
	EveNu=0;
	McuId= 0;
	DwSloBri= 0;
    memset(&gMotorLimit,0x00,sizeof(gMotorLimit));    
}

void Dg_clk()
{
	if ( DgNum_NoSignal )
	{
		DgNum_NoSignal--;
	}
	if ( DgNum_Signal )
	{
		DgNum_Signal--;
	}
}
void Fz_clk()
{
	if ( FzNum )
	{
		FzNum--;
	}
}
void Pos_clk()
{
	if ( PosNum )
	{
		PosNum--;
	}
}
void Hup_clk()
{
	if ( HupNum )
	{
		HupNum--;
	}
}
void Hdw_clk()
{
	if ( HDwNum )
	{
		HDwNum--;
	}
}
void Vup_clk()
{
	if ( VupNum )
	{
		VupNum--;
	}
}

/*****************************************************************************
 �� �� ��  : void SetLimit
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��4��19��
    ��    ��   : �Ŷ�
    �޸�����   : �޸Ĳ��뿪��������ĺ��壬ֻ��BM4 =0���������� =1 �����Ǵӻ���
              ������ÿ��ֵ����һ�ֻ�о��դ���˵Ĺ��BM1/BM2/BM3��=1ʱ����
              ʹ�ñ�������
*****************************************************************************/
void SetLimit()
{
    u8 value = 0;
    u8 bm1=0;
    u8 bm2=0;
    u8 bm3=0;

    if(BM1)
    {
        bm1 = 1;
    }
    if(BM2)
    {
        bm2 = 1;
    }
    if(BM3)
    {
        bm3 = 1;
    } 

    value = bm1<<2+bm2<<1+bm3;
    
    switch (value)
    {
        case FENCE_4M3:  
            gMotorLimit.Two_Spring_Max_Num = 85;
            gMotorLimit.Three_Spring_Offset_Sum = 8;
            gMotorLimit.Two_Spring_Offset_Sum = 32; 
            gMotorLimit.OverCnt = 2;
            Uart_Print(1,3);
            break;
        case FENCE_4M2:
            gMotorLimit.Two_Spring_Max_Num = 86;        //90;
            gMotorLimit.Three_Spring_Offset_Sum = 15;   //20;
            gMotorLimit.Two_Spring_Offset_Sum = 33;     //46; 
            gMotorLimit.OverCnt = 2;
            Uart_Print(1,2);
            break;
//        case FENCE_4M1:            
//            break;
        case FENCE_4M0:
            gMotorLimit.Two_Spring_Max_Num = 86;
            gMotorLimit.Three_Spring_Offset_Sum = 11;
            gMotorLimit.Two_Spring_Offset_Sum = 35;
            gMotorLimit.OverCnt = 2;
            Uart_Print(1,0);
            break;
//        case FENCE_3M8:            
//            break;
        case FENCE_3M5:
            gMotorLimit.Two_Spring_Max_Num = 86;
            gMotorLimit.Three_Spring_Offset_Sum = 12;
            gMotorLimit.Two_Spring_Offset_Sum = 35; 
            gMotorLimit.OverCnt = 2;   
            Uart_Print(1,5);
            break;
//        case FENCE_3M0:            
//            break;
        default:
            gMotorLimit.Two_Spring_Max_Num = 250;
            gMotorLimit.Three_Spring_Offset_Sum = 250;
            gMotorLimit.Two_Spring_Offset_Sum = 250;
            gMotorLimit.OverCnt = 250;
            break;
    }

    
}

void BmRed()		//���������ȡ
{ 
	if ( !BM4 )
	{
		McuId= MACHI_MASTER;
	}
	else
	{
		McuId= MACHI_VICE;
	}

/* BEGIN: Added by �Ŷ�, 2019/4/19 */
//���ﲻ��Ҫʹ�ò��뿪�������û���Ƕ�
//	if ( !BM1 )
//	{
//		DwSloBri|=1;
//	}
//    
//	if ( !BM2 )
//	{
//		DwSloBri|=2;
//	}
}


void Event ( u8 evn )
{
	if ( Even <= evn )
	{
		Even= evn;
		EveNu++;
	}
}


u8 posing()		//��λ���ط���--��0��1������
{
	switch ( PosLin )
	{
		case 0:
			if ( !VER && !HOR )
			{
				PosLin= 4;    //˵����̧�˻�����껺������ surge
			}
			else if ( !VER || !HOR )
			{
				PosLin=1;
				PosNum= POS_ON_TIME;
			}
			else
			{
				PosRal= POS_INT;
			}
			break;

		case 1:
			if ( !VER || !HOR )
			{
				if ( !PosNum )
				{
					PosLin= 2;
				}
			}
			else
			{
				PosNum= 0;
				PosLin= 0;
			}
			break;

		case 2:
			if ( !VER && !HOR )
			{
				PosLin= 4;
				break;
			}
			else if ( !VER )
			{
				PosRal= POS_VER;
				break;
			}
			else if ( !HOR )
			{
				PosRal= POS_HOR;
				break;
			}
			else
			{
				PosNum= POS_OF_TIME;
				PosLin= 3;
			}
			break;

		case 3:
			if ( VER && HOR )
			{
				if ( !PosNum )
				{
					PosLin= 0;
				}
			}
			else
			{
				PosLin= 2;
			}
			break;

		case 4:
			if ( VER || HOR )
			{
				PosLin= 0;
			}
			else
			{
				PosRal= POS_ERR;
			}
			break;

		default:
			PosLin=0;
			PosRal=0;
			break;
	}
	if ( PosRal== POS_ERR )
	{
		return ERR_POS_FAIL;
	}
	return 0;
}


void FZing()	//�����źŷ���--��1����
{
	switch ( FzSta )
	{
		case STA_NA:
			if ( !FZ )
			{
				FzSta=STA_ON;
				FzNum=FZ_OFF_TIME;
			}
			break;
		case STA_ON:
			if ( FZ )
			{
				if ( !FzNum )
				{
					FzSta=STA_NA;
				}
			}
			else
			{
				DgNum_NoSignal=FZ_OFF_TIME;
			}
			break;
		default:
			if ( !FZ )
			{
				FzSta=STA_ON;
				break;
			}
			else
			{
				FzSta=STA_NA;
				DgNum_NoSignal=FZ_OFF_TIME;
			}
			break;
	}
}

void DgStaClr()
{
	if ( DgSta== STA_KEEP )
	{
		DgSta= STA_NA;
	}
}


void DGing()		//�ظ��źŷ���--��1����
{
	/*
		switch(DgSta)
			{
				case STA_NA:
				case STA_KEEP:
					if(!DG) { DgSta= STA_ON; DgNum= DG_OFF_TIME; }
					else if(UpCt) { if(!UpDcNum) { UpCt=0; DgStaClr(); } }
					break;

				case STA_ON:
					if(DG)  { if(!DgNum) DgSta= STA_KEEP1; }
					else DgNum= DG_OFF_TIME;
					if(ViceSta==VICE_STA_EN || ViceSta==VICE_STA_DGOF) ViceSta=VICE_STA_DGON;
					UpDcNum=UPDCTIME;
					break;

				case STA_KEEP1:
					if(UpCt)	UpCt--;
					DgSta= STA_KEEP;
					if(ViceSta==VICE_STA_DGON) ViceSta=VICE_STA_DGOF;
					break;

				default:
					if(!DG) { DgSta=STA_ON; break; }
					else    { DgSta=STA_NA; DgNum=DG_OFF_TIME; }
					break;
			}
	*/
	switch ( DgSta )
	{
		case STA_NA:
		case STA_KEEP:
			if ( !DG )
			{
				DgSta= STA_ON;
				DgNum_NoSignal= DG_OFF_TIME;
				DgNum_Signal=DG_ON_TIME;
			}
			else if ( UpCt )
			{
				if ( !UpDcNum )
				{
					UpCt=0;
					DgStaClr();
				}
			}
			break;

		case STA_ON:
			if ( DG )
			{
				if ( DgNum_Signal )
				{
					DgSta=STA_NA;
				}
				else if ( !DgNum_NoSignal )  //����ط�����Ϊ��������Ҫ�޸� surge
				{
					DgSta= STA_KEEP1;
				}

                //����ҵ���߼�����д
//                if(DgNum_Signal)
//                {
//                    DgSta = STA_NA;
//                }
//                else
//                {
//                    if(!DgNum_NoSignal)
//                    {
//                        DgSta = STA_KEEP1;
//                    }
//                }                
			}
			else
			{
				DgNum_NoSignal= DG_OFF_TIME;
			}
			if ( ViceSta==VICE_STA_EN || ViceSta==VICE_STA_DGOF )
			{
				ViceSta=VICE_STA_DGON;
			}
			UpDcNum=UPDCTIME;
			break;

		case STA_KEEP1:
			if ( UpCt )
			{
				UpCt--;
			}
			DgSta= STA_KEEP;
			if ( ViceSta==VICE_STA_DGON )
			{
				ViceSta=VICE_STA_DGOF;
			}
			break;

		default:
			if ( !DG )
			{
				DgSta=STA_ON;
				break;
			}
			else
			{
				DgSta=STA_NA;
				DgNum_NoSignal=DG_OFF_TIME;
			}
			break;
	}
}

void HanUpEvent()	//�ֶ�̧բ�¼�--��0����
{
	switch ( HupSta )
	{
		case STA_NA:
			if ( !HUP )
			{
				if ( !HupNum )
				{
					HupSta= STA_KEEP;
				}
			}
			else
			{
				HupNum=HUP_ON_TIME;
			}
			break;
		case STA_ON:
			if ( HUP )
			{
				HupSta= STA_NA;
				HupNum= HUP_ON_TIME;
			}
			DgStaClr();
			Event ( ITASK_HAN_UP );
			break;
		case STA_KEEP:
			if ( !JY )
				if ( !UpCtNum )
				{
					UpCt++;
					UpCtNum= UPCTTIME;
				}
			HupSta= STA_ON;
			UpDcNum=UPDCTIME;
			break;
		default:
			HupSta= STA_NA;
			HupNum=HUP_ON_TIME;
			break;
	}
}

void HanDwEvent()						//�ֶ���բ�¼�
{
	if ( !HUP || !VUP )
	{
		HDwSta=STA_NA;
		return;
	}
	switch ( HDwSta )
	{
		case STA_NA:
			if ( !HDW )
			{
				if ( !HDwNum )
				{
					HDwSta= STA_KEEP;
				}
			}
			else
			{
				HDwNum= HDW_ON_TIME;
			}
			break;
		case STA_ON:
			if ( HDW )
			{
				HDwSta= STA_NA;
				HDwNum= HDW_ON_TIME;
			}
			break;
		case STA_KEEP:
			if ( McuId== MACHI_MASTER )
			{
				if ( ViceRal==POS_VER )
				{
					Event ( ITASK_HAN_DW );    //
				}
			}
			else
			{
				Event ( ITASK_HAN_DW );
			}
			HDwSta= STA_ON;
			UpCt=0;
			UpCtNum=0;
			UpDcNum=0;
			if ( McuId==MACHI_MASTER )
				if ( ViceRal!=POS_VER )
				{
					HDwSta=STA_NA;
					break;
				}
			if ( ViceSta==VICE_STA_EN )
			{
				ViceSta=VICE_STA_DW;
			}
			break;
		default:
			HDwSta= STA_NA;
			HDwNum= HDW_ON_TIME;
			break;
	}
}



void VdoUpEvent()	//��Ƶ̧բ�¼�--��0����
{
	switch ( VupSta )
	{
		case STA_NA:
			if ( !VUP )
			{
				if ( !VupNum )
				{
					VupSta= STA_KEEP;
				}
			}
			else
			{
				VupNum=VUP_ON_TIME;
			}
			break;
		case STA_KEEP:
			if ( !JY )
				if ( !UpCtNum )
				{
					UpCt++;
					UpCtNum= UPCTTIME;
				}
			VupSta= STA_ON;
			UpDcNum=UPDCTIME;
			break;
		case STA_ON:
			if ( VUP )
			{
				VupSta= STA_NA;
				VupNum=VUP_ON_TIME;
			}
			DgStaClr();
			Event ( ITASK_VDO_UP );
			break;
		default:
			VupSta= STA_NA;
			VupNum=VUP_ON_TIME;
			break;
	}
}

void AtUpEvent()						//�Զ�̧բ�¼�
{
	if ( Atctr== STA_ON )
	{
		DgStaClr();
		Event ( ITASK_AUTO_UP );
	}
}

void AtDwEvent()						//�Զ���բ�¼�
{
	if ( Atctr== STA_KEEP ) 				//
	{
		Event ( ITASK_AUTO_DW );    //
	}
}

void VerEvenr()							//��ֱ��λ�¼�
{
	if ( PosRal==POS_VER )
		if ( iTask >= ITASK_DG_UP && iTask <= ITASK_HAN_UP )
		{
			Event ( ITASK_REACH_V );    //����¼�
		}
}


void HorEvent()							//ˮƽ��λ�¼�
{
	if ( PosRal== POS_HOR ) 			//ʵ��λ��----��ˮƽλ
		if ( iTask==ITASK_DG_DW || iTask==ITASK_COM_DW || iTask==ITASK_AUTO_DW || iTask==ITASK_HAN_DW )
		{
			Event ( ITASK_REACH_H );    //����¼����ȼ�
		}
}

void DgUpEvent()						//�ظ�̧բ�¼�
{
	if ( DgSta== STA_ON )					//�ظ��ź�----��Ч
		if ( PosRal!= POS_HOR )			//ʵ��λ��----����ˮƽλ
			if ( mPosD== POS_DW 			//ִ��λ��״̬----����բ��
			        || mPosD== POS_VER )  	//����ִ��λ��״̬----�ڴ�ֱλ
			{
				Event ( ITASK_DG_UP );    //�ظ��ź���Ч�򱣳���--����ظ�̧բ�¼�
			}
}

void DgDwEvent()						//�ظ���բ�¼�
{
	if ( PosRal== POS_VER ) 				//λ��״̬��----��ֱλ
		if ( DgSta== STA_KEEP ) 			//�ظ���բ����----��Ч
			if ( !UpCt )
			{
				if ( McuId== MACHI_MASTER )
				{
					if ( ViceRal==POS_VER )
					{
						Event ( ITASK_DG_DW );    //�ظм�����բ��Ч--����ظ���բ�¼�
					}
				}
				else
				{
					Event ( ITASK_DG_DW );
				}
			}
}

void FzUpEvent()						//����̧բ�¼�
{
	if ( FzSta== STA_ON )					//�����ź�----��Ч
		if ( PosRal!= POS_HOR )			//ʵ��λ��----����ˮƽλ
			if ( mPosD== POS_DW			//ִ��λ��״̬----��բ��
			        || mPosD== POS_VER ) 	//����ִ��λ��״̬----�ڴ�ֱλ
			{
				Event ( ITASK_FZ_UP );    //�ظ��ź���Ч�򱣳���--����ظ�̧բ�¼�
			}
}

void InerOkEvt()
{
	if ( iTask== ITASK_INER_DW )
		if ( PosRal== POS_VER )
		{
			Event ( ITASK_INER_OK );
		}
}

void CoerOkEvt()
{
	if ( iTask== ITASK_COER_UP )
		if ( PosRal== POS_HOR )
		{
			Event ( ITASK_COER_OK );
		}
}




void InputTask()
{
	if ( iTask== ITASK_INT|| iTask== ITASK_STR )
	{
		return;    //�ⲿ����δ��ʼ�����
	}
	if ( posing() ) 	//��λ���ط���
	{
		err_volu ( ERR_POS_FAIL );
	}
	if ( UpCt )
	{
		if ( JY )
		{
			UpCt= 0;
			DgStaClr();
		}
	}
	FZing();		//�����źŷ���
	DGing();		//�ظ��źŷ���

	HanUpEvent();	//�ֶ�̧բ�¼�
	VdoUpEvent();	//��Ƶ̧բ�¼�
	AtUpEvent();	//�Զ�̧բ�¼�
	DgUpEvent();	//�ظ�̧բ�¼�
	FzUpEvent();	//����̧բ�¼�

	AtDwEvent();	//�Զ���բ�¼�
	HanDwEvent();	//�ֶ���բ�¼�
	DgDwEvent();	//�ظ���բ�¼�

	VerEvenr();		//��ֱ��λ�¼�
	HorEvent();		//ˮƽ��λ�¼�

	InerOkEvt();	//���˸�λ�¼�
	CoerOkEvt();	//ǿ̧��λ�¼�


	if ( !EveNu )
	{
		Even= 0;    //���¼��������¼�����
	}
	else
	{
		EveNu= 0;
	}
}





