
#include "USART.h"



COMx_Define	COM1;
u8	xdata TX1_Buffer[COM_TX1_Lenth];	//发送缓冲
u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//接收缓冲




/********************* UART1中断函数************************/
void UART1_int ( void ) interrupt 4
{
	if ( RI )
	{
		RI = 0;
		if ( !COM1.RX_busy )
		{
			if ( COM1.RX_Cnt >= COM_RX1_Lenth )
			{
				COM1.RX_Cnt = 0;
				memset ( RX1_Buffer,0x00,sizeof ( RX1_Buffer ) );
				return;
			}
			RX1_Buffer[COM1.RX_Cnt] = SBUF;
			COM1.RX_Cnt++;
			COM1.RX_TimeOut = TimeOutSet1;
		}
	}

	if ( TI )
	{
		TI = 0;
		DE= COM_SEND;
		if ( COM1.TX_read != COM1.TX_write )
		{
			SBUF = TX1_Buffer[COM1.TX_read];
			if ( ++COM1.TX_read >= COM_TX1_Lenth )
			{
				COM1.TX_read = 0;
			}
		}
		else
		{
			COM1.B_TX_busy = 0;
			COM1.TX_read = 0;
			COM1.TX_write=0;
			DE= COM_RECE;
			memset ( TX1_Buffer,0,sizeof ( TX1_Buffer ) );
		}
	}
}


/*************** 装载串口发送缓冲 *******************************/

void TX1_writebuff ( u8 dat )	//写入发送缓冲，指针+1
{
	TX1_Buffer[COM1.TX_write] = dat;	//装发送缓冲
	if ( ++COM1.TX_write >= COM_TX1_Lenth )
	{
		COM1.TX_write = 0;
	}
	if ( COM1.B_TX_busy == 0 )		//空闲
	{
		COM1.B_TX_busy = 1;		//标志忙
		TI = 1;					//触发发送中断
	}
}


void PrintString ( u8* puts )
{
	while ( *puts != '\0' )
	{
		TX1_writebuff ( *puts++ );
	}
}


void intUsart()
{
	memset ( &COM1, 0, sizeof ( COM1 ) );
	memset ( &TX1_Buffer, 0, sizeof ( TX1_Buffer ) );
	memset ( &RX1_Buffer, 0, sizeof ( RX1_Buffer ) );
	DE= COM_RECE;
}

void Uart_Print ( u8 speakTask,u8 num )
{
#if ISDEBUG
	u8 xdata sen_buff[COM_TX1_Lenth];
	u8 i = 0;
	memset ( sen_buff,0,sizeof ( sen_buff ) );

	switch ( speakTask )
	{

		case 3:
          	sen_buff[0]=num + 63;
			sen_buff[1]=0x30+SpeRinN/100%10;
			sen_buff[2]=0x30+SpeRinN/10%10;
			sen_buff[3]=0x30+SpeRinN%10;
			sen_buff[4]=0x2c;
		
			sen_buff[5]=0x30+gRepairMotor.CalcCounts/100%10;
			sen_buff[6]=0x30+gRepairMotor.CalcCounts/10%10;
			sen_buff[7]=0x30+gRepairMotor.CalcCounts%10;
			sen_buff[8]=0x2c;            
            sen_buff[9]=0x30+gRepairMotor.FlagValue%10;
            sen_buff[10]=0x2c;
			sen_buff[11]=0x30+gRepairMotor.CurrentCounts/100%10;
			sen_buff[12]=0x30+gRepairMotor.CurrentCounts/10%10;
            sen_buff[13]=0x30+gRepairMotor.CurrentCounts%10;
            
            sen_buff[14]=0x2c;
            sen_buff[15]=0x30+gRepairMotor.OffsetSum/10%10;
            sen_buff[16]=0x30+gRepairMotor.OffsetSum%10;
			sen_buff[17]=0x3B;   
			break;


//			sen_buff[0]=48+num;
//			sen_buff[1]=0x2E;
//			sen_buff[2]=0x30+SpeRinN/100%10;
//			sen_buff[3]=0x30+SpeRinN/10%10;
//			sen_buff[4]=0x30+SpeRinN%10;
//			sen_buff[5]=0x30+mTask/10%10;
//			sen_buff[6]=0x30+mTask%10;
//			sen_buff[7]=0x30+iTask/10%10;
//			sen_buff[8]=0x30+iTask%10;
//			sen_buff[9]=0x30+iTask2/10%10;
//			sen_buff[10]=0x30+iTask2%10;
//			sen_buff[11]=0x30+mPosD/10%10;
//			sen_buff[12]=0x30+mPosD%10;
//			sen_buff[13]=0x30+DwSlo/100%10;
//			sen_buff[14]=0x30+DwSlo/10%10;
//			sen_buff[15]=0x30+DwSlo%10;
//			sen_buff[16]=0x30+hafTest%10;
//			sen_buff[17]=0x30+AtNum%10;
//			sen_buff[18]=0x30+aLine%10;
//			sen_buff[19]=0x3B;
//			break;

		default:
			break;
	}


	for ( i=0; i<strlen ( sen_buff ); i++ )
	{
		TX1_writebuff ( sen_buff[i] );
	}

#endif

}

