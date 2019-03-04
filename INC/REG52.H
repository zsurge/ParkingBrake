/*--------------------------------------------------------------------------
REG52.H

Header file for generic 80C52 and 80C32 microcontroller.
Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.
--------------------------------------------------------------------------*/

#ifndef __REG52_H__
#define __REG52_H__

#include <intrins.h>
#include <STRING.H>


typedef 	unsigned char	u8;
typedef 	signed char 	s8;
typedef 	unsigned int	u16;
typedef 	signed int		s16;
typedef 	unsigned long	u32;
typedef 	signed long 	s32;




sfr WDT_CONTR = 0xE1; //Watch-Dog-Timer Control register
//										7	  6 	5	   4	   3	  2   1   0 	Reset Value
//									WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0	xx00,0000
#define D_WDT_FLAG			(1<<7)
#define D_EN_WDT			(1<<5)
#define D_CLR_WDT			(1<<4)	//auto clear
#define D_IDLE_WDT			(1<<3)	//WDT counter when Idle
#define D_WDT_SCALE_2		0
#define D_WDT_SCALE_4		1
#define D_WDT_SCALE_8		2		//T=393216*N/fo
#define D_WDT_SCALE_16		3
#define D_WDT_SCALE_32		4
#define D_WDT_SCALE_64		5
#define D_WDT_SCALE_128		6
#define D_WDT_SCALE_256		7
#define	WDT_reset(n)	WDT_CONTR = D_EN_WDT + D_CLR_WDT + D_IDLE_WDT + (n)		//³õÊ¼»¯WDT£¬Î¹¹·


//**************89C52RC EEPROM OPERATION*************//
sfr IAP_DATA  = 0xe2;
sfr IAP_ADDRH = 0xe3;
sfr IAP_ADDRL = 0xe4;
sfr IAP_CMD   = 0xe5;
sfr IAP_TRIG  = 0xe6;
sfr IAP_CONTR = 0xe7;

typedef enum
{
	CMD_IDLE			= 0,			//Stand by
	CMD_READ			= 1,			//byte read
	CMD_PROGRAM			= 2,			//byte program
	CMD_ERASE			= 3,			//sector erase
} EEPROM_COMMAND;


typedef enum
{
//	ENABLE_IAP			= 0X80,		//SYSCLK<40MHZ
	ENABLE_IAP			= 0X81,		//SYSCLK<20MHZ
//	ENABLE_IAP			= 0X82,		//SYSCLK<10MHZ
//	ENABLE_IAP			= 0X83,		//SYSCLK<5MHZ
}EEPROM_OPERATION;

//**************89C52RC EEPROM OPERATION*************//






/*  BYTE Registers  */
sfr P0    = 0x80;
sfr P1    = 0x90;
sfr P2    = 0xA0;
sfr P3    = 0xB0;
sfr PSW   = 0xD0;
sfr ACC   = 0xE0;
sfr B     = 0xF0;
sfr SP    = 0x81;
sfr DPL   = 0x82;
sfr DPH   = 0x83;
sfr PCON  = 0x87;
sfr TCON  = 0x88;
sfr TMOD  = 0x89;
sfr TL0   = 0x8A;
sfr TL1   = 0x8B;
sfr TH0   = 0x8C;
sfr TH1   = 0x8D;
sfr IE    = 0xA8;
sfr IP    = 0xB8;
sfr SCON  = 0x98;
sfr SBUF  = 0x99;

/*  8052 Extensions  */
sfr T2CON  = 0xC8;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;


/*  BIT Registers  */
/*  PSW  */
sbit CY    = PSW^7;
sbit AC    = PSW^6;
sbit F0    = PSW^5;
sbit RS1   = PSW^4;
sbit RS0   = PSW^3;
sbit OV    = PSW^2;
sbit P     = PSW^0; //8052 only

/*  TCON  */
sbit TF1   = TCON^7;
sbit TR1   = TCON^6;
sbit TF0   = TCON^5;
sbit TR0   = TCON^4;
sbit IE1   = TCON^3;
sbit IT1   = TCON^2;
sbit IE0   = TCON^1;
sbit IT0   = TCON^0;

/*  IE  */
sbit EA    = IE^7;
sbit ET2   = IE^5; //8052 only
sbit ES    = IE^4;
sbit ET1   = IE^3;
sbit EX1   = IE^2;
sbit ET0   = IE^1;
sbit EX0   = IE^0;

/*  IP  */
sbit PT2   = IP^5;
sbit PS    = IP^4;
sbit PT1   = IP^3;
sbit PX1   = IP^2;
sbit PT0   = IP^1;
sbit PX0   = IP^0;

/*  P3  */
sbit RD    = P3^7;
sbit WR    = P3^6;
sbit T1    = P3^5;
sbit T0    = P3^4;
sbit INT1  = P3^3;
sbit INT0  = P3^2;
sbit TXD   = P3^1;
sbit RXD   = P3^0;

/*  SCON  */
sbit SM0   = SCON^7;
sbit SM1   = SCON^6;
sbit SM2   = SCON^5;
sbit REN   = SCON^4;
sbit TB8   = SCON^3;
sbit RB8   = SCON^2;
sbit TI    = SCON^1;
sbit RI    = SCON^0;

/*  P1  */
sbit T2EX  = P1^1; // 8052 only
sbit T2    = P1^0; // 8052 only
             
/*  T2CON  */
sbit TF2    = T2CON^7;
sbit EXF2   = T2CON^6;
sbit RCLK   = T2CON^5;
sbit TCLK   = T2CON^4;
sbit EXEN2  = T2CON^3;
sbit TR2    = T2CON^2;
sbit C_T2   = T2CON^1;
sbit CP_RL2 = T2CON^0;

#endif
