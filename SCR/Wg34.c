#include "Wg34.h"

void Delay100us ( u8 us )    //@11.0592MHz
{
	unsigned char i, j;

	do
	{
		_nop_();
		_nop_();
		i = 2;
		j = 15;
		do
		{
			while ( --j );
		}
		while ( --i );
	}
	while ( --us );
}

void delay_ms ( u8 ms )
{
	u16 i;
	do
	{
		i=MAIN_Fosc/9600;
		while ( --i );  //96T per loop
	}
	while ( --ms );     //--ms  ms=ms-1
}


#if WIEGANDPROTOCAL == 34
void Send_Weigand34 ( unsigned char* str )
{
	unsigned char one_num = 0;
	unsigned char even = 0;
	unsigned char odd = 0;
	unsigned char check_temp,i;

	check_temp = *str;

	for ( i = 0; i < 8; i++ )
	{

		if ( check_temp & 0x01 )
		{
			one_num++;
		}

		check_temp >>= 1;
	}

	check_temp = * ( str + 1 );
	for ( i = 0; i < 8; i++ )
	{

		if ( check_temp & 0x01 )
		{
			one_num++;
		}

		check_temp >>= 1;
	}

	if ( one_num % 2 )
	{
		even = 1;
	}
	else
	{
		even = 0;
	}

	one_num = 0;
	check_temp = * ( str + 2 );

	for ( i = 0; i < 8; i++ )
	{
		if ( check_temp & 0x01 )
		{
			one_num++;
		}
		check_temp >>= 1;
	}

	check_temp = * ( str + 3 );

	for ( i = 0; i < 8; i++ )
	{
		if ( check_temp & 0x01 )
		{
			one_num++;
		}

		check_temp >>= 1;
	}

	if ( one_num % 2 )
	{
		odd = 0;
	}
	else
	{
		odd = 1;
	}

	one_num = 0;

	WG_DATA0 = 1;
	WG_DATA1 = 1;

	delay_ms ( 1 );

	if ( even )
	{

		WG_DATA1 = 0; /*偶校验位为1*/
		Delay100us ( 2 );
		WG_DATA1 = 1;
	}
	else

	{
		WG_DATA0 = 0; /*偶校验位为0*/
		Delay100us ( 2 );
		WG_DATA0 = 1;
	}

	delay_ms ( 1 ); /*延时1ms*/

	for ( i = 0; i < 32; i++ )
	{
		WG_DATA0 = 1;
		WG_DATA1 = 1;

		if ( str[0] & 0x80 )
		{
			WG_DATA1 = 0;
			Delay100us ( 2 );
			WG_DATA1 = 1;
		}
		else
		{
			WG_DATA0 = 0;
			Delay100us ( 2 );
			WG_DATA0 = 1;
		}

		( * ( long* ) &str[0] ) <<= 1;

		delay_ms ( 1 ); /*延时1ms*/
	}

	WG_DATA0 = 1;
	WG_DATA1 = 1;

	if ( odd )
	{
		WG_DATA1 = 0;
		Delay100us ( 2 );
		WG_DATA1 = 1;
	}
	else
	{
		WG_DATA0 = 0;
		Delay100us ( 2 );
		WG_DATA0 = 1;
	}

	delay_ms ( 1 );
}

#else

bit Even_Parity ( unsigned char* DataBits )
{
	unsigned char i,j;
	bit Bit_EP;
	for ( i=0,j=0; i<12; i++ )
	{
		if ( DataBits[i]==0x01 )
		{
			j++;
		}
	}
	if ( ( j&0x01 ) ==0x01 )
	{
		Bit_EP=1;
	}
	else
	{
		Bit_EP=0;
	}
	return ( Bit_EP );
}


bit Odd_Parity ( unsigned char* DataBits )
{
	bit Bit_OP;
	unsigned char i,j;
	for ( i=0,j=0; i<12; i++ )
	{
		if ( DataBits[i]==0x01 )
		{
			j++;
		}
	}
	if ( ( j&0x01 ) ==0x01 )
	{
		Bit_OP=0;
	}
	else
	{
		Bit_OP=1;
	}
	return ( Bit_OP );
}

void Send_Wiegand26 ( u8 fc,u16 cc )
{
	u8 i = 0;
	u8 CardNo[26] = {0};
	//得到HID的低8位
	for ( i=1; i<9; i++ )
	{
		CardNo[i]= ( fc&0x80 ) >>7;
		fc=fc<<1;
	}
	//得到16位机器号
	for ( i=9; i<25; i++ )
	{
		if ( ( cc&0x8000 ) ==0x8000 )
		{
			CardNo[i]=0x01;
		}
		else
		{
			CardNo[i]=0x00;
		}

		cc=cc<<1;
	}
	//偶校验位
	if ( ( Even_Parity ( &CardNo[1] ) ) ==1 )
	{
		CardNo[0]=0x01;
	}
	else
	{
		CardNo[0]=0x00;
	}

	//奇校验位
	if ( ( Odd_Parity ( &CardNo[13] ) ) ==1 )
	{
		CardNo[25]=0x01;
	}
	else
	{
		CardNo[25]=0x00;
	}

	for ( i=0; i<26; i++ )
	{
		if ( CardNo[i]==0x01 )
		{
			WG_DATA1=0;
			Delay100us ( 1 );
			WG_DATA1=1;
		}
		else
		{
			WG_DATA0=0;
			Delay100us ( 1 );
			WG_DATA0=1;
		}

		delay_ms ( 1 );
	}
}

#endif



