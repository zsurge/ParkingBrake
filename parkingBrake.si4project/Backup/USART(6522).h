
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#ifndef __USART_H
#define __USART_H

#include "reg52.h"
#include "clk.h"
#include "RUN.h"

#define 	ISDEBUG 	1

#define	COM_SEND	1
#define	COM_RECE	0

#define	COM_TX1_Lenth	20
#define	COM_RX1_Lenth	20


#define	USART1	1
#define	USART2	2

#define	UART_ShiftRight	0		//同步移位输出
#define	UART_8bit_BRTx	(1<<6)	//8位数据,可变波特率
#define	UART_9bit		(2<<6)	//9位数据,固定波特率
#define	UART_9bit_BRTx	(3<<6)	//9位数据,可变波特率

#define	UART1_SW_P30_P31	0
#define	UART1_SW_P36_P37	(1<<6)
#define	UART1_SW_P16_P17	(2<<6)	//必须使用内部时钟
#define	UART2_SW_P10_P11	0
#define	UART2_SW_P46_P47	1


#define	TimeOutSet1		5
#define	TimeOutSet2		5

#define	BRT_Timer1	1
#define	BRT_Timer2	2

/****************************************************************************
* 使用端口定义
****************************************************************************/
#define	PORT_DE			P1
#define	B_DE			6
sbit	DE				= PORT_DE ^ B_DE;


typedef struct
{
	u8  id;				//串口号

	u8	TX_read;		//发送读指针
	u8	TX_write;		//发送写指针
	u8	B_TX_busy;		//忙标志

	u8 	RX_Cnt;			//接收字节计数
	u8	RX_TimeOut;		//接收超时
	u8	RX_busy;		//接收忙
	u8	B_RX_OK;		//接收块完成
} xdata COMx_Define;

typedef struct
{
	u8	UART_Mode;			//模式,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	u8	UART_BRT_Use;		//使用波特率,   BRT_Timer1,BRT_Timer2
	u32	UART_BaudRate;		//波特率,       ENABLE,DISABLE
	u8	Morecommunicate;	//多机通讯允许, ENABLE,DISABLE
	u8	UART_RxEnable;		//允许接收,   ENABLE,DISABLE
	u8	BaudRateDouble;		//波特率加倍, ENABLE,DISABLE
	u8	UART_Interrupt;		//中断控制,   ENABLE,DISABLE
	u8	UART_Polity;		//优先级,     PolityLow,PolityHigh
	u8	UART_P_SW;			//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	u8	UART_RXD_TXD_Short;	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
} xdata COMx_InitDefine;

extern	COMx_Define	COM1;
extern volatile	u8 xdata TX1_Buffer[COM_TX1_Lenth];	//发送缓冲
extern volatile u8 xdata RX1_Buffer[COM_RX1_Lenth];	//接收缓冲




void UART_config ( void );
void TX1_writebuff ( u8 dat );	//写入发送缓冲，指针+1
void PrintString ( u8* puts );
void intUsart();
void uart_aly();
void speak ( u8 speakTask,u8 num );

void Uart_Print ( u8 printTask,u8 num );




#endif

