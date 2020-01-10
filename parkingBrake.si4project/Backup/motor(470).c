
/****************************************************************************
* File:			motor.c														*
* Created:		2017-06-05													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:	�������													*
****************************************************************************/

#include "MOTOR.H"

u8  xdata mType;	//������� 1��    1.8��    3.8��
u8  xdata mPosD;	//ִ��λ��״̬
u8  xdata mPosD2;	//ִ��λ��״̬����������������
u8  xdata PosRal;	//ʵ��λ��״̬
u8  xdata mTask;	//�����ǰִ�е�����
u8  xdata SpriBuf[19];//���ɼ������
u16 data  mCorNum;	//�������ת�л���ʱ
u16  data  mRunNum;	//�������ʱ������
u16  xdata mRunTe;	//�������ʱ�仺��

u16 data  StmSp;	//����������--ȫ����բʱ��ֵ
u16 data  StmSp2;	//����������--ȫ����բʱ����
u16 xdata StmSlow;	//����������--��բ����ʱ��ֵ
u16 xdata StmSlow2;	//����������--��բ����ʱ����
u8  xdata Stm;		//����������--����
u16 data  StmNum;	//�����ʱ
u16 data  StmNum2;	//�����ʱ����
u16 data  StmNum3;	//�����ʱ����
u8  data  StmNumBuf[STMBUF_LEN];//�����ʱ-��ȡƽ��ֵ����
u8  data  StmNumBuf2[STMBUF_LEN2];//�����ʱ-�ж���������

u8 data  SpeRinN;	//���ٻ�ת��Ȧ������
u8 xdata DwSlo;		//��ʼ����Ȧ������(����ʼ����LitM= M_EN;)
u8 xdata DwSloBri;	//��ʼ����Ȧ������(����ʼ����BigM5u=M_NA )

bit SpeSta;			//���ٻ���Ƚϱ�־


volatile RepairMotor_t gRepairMotor;  //�ж���ǰ�Ƿ���Ҫ̧բ






void intMotor()
{
	mType= MNA;
	mPosD= POS_INT;
	mPosD2= POS_INT;
	PosRal= POS_INT;
	mTask= MTASK_NA;
	StmNum= 0;
	StmNum2= 0;
	StmNum3= 0;
	mCorNum= 0;
	mRunNum= 0;
	SpeRinN= 0;
	memset ( StmNumBuf,0,sizeof ( StmNumBuf ) );
	memset ( StmNumBuf2,0,sizeof ( StmNumBuf2 ) );
	memset ( &SpriBuf,0,sizeof ( SpriBuf ) );
	StmSp= 0;
	StmSp2= 0;
	mRunTe= 0;
	DwSlo= 0;
	Stm= 0;
	StmSlow= 0;
	StmSlow2= 0;
	SpeSta= 0;
	InitRepairMotor();
}

void InitRepairMotor ( void )
{
	memset ( &gRepairMotor,0x00,sizeof ( gRepairMotor ) );
}



void mRunClk()
{
	if ( mRunNum )
	{
		mRunNum--;    //�������ʱ��ʱ��
	}
}
void mCtrClk()
{
	if ( mCorNum )
	{
		mCorNum--;    //�������ת�л�ʱ��   //����ʱ��
	}
	else
	{
		StmNum++;
	}
}




//**********************************************//
//������:void StmInt(u8 type)
//����:���������ȵ��������
//����:type-��о����
//����: ��
//**********************************************//
void StmInt ( u8 type )
{
	if ( type== M1S )
	{
		StmSp= StmSp2 + STP1S_REDUNDANCY_TIME;			//������բʱ������������ = ʵ����ٻ���Ȧ������ + �û�о������ֵ
//			StmSlow2= StmSp2 + SLOW1S_REDUNDANCY_TIME;		//���١�����ʱ������������ = ������բʱ������������ * 3
		StmSlow2= SpriBuf[SPRIBUF_END_MAX] + 60;
	}
	else if ( type== M18S )
	{
		StmSp= StmSp2 + STP18S_REDUNDANCY_TIME;
//			StmSlow2= StmSp2 + SLOW18S_REDUNDANCY_TIME;
		StmSlow2= SpriBuf[SPRIBUF_END_MAX] + 60;

	}
	if ( StmSlow2 > STMMAXTIME )
	{
		StmSlow2= STMMAXTIME;    //���١�������������������Ʋ���
	}
}





