/****************************************************************************
* File:			MOTOR.h														*
* Created:		2015-02-28													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:																*
****************************************************************************/
#ifndef	__MOTOR_H
#define	__MOTOR_H


#include "run.H"
#include "INPUT.H"
#include "ERR.h"
#include "reg52.h"



/****************************************************************************
* 宏定义
****************************************************************************/



//导通角控制/电机任务/电机工作位置



//电机类型
#define MNA						5			//10 电机类型-无
#define MLN_OTH					MNA+1		//11 电机类型识别异常
#define MLN_DW					MLN_OTH+1	//12 电机类型自动辨别流程1
#define MLN_UP					MLN_DW+1	//13 电机类型自动辨别流程2
#define MTPYREAD				9			//
#define M1S						10			//14 1秒电机(转速比27:1)
#define M18S					18			//15 1.8秒电机(转速比53:1)
#define M38S					38			//16 3.8秒电机(转速比115:1)

//大电机正反转时间
#define M_ON_TIME				10			//电机启动延迟时间
#define M_OF_TIME				2			//电机关闭延迟时间
#define M_CV_UP_TIME			1000		//电机正反转--切换抬闸时间
#define M_CV_DW_TIME			800			//电机正反转--切换抬闸时间

//遇阻灵明度冗余值
#define STP1S_REDUNDANCY_TIME	30		//
#define STP18S_REDUNDANCY_TIME	20		//
#define STP38S_REDUNDANCY_TIME	20		//
#define SLOWINT_REDUNDANCY_TIME	160		//170627 CZP修改，降低减速、缓冲时误判为遇阻的BUG
#define SLOW1S_REDUNDANCY_TIME	160		//170627 CZP修改，降低减速、缓冲时误判为遇阻的BUG
#define SLOW18S_REDUNDANCY_TIME	160		//170627 CZP修改，降低减速、缓冲时误判为遇阻的BUG
#define SLOW38S_REDUNDANCY_TIME	160		//170627 CZP修改，降低减速、缓冲时误判为遇阻的BUG
#define STMMAXTIME				300		//

//获取测速环转速的数组指针
#define STMBUF_AVER_SPE			1		//每次落闸时，计量测速环10圈的平均周期；
#define STMBUF_CALC_SPE			2		//速度判断并修正
#define STMBUF_CLR1				3		//遇阻计时-获取平均值数组清零
#define STMBUF_CLR2				4		//遇阻计时-判断修正数组清零
#define STMBUF_POST				11		//测速环当前位置缓存
#define STMBUF_POIN				12		//写数组的指针
#define STMBUF_DATA				13		//写数组的数据
#define STMBUF_LEN				14		//数组长度
#define STMBUF_LEN2				3		//数组长度



//大电机运行时间限制
#define M1SRUNTIME				600		//1秒电机运行时间		----限制运行4秒
#define M18SRUNTIME				900		//1.8秒电机运行时间		----限制运行6秒
#define M38SRUNTIME				900		//3.8秒电机运行时间		----限制运行9秒
#define MAXRUNTIME				1200	//最大运行时间			----限制运行20秒

//小电机运行时间限制
#define M_LIT_UP_TIME			30		//
#define M_LIT_DW_TIME			20		//



//电机落闸提前缓冲圈数设置
#define SLO_1S_RIN				56		//
#define SLO_18S_RIN				95		//
#define SLO_38S_RIN				106		//
#define SLO_MAX_RIN				106		//



//电机工作状态
#define MTASK_NA				0		//
#define MTASK_UP				1		//
#define MTASK_US				2		//
#define MTASK_DW				3		//
#define MTASK_DS				4		//
#define MTASK_UP1				11		//
#define MTASK_UP2				12		//
#define MTASK_UP3				13		//
#define MTASK_UP4				14		//
#define MTASK_US1				21		//
#define MTASK_US2				22		//
#define MTASK_US3				23		//
#define MTASK_US4				24		//
#define MTASK_US5				25		//
#define MTASK_DW1				31		//
#define MTASK_DW2				32		//
#define MTASK_DW3				33		//
#define MTASK_DW4				34		//
#define MTASK_DS1				41		//
#define MTASK_DS2				42		//
#define MTASK_DS3				43		//
#define MTASK_DS4				44		//
#define MTASK_DS5				45		//



