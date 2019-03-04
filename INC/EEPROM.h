#ifndef	__EEPROM_H
#define	__EEPROM_H

#include "reg52.h"

//     型号        大小   扇区数  开始地址  结束地址   MOVC读偏移地址
//STC15F/L2K60S2   1K      8扇区  0x2000  ~  0x2FFF        0xF000


/**************************************************************************** 
* 宏定义
****************************************************************************/

//***************扇区地址*****************//
#define FIRST_SECTOR			0X2000			//扇区一	第0~8个字节用作初始化配置识别，识别码:88
#define SECOND_SECTOR			0X2200			//扇区二	待定~~
#define THIRD_SECTOR			0x2400			//扇区三	待定~~
#define FOURTH_SECTOR			0X2600			//扇区四	待定~~
#define FIFTH_SECTOR			0X2800			//扇区五	待定~~
#define SIXTH_SECTOR			0X2A00			//扇区六	待定~~
#define SEVTH_SECTOR			0X2C00			//扇区七	待定~~
#define EIGTH_SECTOR			0X2E00			//扇区八	待定~~








/**************************************************************************** 
* 使用端口定义
****************************************************************************/




/**************************************************************************** 
* 函数原型
****************************************************************************/
void	DisableEEPROM(void);
void 	EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number);
void 	EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number);
void	EEPROM_SectorErase(u16 EE_address);


#endif