//**********************************************//
//������:void KnowMtyp(u8 sta,u8 Prg,u8 pos)
//����:��ʼ���١�����ļ��㡢��ֵ
//����:bm-���벹��������  type-��ͬ��о���Ͳ�������
//����: ��
//**********************************************//
void DwSloInt ( u8 bm,u8 type )
{
	DwSloBri*=bm; 					//����ı�������
	DwSloBri+=type; 				//��ǰ���ٵ�Ȧ��= ���벹��Ȧ�� + ��о����Ȧ��
	DwSlo= SpeRinN - DwSloBri/3*2;	//��ʼ�����Ȧ��= ���ٻ���Ȧ�� - ��ǰ����Ȧ����2/3
	DwSloBri= SpeRinN - DwSloBri;	//��ʼ���ٵ�Ȧ��= ���ٻ���Ȧ�� - ��ǰ����Ȧ��
}


//**********************************************//
//������:  SpringTest(u8 cmd)
//����:�����������
//����:cmd-��������
//����:��
//**********************************************//
u8 SpringTest ( u8 cmd )
{
	u16 data dat=0;
	switch ( cmd )
	{
		case SPRI_RED_PARA:
			if ( SpeRinN!= SpriBuf[SPRIBUF_DAT] )
			{
				SpriBuf[SPRIBUF_DAT]= SpeRinN;
				SpriBuf[SpriBuf[SPRIBUF_POIN]]= StmNum2;
				SpriBuf[SPRIBUF_POIN] ++;
				if ( SpriBuf[SPRIBUF_POIN]== 12 )
				{
					SpriBuf[SPRIBUF_POIN]= 6;
				}
			}
			break;

		case SPRI_LOG_PARA:
			//********��ǰ6Ȧ��ƽ��ֵ ����Сֵ*************//
			SpriBuf[SPRIBUF_FST_MIN]= 255;
			for ( SpriBuf[SPRIBUF_POIN]=0; SpriBuf[SPRIBUF_POIN] < 6; SpriBuf[SPRIBUF_POIN]++ )
			{
				dat+= SpriBuf[SpriBuf[SPRIBUF_POIN]];//ǰ6�ۼ�
				if ( SpriBuf[SpriBuf[SPRIBUF_POIN]] < SpriBuf[SPRIBUF_FST_MIN] )
				{
					SpriBuf[SPRIBUF_FST_MIN]= SpriBuf[SpriBuf[SPRIBUF_POIN]];    //��ȡ��Сֵ
				}
			}
			dat/= 6;
			SpriBuf[SPRIBUF_FST_AVER]= dat;

			//********���6Ȧ��ƽ��ֵ�����ֵ*************//
			SpriBuf[SPRIBUF_END_MAX]= 0;
			for ( SpriBuf[SPRIBUF_POIN]=6; SpriBuf[SPRIBUF_POIN] < 12; SpriBuf[SPRIBUF_POIN]++ )
			{
				dat+= SpriBuf[SpriBuf[SPRIBUF_POIN]];
				if ( SpriBuf[SpriBuf[SPRIBUF_POIN]] > SpriBuf[SPRIBUF_END_MAX] )
				{
					SpriBuf[SPRIBUF_END_MAX]= SpriBuf[SpriBuf[SPRIBUF_POIN]];
				}
			}
			dat/= 6;
			SpriBuf[SPRIBUF_END_AVER]= dat;

			//********�ж���������*************//
			if ( ( SpriBuf[SPRIBUF_FST_AVER] > 20 && SpriBuf[SPRIBUF_FST_AVER] < 80 ) || SpriBuf[SPRIBUF_END_AVER] > 220 )
			{
				if ( SpriBuf[SPRIBUF_END_MAX] > 190 )
				{
					SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_ERR;    //�����쳣
					break;
				}

				else if ( SpriBuf[ SPRIBUF_END_MAX] > 150 )
				{
					SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_BIG;    //����ƫ��
					break;
				}

				else if ( SpriBuf[SPRIBUF_END_MAX] > 110 )
				{
					SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_MID;    //�����е�
					break;
				}

				else if ( SpriBuf[SPRIBUF_END_MAX] > 90 )
				{
					SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_LIT;    //����ƫС
					break;
				}

				else
				{
					SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_ERR;
				}
			}
			else
			{
				SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_ERR;
			}
			break;

		case SPRI_LOG_PARA1:
			SpriBuf[SPRIBUF_END_MAX]= 0;
			for ( SpriBuf[SPRIBUF_POIN]=0; SpriBuf[SPRIBUF_POIN] < 12; SpriBuf[SPRIBUF_POIN]++ )
			{
				if ( SpriBuf[SpriBuf[SPRIBUF_POIN]] > SpriBuf[SPRIBUF_END_MAX] )
				{
					SpriBuf[SPRIBUF_END_MAX]= SpriBuf[SpriBuf[SPRIBUF_POIN]];
				}
			}
			break;

		case SPRI_CLR_PARA:
			memset ( &SpriBuf,0,sizeof ( SpriBuf ) );
			break;

		default:
			break;
	}
	return SpriBuf[SPRIBUF_STA];
}



