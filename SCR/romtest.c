/****************************************************************************
* File:			romtest.c													*
* Created:		2015-02-13													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#include "ROMTEST.h"




//************************************************************************************************************//
////存储器测试
//************************************************************************************************************//
void RomTest()
{    
	u8 t=0,i=0; u8 xdata buff[8];
	WDT_CONTR=DISABLE;		//内部看门狗关闭
	err=ERR_EEPROM_WRITE;
	EEPROM_read_n(FIRST_SECTOR,&buff,8);
	for(t=0;t<8;t++) {if(buff[t]==eight) i++;}	//存储器识别码
	if(i==8)err=ERR_INT;		
	if(err==ERR_EEPROM_WRITE)//系统初始化
		{
	//********擦&写&读3次测试X个扇区**********//
			for(t=0;t<1;t++)
				{	
					SectotE(FIRST_SECTOR,0x200,2);//删除x扇区
					delay(10);
					SectotWT(FIRST_SECTOR,0xaa,2);	//x个扇区里全写满数据0xAA=10101010
					delay(10);
					if(SectotRT(FIRST_SECTOR,0xaa,2)) { BUZZ=BUZZ_EN;  } //读x个扇区并判断数据是否错误
					delay(10);
					SectotE(FIRST_SECTOR,0x200,2);//删除x扇区
					delay(10);
					SectotWT(FIRST_SECTOR,0x55,2);//x个扇区里全写满数据0x55=01010101
					delay(10);
					if(SectotRT(FIRST_SECTOR,0x55,2)) { BUZZ=BUZZ_EN;  } //读x个扇区并判断数据是否错误
					delay(10);
				}
	//********测试完成，1扇区写入初始化识别数-8个识别码,用作识别是否已经初始化配置**********//
			if(BUZZ)
				{
					EEPROM_SectorErase(FIRST_SECTOR);
					for(t=0;t<8;t++) buff[t]=eight;
					EEPROM_write_n(FIRST_SECTOR,&buff,8);
				}
			else 
				{
					BUZZ=BUZZ_CL;
					err=ERR_EEPROM_WRITE1;
				}
		}

}





//************************************************************************************************************
////SectotE扇区删除
//入口变量 dx:扇区起始地址
//		   hf:扇区首地址间隔
//		   tt:删除扇区数量
//         
//************************************************************************************************************
void SectotE(u16 dx, u16 hf, u8 tt)
{


	u8 k;
	for(k=0;k<tt;k++)
		{	
			LED=1;
			EEPROM_SectorErase(dx);
			delay(10);
			dx+=hf;
		}
}

//************************************************************************************************************
////SectotWT扇区写入测试
//入口变量  dx:扇区起始地址
//			nu:要写的数据
//		    tt:写操作扇区数量
//************************************************************************************************************
void SectotWT(u16 dx, u8 nu, u8 tt)
{


	u8 k,i; u8 xdata buff[8];
	for(k=0;k<8;k++) buff[k]=nu;
	for(k=0;k<tt;k++)
		{
			for(i=0;i<64;i++)//一个扇区分64次写入，一次写8个字节
				{
					EEPROM_write_n(dx,&buff,8);
					dx+=8;
					delay(10);
				}
			LED=!LED;
			
			delay(30);
		}
  
}
					

//************************************************************************************************************
////SectotRT扇区读出测试
//入口变量  dx:扇区起始地址
//			nu:要比较的数据
//		    tt:读操作扇区数量
//************************************************************************************************************
bit SectotRT(u16 dx, u8 nu, u8 tt)
{

	u8 k,u,i;		u8 xdata buff[8];		 u16 cnum=0;

	for(k=0;k<tt;k++)
		{
			for(i=0;i<64;i++) 
				{
					EEPROM_read_n(dx,&buff,8);
					dx+=8;
					for(u=0;u<8;u++)
						{
							if(buff[u]==nu)
							cnum++;
						}
					LED=!LED;
					delay(10);
				}
			delay(30);
		}
	if(cnum<(512*tt-1)) return 1;
	else return 0;
	return 0;

}