//电机位置状态
#define POS_INT					1			//初始未知	
#define POS_UP					2			//正在抬闸	
#define POS_VER					3			//垂直位	
#define POS_DW					4			//正在落闸	
#define POS_HOR					5			//水平位
#define POS_ERR					6			//错误位置

//弹簧任务操作命令
#define SPRI_RED_PARA			1			//获取弹簧参数
#define SPRI_LOG_PARA			2			//弹簧拉力参数判断
#define SPRI_LOG_PARA1			3			//弹簧拉力参数判断
#define SPRI_CLR_PARA			4			//弹簧拉力数组清零



//弹簧拉力状态
#define SPRIFRCSTA_CLR			0			//弹簧拉力正常
#define SPRIFRCSTA_ERR			1			//弹簧拉力异常
#define SPRIFRCSTA_LIT			2			//弹簧拉力偏小
#define SPRIFRCSTA_MID			3			//弹簧拉力偏大
#define SPRIFRCSTA_BIG			4			//弹簧拉力偏大

//弹簧拉力数组指针
#define SPRIBUF_FST_AVER		12			//前面6个数组的平均值
#define SPRIBUF_END_AVER		13			//后面6个数组的平均值
#define SPRIBUF_FST_MIN			14			//前面6个数组的最小值
#define SPRIBUF_END_MAX			15			//后面6个数组的最大值
#define SPRIBUF_DAT				16			//处理缓存
#define SPRIBUF_POIN			17			//指针
#define SPRIBUF_STA				18			//弹簧拉力状态



//电机使能
#define M_EN					0			//输出
#define M_NA					1			//无输出


//继电器动作方向
#define BIG_MCR_UP					1			//
#define BIG_MCR_DW					0			//
#define LIT_MCR_UP					0			//
#define LIT_MCR_DW					1			//

#define MCR_UP		{BigMCR=BIG_MCR_UP; LitMCR=LIT_MCR_UP;  }			//
#define MCR_DW		{BigMCR=BIG_MCR_DW; LitMCR=LIT_MCR_DW;  }			//



#define M_CLOS 		{ BigM5u= M_NA; BigM3u= M_NA; LitM= M_NA;}			//
#define M_OPEN 		{ BigM5u= M_EN; BigM3u= M_EN; }						//
#define M_SLOW	 	{ BigM5u= M_NA; BigM3u= M_EN; LitM= M_EN; }			//

//电机监控
#define START_POSITION 35   //从第35次进入中断开始计算
#define END_POSITION (START_POSITION+TIMER_FRE*SPRING_CHECK_NUM ) //从第50次进入中断开始计算
#define TIMER_FRE  5        //计算周期
#define BASIC_SPRING_NUM 39   //初始值
#define BASIC_OFFSET_250MS 6 //每250ms测速环转到的圈数
#define SPRING_CHECK_NUM 8//取样次数

#ifdef GAN_CHANG_4M2
#define TWO_SPRING_MIN_NUM 90   //两条弹簧在第8次查询的时候，最少是90转
#define THREE_SPRING_LOWER_LIMIT 20 //4.2米为20 //4.0米为11//如果统计总的圈数差大于这个值，说明已经不是三根弹簧了
#define TWO_SPRINT_LOWER_LIMIT 46//4.2米值为 46 //4.0米值为28//如果统计总的圈数差大于这个值，说明已经不是两根弹簧了
#else
#define TWO_SPRING_MIN_NUM 86   //两条弹簧在第8次查询的时候，最少是90转
#define THREE_SPRING_LOWER_LIMIT 11 //4.2米为20 //4.0米为11//如果统计总的圈数差大于这个值，说明已经不是三根弹簧了
#define TWO_SPRINT_LOWER_LIMIT 28//4.2米值为 46 //4.0米值为28//如果统计总的圈数差大于这个值，说明已经不是两根弹簧了
#endif