//**********************************************//
//������:void KnowMtyp(u8 sta,u8 Prg,u8 pos)
//����:ʶ���о���ͣ�����ֵ���в���
//����:sta-����״̬��  prg-ʶ����̣�  pos-����λ��
//����: ��
//**********************************************//
void KnowMtyp ( u8 sta,u8 Prg,u8 pos )	//��о����ʶ�𼰲�����ֵ
{
	if ( sta== MNA )					//բ�˵�λ��׼���жϻ�о����
	{
		if ( mType== Prg )			//ʶ�����
			if ( PosRal== pos )	//����λ��
			{
				switch ( SpringTest ( SPRI_LOG_PARA ) )
				{
					case SPRIFRCSTA_BIG:
						err_volu ( ERR_SPRI_BIG );
						break;
					case SPRIFRCSTA_LIT:
						err_volu ( ERR_SPRI_LIT );
						break;
					case SPRIFRCSTA_MID:
						err_volu ( ERR_SPRI_MID );
						break;
					case SPRIFRCSTA_ERR:
						err_volu ( ERR_SPRI_ERR );
						break;
					default:
						break;
				}
				if ( SpeRinN < 80 )  						//�����г̣�������Ȧ������80ȦΪ1���о
				{
					mType= M1S; 					//ʶ���о����-��ֵ
					mRunTe=M1SRUNTIME;   			//��ֵ������г�ʱʱ��
					if ( !StmSp2 )
					{
						StmSp2=50;    //��һ����բ����ʱ��δʶ��ʱ��ǿ�и�ֵ
					}
					StmInt ( M1S );					//��ȡ����������
					DwSloInt ( 5,15 );					//��ȡ��ʼ���١�����Ĳ���
				}
				else if ( SpeRinN < 130 ) 					//�����г̣�������Ȧ������130ȦΪ1.8��/3.8���о
				{
					mType= M18S; 					//ʶ���о����-��ֵ
					mRunTe=M18SRUNTIME; 			//��ֵ������г�ʱʱ��
					if ( !StmSp2 )
					{
						StmSp2=30;    //��һ����բ����ʱ��δʶ��ʱ��ǿ�и�ֵ
					}
					StmInt ( M18S );					//��ȡ����������
					DwSloInt ( 10,35 );				//��ȡ��ʼ���١�����Ĳ���
				}
				else			   						//ʶ���쳣ʱ
				{
					mType= M18S; 					//ʶ���о����-��ֵ
					mRunTe=M18SRUNTIME; 			//��ֵ������г�ʱʱ��
					if ( !StmSp2 )
					{
						StmSp2=50;    //��һ����բ����ʱ��δʶ��ʱ��ǿ�и�ֵ
					}
					StmInt ( M18S );					//��ȡ����������
					DwSloInt ( 10,35 );				//��ȡ��ʼ���١�����Ĳ���
				}
			}
	}
	else								//���ʶ��״̬: Ŀǰֻ������բʱʶ���о����
	{
		if ( mType < Prg )				//��о����δʶ��
		{
			if ( PosRal== pos ) 	//��ʼʶ��λ��
			{
//							if(sta== MLN_UP) mType= MLN_UP;  //̧բ������ʶ���о����----ȡ������Ӱ����բ����Ȧ����ȡ����
				if ( sta== MLN_DW )
				{
					mType= MLN_DW;
				}
			}
			mRunTe=MAXRUNTIME;		//��о����δ֪ʱ����ȡĬ������ʱ�����
			StmSp=STMMAXTIME;		//��о����δ֪ʱ����ȡĬ��������
		}
	}

}

