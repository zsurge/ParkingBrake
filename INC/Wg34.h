#ifndef __WG34_H
#define __WG34_H	
#include "reg52.h"

#define MAIN_Fosc        11059200UL    //定义主时钟HZ  
#define WIEGANDPROTOCAL  0

   
sbit WG_DATA0 = P3^6;  
sbit WG_DATA1 = P3^7;




void delay_ms(u8 ms); 
void Delay100us(u8 us);       //@11.0592MHz

#if WIEGANDPROTOCAL == 34
void Send_Weigand34(unsigned char *str);
#else
bit Even_Parity(unsigned char *DataBits);
bit Odd_Parity(unsigned char *DataBits);
void Send_Wiegand26(u8 fc,u16 cc);
#endif



#endif