//speakTask:操作命令
//num:操作数
void speak ( u8 speakTask,u8 num )
{
	u8 xdata sen_buff[COM_TX1_Lenth];
	memset ( sen_buff,0,sizeof ( sen_buff ) );
	switch ( speakTask )
	{
		case 1:
			break;


		case VICE_STA:
			sen_buff[0]=num;
			sen_buff[1]=num;
			sen_buff[2]=num;
			break;

//			case 3:
//				sen_buff[0]=num + 63;
//				sen_buff[1]=0x30+SpeRinN/100%10;
//				sen_buff[2]=0x30+SpeRinN/10%10;
//				sen_buff[3]=0x30+SpeRinN%10;
//				sen_buff[4]=0x2c;

//				sen_buff[5]=0x30+gRepairMotor.LastSpeRin/100%10;
//				sen_buff[6]=0x30+gRepairMotor.LastSpeRin/10%10;
//				sen_buff[7]=0x30+gRepairMotor.LastSpeRin%10;
//				sen_buff[8]=0x2c;

//				sen_buff[9]=0x30+gRepairMotor.FlagValue/100%10;
//				sen_buff[10]=0x30+gRepairMotor.FlagValue/10%10;
//				sen_buff[11]=0x30+gRepairMotor.FlagValue%10;
//				sen_buff[12]=0x2c;

//				sen_buff[13]=0x30+gRepairMotor.CurrentCounts/100%10;
//				sen_buff[14]=0x30+gRepairMotor.CurrentCounts/10%10;
//				sen_buff[15]=0x30+gRepairMotor.CurrentCounts%10;
//				sen_buff[16]=0x2c;

//				break;
//
//			case 2:
//				sen_buff[0]=num + 39;
//				sen_buff[1]=0x30+gRepairMotor.CurrentCounts/100%10;
//				sen_buff[2]=0x30+gRepairMotor.CurrentCounts/10%10;
//				sen_buff[3]=0x30+gRepairMotor.CurrentCounts%10;
//				sen_buff[4]=0x3b;
//				sen_buff[5]=0x0a;

//				sen_buff[0]=0x30+test_dly/10000%10;
//				sen_buff[1]=0x30+test_dly/1000%10;
//				sen_buff[2]=0x30+test_dly/100%10;
//				sen_buff[3]=0x30+test_dly/10%10;
//				sen_buff[4]=0x30+test_dly%10;
//				sen_buff[5]=0x3b;
//				sen_buff[6]=0x0a;

//				sen_buff[0]=0x30+err/100%10;
//				sen_buff[1]=0x30+err/10%10;
//				sen_buff[2]=0x30+err%10;
//				sen_buff[3]=0x30+mTask/100%10;
//				sen_buff[4]=0x30+mTask/10%10;
//				sen_buff[5]=0x30+mTask%10;
//				sen_buff[6]=0x3b;
//				sen_buff[7]=0x0a;

//				sen_buff[0]=0x30+mTask/10%10;
//				sen_buff[1]=0x30+mTask%10;
//				sen_buff[2]=0x30+iTask/10%10;
//				sen_buff[3]=0x30+iTask%10;
//				sen_buff[4]=0x30+Even/10%10;
//				sen_buff[5]=0x30+Even%10;
//				sen_buff[6]=0x30+mPosD/10%10;
//				sen_buff[7]=0x30+mPosD%10;
//				sen_buff[8]=0x3b;
//				sen_buff[9]=0x0a;

//				sen_buff[0]=0x30+HupSta/100%10;
//				sen_buff[1]=0x30+HupSta/10%10;
//				sen_buff[2]=0x30+HupSta%10;
//				sen_buff[3]=0x30+HDwSta/100%10;
//				sen_buff[4]=0x30+HDwSta/10%10;
//				sen_buff[5]=0x30+HDwSta%10;
//				sen_buff[6]=0x3b;
//				sen_buff[7]=0x0a;

//				sen_buff[0]=0x30+mRunNum/100%10;
//				sen_buff[1]=0x30+mRunNum/10%10;
//				sen_buff[2]=0x30+mRunNum%10;
//				sen_buff[3]=0x30+mRunTe/100%10;
//				sen_buff[4]=0x30+mRunTe/10%10;
//				sen_buff[5]=0x30+mRunTe%10;
//				sen_buff[6]=0x3b;
//				sen_buff[7]=0x0a;

//				sen_buff[0]=0x30+FzSta/10%10;
//				sen_buff[1]=0x30+FzSta%10;
//				sen_buff[2]=0x30+DgSta/10%10;
//				sen_buff[3]=0x30+DgSta%10;
//				sen_buff[4]=0x30+PosRal/10%10;
//				sen_buff[5]=0x30+PosRal%10;
//				sen_buff[6]=0x3b;
//				sen_buff[7]=0x0a;

//				sen_buff[0]=0x30+mType/100%10;
//				sen_buff[1]=0x30+mType/10%10;
//				sen_buff[2]=0x30+mType%10;
//				sen_buff[3]=0x30+SpeRinN/100%10;
//				sen_buff[4]=0x30+SpeRinN/10%10;
//				sen_buff[5]=0x30+SpeRinN%10;
//				sen_buff[6]=0x3b;
//				sen_buff[7]=0x0a;

//				sen_buff[0]=0x30+err/100%10;
//				sen_buff[1]=0x30+err/10%10;
//				sen_buff[2]=0x30+err%10;
//				sen_buff[3]=0x30+mType/100%10;
//				sen_buff[4]=0x30+mType/10%10;
//				sen_buff[5]=0x30+mType%10;
//				sen_buff[6]=0x3b;
//				sen_buff[7]=0x0a;

//				sen_buff[0]=0x30+DgSta/100%10;
//				sen_buff[1]=0x30+DgSta/10%10;
//				sen_buff[2]=0x30+DgSta%10;
//				sen_buff[3]=0x30+Even/100%10;
//				sen_buff[4]=0x30+Even/10%10;
//				sen_buff[5]=0x30+Even%10;
//				sen_buff[6]=0x3b;
//				sen_buff[7]=0x0a;

//				sen_buff[0]=0x30+mTask/100%10;
//				sen_buff[1]=0x30+mTask/10%10;
//				sen_buff[2]=0x30+mTask%10;
//				sen_buff[3]=0x30+Even/100%10;
//				sen_buff[4]=0x30+Even/10%10;
//				sen_buff[5]=0x30+Even%10;
//				sen_buff[6]=0x30+iTask/100%10;
//				sen_buff[7]=0x30+iTask/10%10;
//				sen_buff[8]=0x30+iTask%10;
//				sen_buff[9]=0x30+iTask2/10%10;
//				sen_buff[10]=0x30+iTask2%10;
//				sen_buff[11]=0x30+mPosD/10%10;
//				sen_buff[12]=0x30+mPosD%10;
//				sen_buff[13]=0x3b;
//				sen_buff[14]=0x0a;

//				sen_buff[0]=0x30+mType/100%10;
//				sen_buff[1]=0x30+mType/10%10;
//				sen_buff[2]=0x30+mType%10;
//				sen_buff[3]=0x30+mRunTe/100%10;
//				sen_buff[4]=0x30+mRunTe/10%10;
//				sen_buff[5]=0x30+mRunTe%10;
//				sen_buff[6]=0x30+StmSp/100%10;
//				sen_buff[7]=0x30+StmSp/10%10;
//				sen_buff[8]=0x30+StmSp%10;
//				sen_buff[9]=0x30+DwSlo/100%10;
//				sen_buff[10]=0x30+DwSlo/10%10;
//				sen_buff[11]=0x30+DwSlo%10;
//				sen_buff[12]=0x30+DwSloBri/100%10;
//				sen_buff[13]=0x30+DwSloBri/10%10;
//				sen_buff[14]=0x30+DwSloBri%10;
//				sen_buff[15]=0x3b;
//				sen_buff[16]=0x0a;

//				sen_buff[0]=0x30+aLine%10;
//				sen_buff[1]=0x30+hafTest/10%10;
//				sen_buff[2]=0x30+hafTest%10;
//				sen_buff[3]=0x30+AtNum/100%10;
//				sen_buff[4]=0x30+AtNum/10%10;
//				sen_buff[5]=0x30+AtNum%10;
//				sen_buff[6]=0x30+iTask/100%10;
//				sen_buff[7]=0x30+iTask/10%10;
//				sen_buff[8]=0x30+iTask%10;
//				sen_buff[9]=0x3b;
//				sen_buff[10]=0x0a;

//				sen_buff[0]=0x30+StmNum2/100%10;
//				sen_buff[1]=0x30+StmNum2/10%10;
//				sen_buff[2]=0x30+StmNum2%10;
//				sen_buff[3]=0x3b;
//				sen_buff[4]=0x0a;

//				sen_buff[0]=0x30+ViceRal/10%10;
//				sen_buff[1]=0x30+ViceRal%10;
//				sen_buff[2]=0x3b;
//				sen_buff[3]=0x0a;


//				sen_buff[0]=0x30+StmSp/100%10;
//				sen_buff[1]=0x30+StmSp/10%10;
//				sen_buff[2]=0x30+StmSp%10;
//				sen_buff[3]=0x30+StmSp2/100%10;
//				sen_buff[4]=0x30+StmSp2/10%10;
//				sen_buff[5]=0x30+StmSp2%10;
//				sen_buff[6]=0x30+StmSlow/100%10;
//				sen_buff[7]=0x30+StmSlow/10%10;
//				sen_buff[8]=0x30+StmSlow%10;
//				sen_buff[9]=0x30+StmSlow2/100%10;
//				sen_buff[10]=0x30+StmSlow2/10%10;
//				sen_buff[11]=0x30+StmSlow2%10;
//				sen_buff[12]=0x3b;
//				sen_buff[13]=0x0a;

//				sen_buff[0]=0x30+StmSp/1000%10;
//				sen_buff[1]=0x30+StmSp/100%10;
//				sen_buff[2]=0x30+StmSp/10%10;
//				sen_buff[3]=0x30+StmSp%10;
//				sen_buff[4]=0x30+StmSlow/1000%10;
//				sen_buff[5]=0x30+StmSlow/100%10;
//				sen_buff[6]=0x30+StmSlow/10%10;
//				sen_buff[7]=0x30+StmSlow%10;
//				sen_buff[8]=0x3b;
//				sen_buff[9]=0x0a;


//				sen_buff[0]=0x30+SpriBuf[SPRIBUF_FST_AVER]/100%10;
//				sen_buff[1]=0x30+SpriBuf[SPRIBUF_FST_AVER]/10%10;
//				sen_buff[2]=0x30+SpriBuf[SPRIBUF_FST_AVER]%10;
//				sen_buff[3]=0x30+SpriBuf[SPRIBUF_FST_MIN]/100%10;
//				sen_buff[4]=0x30+SpriBuf[SPRIBUF_FST_MIN]/10%10;
//				sen_buff[5]=0x30+SpriBuf[SPRIBUF_FST_MIN]%10;
//				sen_buff[6]=0x30+SpriBuf[SPRIBUF_END_AVER]/100%10;
//				sen_buff[7]=0x30+SpriBuf[SPRIBUF_END_AVER]/10%10;
//				sen_buff[8]=0x30+SpriBuf[SPRIBUF_END_AVER]%10;
//				sen_buff[9]=0x30+SpriBuf[SPRIBUF_END_MAX]/100%10;
//				sen_buff[10]=0x30+SpriBuf[SPRIBUF_END_MAX]/10%10;
//				sen_buff[11]=0x30+SpriBuf[SPRIBUF_END_MAX]%10;

//				sen_buff[0]=0x30+SpriBuf[0]/100%10;
//				sen_buff[1]=0x30+SpriBuf[0]/10%10;
//				sen_buff[2]=0x30+SpriBuf[0]%10;
//				sen_buff[3]=0x30+SpriBuf[1]/100%10;
//				sen_buff[4]=0x30+SpriBuf[1]/10%10;
//				sen_buff[5]=0x30+SpriBuf[1]%10;
//				sen_buff[6]=0x30+SpriBuf[2]/100%10;
//				sen_buff[7]=0x30+SpriBuf[2]/10%10;
//				sen_buff[8]=0x30+SpriBuf[2]%10;
//				sen_buff[9]=0x30+SpriBuf[3]/100%10;
//				sen_buff[10]=0x30+SpriBuf[3]/10%10;
//				sen_buff[11]=0x30+SpriBuf[3]%10;
//				sen_buff[12]=0x30+SpriBuf[4]/100%10;
//				sen_buff[13]=0x30+SpriBuf[4]/10%10;
//				sen_buff[14]=0x30+SpriBuf[4]%10;
//				sen_buff[15]=0x30+SpriBuf[5]/100%10;
//				sen_buff[16]=0x30+SpriBuf[5]/10%10;
//				sen_buff[17]=0x30+SpriBuf[5]%10;

//				sen_buff[0]=0x30+SpriBuf[6]/100%10;
//				sen_buff[1]=0x30+SpriBuf[6]/10%10;
//				sen_buff[2]=0x30+SpriBuf[6]%10;
//				sen_buff[3]=0x30+SpriBuf[7]/100%10;
//				sen_buff[4]=0x30+SpriBuf[7]/10%10;
//				sen_buff[5]=0x30+SpriBuf[7]%10;
//				sen_buff[6]=0x30+SpriBuf[8]/100%10;
//				sen_buff[7]=0x30+SpriBuf[8]/10%10;
//				sen_buff[8]=0x30+SpriBuf[8]%10;
//				sen_buff[9]=0x30+SpriBuf[9]/100%10;
//				sen_buff[10]=0x30+SpriBuf[9]/10%10;
//				sen_buff[11]=0x30+SpriBuf[9]%10;
//				sen_buff[12]=0x30+SpriBuf[10]/100%10;
//				sen_buff[13]=0x30+SpriBuf[10]/10%10;
//				sen_buff[14]=0x30+SpriBuf[10]%10;
//				sen_buff[15]=0x30+SpriBuf[11]/100%10;
//				sen_buff[16]=0x30+SpriBuf[11]/10%10;
//				sen_buff[17]=0x30+SpriBuf[11]%10;
//				sen_buff[18]=0x3b;
//				sen_buff[19]=0x0a;


//				sen_buff[0]=0x30+StmNumBuf[0]/100%10;
//				sen_buff[1]=0x30+StmNumBuf[0]/10%10;
//				sen_buff[2]=0x30+StmNumBuf[0]%10;
//				sen_buff[3]=0x30+StmNumBuf[5]/100%10;
//				sen_buff[4]=0x30+StmNumBuf[5]/10%10;
//				sen_buff[5]=0x30+StmNumBuf[5]%10;
//				sen_buff[6]=0x30+StmNumBuf[9]/100%10;
//				sen_buff[7]=0x30+StmNumBuf[9]/10%10;
//				sen_buff[8]=0x30+StmNumBuf[9]%10;
//				sen_buff[9]=0x30+StmNumBuf[STMBUF_DATA]/100%10;
//				sen_buff[10]=0x30+StmNumBuf[STMBUF_DATA]/10%10;
//				sen_buff[11]=0x30+StmNumBuf[STMBUF_DATA]%10;


//				sen_buff[12]=0x3b;
//				sen_buff[13]=0x0a;

//				sen_buff[0]=0x30+StmNumBuf2[0]/100%10;
//				sen_buff[1]=0x30+StmNumBuf2[0]/10%10;
//				sen_buff[2]=0x30+StmNumBuf2[0]%10;
//				sen_buff[3]=0x30+StmNumBuf2[1]/100%10;
//				sen_buff[4]=0x30+StmNumBuf2[1]/10%10;
//				sen_buff[5]=0x30+StmNumBuf2[1]%10;
//				sen_buff[6]=0x30+StmNumBuf2[2]/100%10;
//				sen_buff[7]=0x30+StmNumBuf2[2]/10%10;
//				sen_buff[8]=0x30+StmNumBuf2[2]%10;
//				sen_buff[9]=0x3b;
//				sen_buff[10]=0x0a;

			break;

	}

	PrintString ( sen_buff );	//SUART1发送一个字符串
}


/**************************************************************/
/**************************************************************/
/**************************************************************/
void uart_aly()
{
//	u8 xdata buf[COM_RX1_Lenth];

	if ( !COM1.RX_TimeOut )	//超时计数
	{
		if ( COM1.RX_Cnt>0 )
		{
			COM1.RX_busy = 1;
//							memcpy(&buf,&RX1_Buffer,sizeof(RX1_Buffer));
			if ( RX1_Buffer[0]== RX1_Buffer[1] && RX1_Buffer[0]== RX1_Buffer[2] )
			{
				ViceRal= RX1_Buffer[0];
			}
			memset ( RX1_Buffer,0x00,sizeof ( RX1_Buffer ) );
			COM1.RX_Cnt = 0;
			COM1.RX_busy = 0;
			COM1.B_RX_OK = 1;
		}
	}
	if ( COM1.B_RX_OK )
	{
//			if(buf[0]== buf[1] && buf[0]== buf[2]) ViceRal= buf[0];
		COM1.B_RX_OK= 0;
	}
}