//**********************************************//
//������:u8 StmCtr(u16 stmlm)
//����:�����жϺ���
//����:������ֵ
//����:����״̬: 1-���裻   0-����
//**********************************************//
u8 StmCtr ( u16 stmlm )
{
	if ( !mCorNum )	//Ϊ��ʱ������𲽽�������ʼ�ж��Ƿ�����
	{
		if ( StmNum > stmlm ) //ʵ����ٻ���Ȧ������ > �趨������ = ����
		{
			if ( Stm )
			{
				Stm--;    //��������ж�
			}
			else
			{
				return 1;
			}
		}
		else
		{
			Stm=2;
		}
	}
	else
	{
		Stm=2;
	}

	return 0;
}




//**********************************************//
//������:  u8 CalcuStm(u8 cmd)
//����:����Ӧ���������ȣ���߲�ͬ��о������
//����:cmd-��������
//����:״̬/���ٻ���Ȧ��ƽ������
//**********************************************//
u8 CalcuStm ( u8 cmd )
{
	u8 i= 0;
	switch ( cmd )
	{
		// 	ÿ����բʱ���������ٻ�10Ȧ��ƽ�����ڣ�
		case STMBUF_AVER_SPE:
			if ( StmNumBuf[STMBUF_POIN]== STMBUF_POST )
			{
				StmNum3=0;
				for ( i=0; i < ( STMBUF_POST-1 ); i++ )
				{
					if ( StmNumBuf[i] > StmNum3 )
					{
						StmNum3= StmNumBuf[i];
					}
				}
				StmNumBuf[STMBUF_DATA]= StmNum3;
				StmNumBuf[STMBUF_POIN]= STMBUF_POST+1;
				return StmNumBuf[STMBUF_DATA];			//���ز��ٻ���Ȧ��ƽ������
			}
			else if ( StmNumBuf[STMBUF_POIN] < STMBUF_POST )
			{
				if ( StmNumBuf[STMBUF_POST]!= SpeRinN )
				{
					StmNumBuf[STMBUF_POST]= SpeRinN;
					StmNumBuf[StmNumBuf[STMBUF_POIN]]= StmNum2;
					StmNumBuf[STMBUF_POIN]++;
				}
			}
			return StmNumBuf[STMBUF_DATA];	//���ز��ٻ���Ȧ��ƽ������
			break;

		// 	ÿ������բ2�κ󣬶Աȵ�ǰ���������ȣ�������������ʱ���������������ȣ�//
		case STMBUF_CALC_SPE:
			if ( StmSp== STMMAXTIME + 1 )
			{
				StmInt ( mType );    //���������һ����բ�ɹ������¸�ֵ����������
			}
			if ( StmSp== STMMAXTIME )
			{
				break;    //�ϵ��һ����բ��������
			}

			if ( StmNumBuf2[2] < 2 )
			{
				if ( StmSp2 > 20 )
				{
					StmNumBuf2[StmNumBuf2[2]]= StmSp2;
					StmNumBuf2[2]++;
				}
			}
			if ( StmNumBuf2[2]== 2 )
			{
				//		if( (StmNumBuf2[0] > StmSp2 && StmNumBuf2[1] > StmSp2 ) || (StmNumBuf2[0] < StmSp2 && StmNumBuf2[1] < StmSp2 ) )
				if ( StmNumBuf2[0] > StmSp2 && StmNumBuf2[1] > StmSp2 )
				{
					StmNum3= StmNumBuf[0] + StmNumBuf[1];
					StmNum3/= 2;
					StmSp2= StmNum3;
				}
				memset ( StmNumBuf2,0,sizeof ( StmNumBuf2 ) );
				return 1;
			}
			return 0;
			break;

		//����1����
		case STMBUF_CLR1:
			memset ( StmNumBuf,0,sizeof ( StmNumBuf ) );
			break;

		//����2����
		case STMBUF_CLR2:
			StmSlow2= 0;					//����������--��բ����ʱ�������㣬����ѧϰ
			StmSp= STMMAXTIME+1;			//�������ʱ�����������ȵ����������һ����բ�ɹ����ڼ������������Ȳ���ֵ
			memset ( StmNumBuf2,0,sizeof ( StmNumBuf2 ) );
			break;
		default:
			break;
	}
	return 0;
}