typedef struct 
{
    u8 OffsetSum;       //多走的圈数和
	u8 Times;           //定时器计数
	u8 CurrentCounts;   //统计次数
	u8 CalcCounts;      //计算次数
    u8 ExFlag;          //是否执行定时器的标志位
	u8 Direction;       //电机转动方向	
	u8 FlagValue;       //统计超限次数	
	u8 OutFlag;         //电平信号转换
//	u8 BasicSpringNum;  //第一次落点的位置
//	u8 CurrentMotorType;//当前电机类型
//	u8 CurrentSprintType;//当前弹簧类型
//	u8 CurrentMotorPosition;//当前电机位置
//  u8 CurrentSpeRin;   //当前测速环转动圈数
//  u8 AverageValue;    //所计算圈数平均值    
}RepairMotor_t;



/**************************************************************************** 
* 使用端口定义
****************************************************************************/
#define	PORT_BTA1			P2
#define	B_BTA1				7
sbit	BTA1				= PORT_BTA1 ^ B_BTA1;

#define	PORT_BTA2			P2
#define	B_BTA2				6
sbit	BTA2				= PORT_BTA2 ^ B_BTA2;

#define	PORT_BTA3			P2
#define	B_BTA3				5
sbit	BTA3				= PORT_BTA3 ^ B_BTA3;

#define	PORT_MCR1			P2
#define	B_MCR1				3
sbit	MCR1				= PORT_MCR1 ^ B_MCR1;

#define	PORT_MCR2			P0
#define	B_MCR2				7
sbit	MCR2				= PORT_MCR2 ^ B_MCR2;

#define	PORT_SPE			P3
#define	B_SPE				2
sbit	SPE					= PORT_SPE ^ B_SPE;


#define BigM5u			BTA1
#define BigM3u			BTA2
#define LitM			BTA3
#define BigMCR			MCR1
#define LitMCR			MCR2


/**************************************************************************** 
* 变量定义/声明
****************************************************************************/
extern volatile u8  xdata mType;	//电机类型 1秒    1.8秒    3.8秒 
extern volatile u8  xdata mPosD;	//执行位置状态
extern volatile u8  xdata mPosD2;	//执行位置状态用作主副机控制用
extern volatile u8  xdata PosRal;	//实际位置状态
extern volatile u8  xdata mTask;	//电机当前执行的任务
extern volatile u8  xdata SpriBuf[19];//弹簧检测数组
extern volatile u16 data  mCorNum;	//电机正反转切换计时
extern volatile u16  data  mRunNum;	//电机运行时间限制
extern volatile u16 xdata mRunTe;	//电机运行时间缓存

extern volatile u16 data  StmSp;	//遇阻灵敏度--全速落闸时赋值
extern volatile u16 data  StmSp2;	//遇阻灵敏度--全速落闸时缓存
extern volatile u16 xdata StmSlow;	//遇阻灵敏度--落闸缓冲时赋值
extern volatile u16 xdata StmSlow2;	//遇阻灵敏度--落闸缓冲时缓存
extern volatile u8  xdata Stm;		//遇阻灵敏度--防抖
extern volatile u16 data  StmNum;	//遇阻计时
extern volatile u16 data  StmNum2;	//遇阻计时缓存
extern volatile u16 data  StmNum3;	//遇阻计时缓存
extern volatile u8  data  StmNumBuf[STMBUF_LEN];//遇阻计时-获取平均值数组
extern volatile u8  data  StmNumBuf2[STMBUF_LEN2];//遇阻计时-判断修正数组

extern volatile u8 data  SpeRinN;	//测速环转动圈数计量
extern volatile u8 xdata DwSlo;		//开始缓冲圈数计算(即开始设置LitM= M_EN;)
extern volatile u8 xdata DwSloBri;	//开始减速圈数计算(即开始设置BigM5u=M_NA )

extern volatile bit SpeSta;			//测速缓存比较标志

extern volatile RepairMotor_t gRepairMotor;  //判定当前是否需要抬闸







/**************************************************************************** 
* 函数原型
****************************************************************************/
void intMotor();
void mRunClk();
void mCtrClk();
void mTaskControl();	
void InitRepairMotor(void);

void MonitorTask(void);
void Monitor_clk(void);		//监控计数




#endif