void mTaskControl()			//����������
{
	if ( SPE!=SpeSta )
	{
		SpeSta= SPE;
		StmNum2= StmNum;
		StmNum= 0;
		SpeRinN++;
	}

	switch ( mTask )
	{
		case MTASK_NA:
			if ( PosRal== POS_HOR )
			{
				mTask= MTASK_DS;
			}
			if ( PosRal== POS_VER )
			{
				mTask= MTASK_US;
			}
			break;
		/***********************************************************************************************/
//					̧բ
		/***********************************************************************************************/
		case MTASK_UP:
			InitRepairMotor();
			gRepairMotor.ExFlag = 1;
			gRepairMotor.Direction = POS_UP;
			BrakeClr();
			KnowMtyp ( MLN_UP,MTPYREAD,POS_HOR );
			//GetLimitValue(mType);//��ȡ�߽�ֵ
			SpringTest ( SPRI_CLR_PARA );
			SpeRinN= 0;
			mCorNum= M_OF_TIME;
			mTask= MTASK_UP1;
			mPosD2= POS_UP;
			M_CLOS;
			break;
		case MTASK_UP1:
			if ( !mCorNum )
			{
				MCR_UP;
				mCorNum= M_ON_TIME;
				mTask= MTASK_UP2;
			}
			break;
		case MTASK_UP2:
			if ( !mCorNum )
			{
				mRunNum= mRunTe;				//��������ʱ������
				mCorNum= M_CV_UP_TIME;			//����ת�л���ʱ��
				mTask= MTASK_UP3;
				if ( mPosD== POS_DW ) 				//����բ������̧բ
				{
					LitM= M_EN;				//��/̧�л�ʱ��С�����̧բ1�룬������բʱ�˵Ĺ���
				}
				mPosD= POS_UP;
				M_OPEN;
			}
			break;
		case MTASK_UP3:
			if ( !mCorNum )
			{
				LitM= M_NA;
			}

			if ( !mRunNum ) //̧բ��ʱʧ��
			{
				mPosD=POS_INT;
				err_volu ( ERR_UP_FAIL );
			}
			else if ( StmCtr ( StmSp + StmSlow+80 ) )
			{
				err_volu ( ERR_DW_DRAG );
			}
			else
			{
				;
			}


			break;
		/***********************************************************************************************/
//					̧բ����
		/***********************************************************************************************/
		case MTASK_US:
			KnowMtyp ( MNA,MLN_UP,POS_VER );
			LitM= M_NA;
			BigM5u= M_NA;
			mCorNum= M_OF_TIME;
			mTask= MTASK_US1;
			mPosD= POS_VER;
			mPosD2= POS_VER;
			break;
		case MTASK_US1:
			if ( !mCorNum )
			{
				MCR_UP;
				mCorNum= M_ON_TIME;
				mTask= MTASK_US2;
			}
			break;
		case MTASK_US2:
			if ( !mCorNum )
			{
				M_SLOW;
				mRunNum= M_LIT_UP_TIME;
				mTask= MTASK_US3;
			}
			break;
		case MTASK_US3:
			if ( !mRunNum )
			{
				LitM= M_NA;
				if ( PosRal== POS_VER )
				{
					mTask= MTASK_US5;
				}
				else
				{
					mTask= MTASK_US4;
					mPosD=POS_INT;
				}
			}
			else if ( PosRal!= POS_VER )
			{
				mPosD=POS_INT;
				Event ( ITASK_INER_DW );
				BrakeClr();
			}
			break;
		case MTASK_US4:
			if ( PosRal== POS_VER )
			{
				mTask= MTASK_US5;
			}
			err_volu ( ERR_US_FAIL );              	 //̧բ����ʧ��
			break;
		case MTASK_US5:
			if ( PosRal!= POS_VER )
			{
				mPosD=POS_INT;
				Event ( ITASK_INER_DW );
				BrakeClr();
				break;
			}
			if ( mPosD== POS_INT )
			{
				mPosD= POS_VER;
			}
//				 Timer2_Zero();
			break;

		/***********************************************************************************************/
//					      ��բ
		/***********************************************************************************************/
		case MTASK_DW:
			InitRepairMotor();
			gRepairMotor.ExFlag = 0;
			gRepairMotor.Direction = POS_DW;
			BrakeClr();
			KnowMtyp ( MLN_DW,MTPYREAD,POS_VER );
			CalcuStm ( STMBUF_CLR1 );
			SpringTest ( SPRI_CLR_PARA );
			SpeRinN= 0;
			mCorNum= M_OF_TIME;
			mTask= MTASK_DW1;
			mPosD= POS_DW;
			mPosD2= POS_DW;
			M_CLOS;
			break;
		case MTASK_DW1:
			if ( !mCorNum )
			{
				MCR_DW;
				mCorNum= M_ON_TIME;
				mTask= MTASK_DW2;
			}
			break;
		case MTASK_DW2:
			if ( !mCorNum )
			{
				mRunNum= mRunTe;
				mCorNum= M_CV_DW_TIME;
				mTask= MTASK_DW3;
				M_OPEN;
			}
			break;
		case MTASK_DW3:
			if ( mType > MTPYREAD ) 				 //��ʶ���о����ʱ����������
			{
				if ( SpeRinN < 26 ) 			 //��ʱ��ǰ26Ȧ������������ = Ĭ�������� + ����������
				{
					StmSlow= STMMAXTIME;
				}
				else if ( SpeRinN > DwSlo )	 //��ʼ����׶�
				{
					LitM= M_EN;			 //����
					SpringTest ( SPRI_RED_PARA );	//��ȡ���ɵ���������
				}
				else if ( SpeRinN > DwSloBri )	 //��ʼ���ٽ׶�
				{
					BigM5u= M_NA; 		 //����
					SpringTest ( SPRI_RED_PARA );	//��ȡ���ɵ���������
					StmSlow= StmSlow2;	 //��բ����ʱ�����������ȵ���(��ֵԽ��������Խ��)
				}
				else
				{
					StmSlow= 0;    //��������բ�׶Σ����������ȵļ��١����岹������
				}
				if ( !mCorNum )
				{
					StmSp2= CalcuStm ( STMBUF_AVER_SPE );    //��ȡ��բʱƽ���ٶ�
				}
			}
			else 								 //δʶ���о����ʱ����������
			{
				if ( SpeRinN < 20 ) 			 //��ʱ��ǰ20Ȧ������������ = Ĭ�������ȵ�2��
				{
					StmSlow= STMMAXTIME;
				}
				else
				{
					StmSlow= 0;
				}
				if ( SpeRinN > 40 ) 			 //����40Ȧ���û���ķ�ʽ��բ��ȷ���״��ϵ���բƽ��
				{
					BigM5u= M_NA;
					LitM= M_EN; //�״��ϵ���բ��ǰ���ٺͻ���
					SpringTest ( SPRI_RED_PARA );	//��ȡ���ɵ���������
				}
				if ( !mCorNum )
				{
					StmSp2= CalcuStm ( STMBUF_AVER_SPE );    //��ȡ��һ����բ��ƽ���ٶ�
				}
			}

			if ( !mRunNum )   						//��ʱֹͣ��բ
			{
				mPosD= POS_INT;
				BigM5u= M_NA;
				LitM= M_NA;
				err_volu ( ERR_DW_FAIL );
			}
			else if ( StmCtr ( StmSp + StmSlow ) ) //��բ����
			{
				if ( iTask== ITASK_COER_UP )
				{
					break;    //ˮƽλǿ��̧բ����ʱ����ִ����������
				}
				Event ( ITASK_DRAG_UP );
				err_volu ( ERR_DW_DRAG );
				CalcuStm ( STMBUF_CLR2 );	//����������������ж�������������
				DgStaClr();
				if ( ViceSta==VICE_STA_EN )
				{
					ViceSta=VICE_STA_DRAG;
				}
				break;
			}
			else;
			break;

		/***********************************************************************************************/
//						  ��բ����
		/***********************************************************************************************/
		case MTASK_DS:
			SpringTest ( SPRI_LOG_PARA1 );
			if ( CalcuStm ( STMBUF_CALC_SPE ) )
			{
				StmInt ( mType );    //ÿ��բ���κ󣬼��������������Ƿ�Ҫ����
			}
			KnowMtyp ( MNA,MLN_DW,POS_HOR );
			LitM= M_NA;
			BigM5u= M_NA;
			mCorNum= M_OF_TIME;
			mTask= MTASK_DS1;
			mPosD= POS_HOR;
			mPosD2= POS_HOR;
			break;
		case MTASK_DS1:
			if ( !mCorNum )
			{
				MCR_DW;
				mCorNum= M_ON_TIME;
				mTask= MTASK_DS2;
			}
			break;
		case MTASK_DS2:
			if ( !mCorNum )
			{
				M_SLOW;
				mRunNum= M_LIT_DW_TIME;
				mTask= MTASK_DS3;
			}
			break;
		case MTASK_DS3:
			if ( !mRunNum )
			{
				LitM= M_NA;
				if ( PosRal== POS_HOR )
				{
					mTask= MTASK_DS5;
				}
				else
				{
					mTask= MTASK_DS4;
					mPosD=POS_INT;
				}
			}
			else if ( PosRal!= POS_HOR )
			{
				mPosD= POS_INT;
				Event ( ITASK_COER_UP );
				BrakeClr();
			}
			break;
		case MTASK_DS4:
			if ( PosRal== POS_HOR )
			{
				mTask= MTASK_DS5;
			}
			err_volu ( ERR_DS_FAIL );  	//̧բ����ʧ��
			break;
		case MTASK_DS5:
			if ( mPosD== POS_INT )
			{
				mPosD= POS_HOR;
			}
			if ( PosRal!= POS_HOR )
			{
				mPosD= POS_INT;
				Event ( ITASK_COER_UP );
				BrakeClr();
			}
//				Timer2_Zero();
			break;

		default:
			break;
	}
}



void Monitor_clk ( void )	  //��ؼ���
{
	if ( gRepairMotor.Times )
	{
		gRepairMotor.Times--;
	}
}

//DEUBGר��
//void MonitorTask(void)
//{
//    if(gRepairMotor.Times == 0 && gRepairMotor.Direction == POS_DW) //���ٻ�ת���ſ�ʼ����
//    {
//        if(gRepairMotor.CurrentCounts >= 0 && gRepairMotor.CurrentCounts < 255)//40
//        {
//            gRepairMotor.CurrentCounts++;
//            //��ӡ��ǰ���ٻ�ֵ
//            Uart_Print(3,gRepairMotor.Direction);

//            gRepairMotor.Times = 5;
//        }
//    }
//}

/*
1.ֻ����3500ms��ʱ��1�����ɣ�2�����ɣ�3�����ɲŻ���һ�����ԵĽ���,���ԣ�
�����ڵ��������� �ж���2�����ɻ���3������ʱ��������ߵ�Ȧ���ĺ�Ҫ����11��
��������Ȧ����ҲҪ���ڵ���85�ſ��ԣ��������޶��˵�һ����ƫ����������;
2.����һ�������ǣ�ֻҪ���ߵ�Ȧ���ʹ���28����ֱ���ж������⣬Ȼ��̧բ��
��Ȼ˵�����������������35Ȧ�����ǣ�Ҫ����28Ȧ��ʱ��������������������6��
һ���������ڵ�Ȧ����6�����ԣ�ֻҪ����28Ȧ�������Ǽ�����������δ���6������Ϊ�Ƕ��˵��ɵġ�

*/

void MonitorTask ( void )
{    
    if(gRepairMotor.Times == 0 && gRepairMotor.ExFlag == 0 && gRepairMotor.Direction == POS_DW) //���ٻ�ת���ſ�ʼ����
    {
        gRepairMotor.CurrentCounts++;

        if((gRepairMotor.CurrentCounts >= START_POSITION) && (gRepairMotor.CurrentCounts%TIMER_FRE == 0) && (gRepairMotor.CurrentCounts < END_POSITION) && gMotorLimit.OverCnt != 250 )//39
        {  
            gRepairMotor.CalcCounts++;

            if(SpeRinN > BASIC_SPRING_NUM + BASIC_OFFSET_250MS*(gRepairMotor.CalcCounts+1))
            {
                //ֱ��̧բ������
                gRepairMotor.OutFlag = 1;
                Event(ITASK_DG_UP);
                Uart_Print(3,25);
                err_volu(ERR_SPRI_ERR);
            }
            else if(SpeRinN > BASIC_SPRING_NUM + BASIC_OFFSET_250MS*gRepairMotor.CalcCounts)
            {
                gRepairMotor.OverCount += 1;//����Ӧ�ò�����֣�������־��Ǵ�����
                gRepairMotor.OffsetSum += SpeRinN-(BASIC_SPRING_NUM + BASIC_OFFSET_250MS*(gRepairMotor.CalcCounts-1));
            }
            else if(SpeRinN > (BASIC_SPRING_NUM + BASIC_OFFSET_250MS*(gRepairMotor.CalcCounts-1)))
            {
                 gRepairMotor.OffsetSum += SpeRinN-(BASIC_SPRING_NUM + BASIC_OFFSET_250MS*(gRepairMotor.CalcCounts-1));
            }
            else
            {
                ;
            }

            //ͬ���ģ�����Ȧ���ߵ�̫�࣬��ֱ�ӱ���
            if (gRepairMotor.OverCount >= gMotorLimit.OverCnt || gRepairMotor.OffsetSum > gMotorLimit.Two_Spring_Offset_Sum) //������ȫ�����ж�������ֱ����Ϊ���������
            {
                //ֱ��̧բ������
                gRepairMotor.OutFlag = 1;
                Event(ITASK_DG_UP);
                Uart_Print(3,5);
                err_volu(ERR_SPRI_ERR);
            }



            //�������ɱ���
            //���ٻ���Ԥ��ֵ����������λ�ô�����һ�����Զε�ֵ-2,-2��Ϊһ��ƫ�������ڣ�У����������
            //�������������ʼ��Ϊ39����һ������������39������һ��ƫ���� for(i=0;i<calccount;i++)offset+=offset;
            //if ( gRepairMotor.AverageValue >= gMotorLimit.Three_Spring_Offset_Sum && SpeRinN > BASIC_SPRING_NUM+BASIC_OFFSET_250MS*gRepairMotor.CalcCounts-2 ) //4�׸�
            if(gRepairMotor.OffsetSum >= gMotorLimit.Three_Spring_Offset_Sum && (SpeRinN >= gMotorLimit.Two_Spring_Max_Num))
            {
                gRepairMotor.OutFlag = 1;
                //��ӡ��ǰ���ٻ�ֵ
                Uart_Print(3,3);
                //������ֻ����������
                err_volu(ERR_SPRI_LIT);
            }

            //����һ�����ڣ��Ž�����һ�μ���
            if(gRepairMotor.CalcCounts == SPRING_CHECK_NUM)
            {
                gRepairMotor.ExFlag = 1;
            }

            //��ӡ��ǰ���ٻ�ֵ
            Uart_Print(3,gRepairMotor.Direction);

        }
        gRepairMotor.Times = 5;
    }
}


