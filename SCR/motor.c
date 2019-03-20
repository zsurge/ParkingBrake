
/****************************************************************************
* File:			motor.c														*
* Created:		2017-06-05													*
* Last Change:																*
* Author:		chen zhi peng 												*
* Description:	电机控制													*
****************************************************************************/

#include "MOTOR.H"

u8  xdata mType;	//电机类型 1秒    1.8秒    3.8秒 
u8  xdata mPosD;	//执行位置状态
u8  xdata mPosD2;	//执行位置状态用作主副机控制用
u8  xdata PosRal;	//实际位置状态
u8  xdata mTask;	//电机当前执行的任务
u8  xdata SpriBuf[19];//弹簧检测数组
u16 data  mCorNum;	//电机正反转切换计时
u16  data  mRunNum;	//电机运行时间限制
u16  xdata mRunTe;	//电机运行时间缓存

u16 data  StmSp;	//遇阻灵敏度--全速落闸时赋值
u16 data  StmSp2;	//遇阻灵敏度--全速落闸时缓存
u16 xdata StmSlow;	//遇阻灵敏度--落闸缓冲时赋值
u16 xdata StmSlow2;	//遇阻灵敏度--落闸缓冲时缓存
u8  xdata Stm;		//遇阻灵敏度--防抖
u16 data  StmNum;	//遇阻计时
u16 data  StmNum2;	//遇阻计时缓存
u16 data  StmNum3;	//遇阻计时缓存
u8  data  StmNumBuf[STMBUF_LEN];//遇阻计时-获取平均值数组
u8  data  StmNumBuf2[STMBUF_LEN2];//遇阻计时-判断修正数组

u8 data  SpeRinN;	//测速环转动圈数计量
u8 xdata DwSlo;		//开始缓冲圈数计算(即开始设置LitM= M_EN;)
u8 xdata DwSloBri;	//开始减速圈数计算(即开始设置BigM5u=M_NA )

bit SpeSta;			//测速缓存比较标志


volatile RepairMotor_t gRepairMotor;  //判定当前是否需要抬闸
volatile u8 xdata gCurrentSpringNum = 3;//设置弹簧条数，默认为3





void intMotor() 
{ 
	mType= MNA; mPosD= POS_INT; mPosD2= POS_INT; PosRal= POS_INT; mTask= MTASK_NA;
	StmNum= 0; StmNum2= 0; StmNum3= 0; mCorNum= 0; mRunNum= 0; SpeRinN= 0; 
	memset(StmNumBuf,0,sizeof(StmNumBuf));  memset(StmNumBuf2,0,sizeof(StmNumBuf2));  memset(&SpriBuf,0,sizeof(SpriBuf));
	StmSp= 0; StmSp2= 0; mRunTe= 0; DwSlo= 0; Stm= 0; StmSlow= 0; StmSlow2= 0; SpeSta= 0;
	InitRepairMotor();
}

void InitRepairMotor(void)
{
	memset(&gRepairMotor,0x00,sizeof(gRepairMotor));    
}



void mRunClk()	{ if(mRunNum) mRunNum--; }					//电机运行时间时钟
void mCtrClk()	{ if(mCorNum) mCorNum--;  else StmNum++; }	//电机正反转切换时钟   //遇阻时钟	




//**********************************************//
//函数名:void StmInt(u8 type)
//作用:遇阻灵敏度的冗余计算
//输入:type-机芯类型
//返回: 无
//**********************************************//
void StmInt(u8 type)
{
	if(type== M1S)
		{
			StmSp= StmSp2 + STP1S_REDUNDANCY_TIME;			//正常落闸时的遇阻灵敏度 = 实测测速环半圈的周期 + 该机芯的冗余值
//			StmSlow2= StmSp2 + SLOW1S_REDUNDANCY_TIME;		//减速、缓冲时的遇阻灵敏度 = 正常落闸时的遇阻灵敏度 * 3
			StmSlow2= SpriBuf[SPRIBUF_END_MAX] + 60;
		}
	else if(type== M18S)
		{
			StmSp= StmSp2 + STP18S_REDUNDANCY_TIME;
//			StmSlow2= StmSp2 + SLOW18S_REDUNDANCY_TIME;
			StmSlow2= SpriBuf[SPRIBUF_END_MAX] + 60;

		}
	if(StmSlow2 > STMMAXTIME) StmSlow2= STMMAXTIME;			//减速、缓冲的遇阻灵敏度限制参数
}





//**********************************************//
//函数名:void KnowMtyp(u8 sta,u8 Prg,u8 pos)
//作用:开始减速、缓冲的计算、赋值
//输入:bm-拨码补偿倍数；  type-不同机芯类型参数补偿
//返回: 无
//**********************************************//
void DwSloInt(u8 bm,u8 type)
{
	DwSloBri*=bm; 					//拨码的倍数补偿
	DwSloBri+=type; 				//提前减速的圈数= 拨码补偿圈数 + 机芯补偿圈数
	DwSlo= SpeRinN - DwSloBri/3*2;	//开始缓冲的圈数= 测速环总圈数 - 提前减速圈数的2/3
	DwSloBri= SpeRinN - DwSloBri;	//开始减速的圈数= 测速环总圈数 - 提前减速圈数
}


//**********************************************//
//函数名:  SpringTest(u8 cmd)
//作用:弹簧拉力检测
//输入:cmd-操作命令
//返回:无
//**********************************************//
u8 SpringTest(u8 cmd)
{
	u16 data dat=0;
	switch(cmd)
		{
			case SPRI_RED_PARA:
				if(SpeRinN!= SpriBuf[SPRIBUF_DAT])
					{
						SpriBuf[SPRIBUF_DAT]= SpeRinN;
						SpriBuf[SpriBuf[SPRIBUF_POIN]]= StmNum2;
						SpriBuf[SPRIBUF_POIN] ++;
						if(SpriBuf[SPRIBUF_POIN]== 12) SpriBuf[SPRIBUF_POIN]= 6;
					}
				break;
				
			case SPRI_LOG_PARA:
				//********求前6圈的平均值 、最小值*************//
				SpriBuf[SPRIBUF_FST_MIN]= 255;
				for(SpriBuf[SPRIBUF_POIN]=0;SpriBuf[SPRIBUF_POIN] < 6;SpriBuf[SPRIBUF_POIN]++)
					{
						dat+= SpriBuf[SpriBuf[SPRIBUF_POIN]];
						if(SpriBuf[SpriBuf[SPRIBUF_POIN]] < SpriBuf[SPRIBUF_FST_MIN]) SpriBuf[SPRIBUF_FST_MIN]= SpriBuf[SpriBuf[SPRIBUF_POIN]];
					}
				dat/= 6;
				SpriBuf[SPRIBUF_FST_AVER]= dat;

				//********求后6圈的平均值、最大值*************//
				SpriBuf[SPRIBUF_END_MAX]= 0;
				for(SpriBuf[SPRIBUF_POIN]=6;SpriBuf[SPRIBUF_POIN] < 12;SpriBuf[SPRIBUF_POIN]++)
					{
						dat+= SpriBuf[SpriBuf[SPRIBUF_POIN]];
						if(SpriBuf[SpriBuf[SPRIBUF_POIN]] > SpriBuf[SPRIBUF_END_MAX]) SpriBuf[SPRIBUF_END_MAX]= SpriBuf[SpriBuf[SPRIBUF_POIN]];
					}
				dat/= 6;
				SpriBuf[SPRIBUF_END_AVER]= dat;
				
				//********判断拉力方法*************//
				if((SpriBuf[SPRIBUF_FST_AVER] > 20 && SpriBuf[SPRIBUF_FST_AVER] < 80) || SpriBuf[SPRIBUF_END_AVER] > 220)
					{
						if(SpriBuf[SPRIBUF_END_MAX] > 190 ) 
							{ SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_ERR; break; }	//拉力异常
							
						else if(SpriBuf[ SPRIBUF_END_MAX] > 150)
							{ SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_BIG; break; }	//拉力偏大
							
						else if(SpriBuf[SPRIBUF_END_MAX] > 110)
							{ SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_MID; break; }	//拉力中等	
							
						else if(SpriBuf[SPRIBUF_END_MAX] > 90 )
							{ SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_LIT; break; }	//拉力偏小

						else SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_ERR;
					}
				else SpriBuf[SPRIBUF_STA]= SPRIFRCSTA_ERR;
				break;

			case SPRI_LOG_PARA1:
				SpriBuf[SPRIBUF_END_MAX]= 0;
				for(SpriBuf[SPRIBUF_POIN]=0;SpriBuf[SPRIBUF_POIN] < 12;SpriBuf[SPRIBUF_POIN]++)
					{
						if(SpriBuf[SpriBuf[SPRIBUF_POIN]] > SpriBuf[SPRIBUF_END_MAX]) SpriBuf[SPRIBUF_END_MAX]= SpriBuf[SpriBuf[SPRIBUF_POIN]];
					}
				break;
				
			case SPRI_CLR_PARA:
				memset(&SpriBuf,0,sizeof(SpriBuf));
				break;
				
			default: break;
		}
	return SpriBuf[SPRIBUF_STA];
}



//**********************************************//
//函数名:void KnowMtyp(u8 sta,u8 Prg,u8 pos)
//作用:识别机芯类型，并赋值运行参数
//输入:sta-操作状态；  prg-识别过程；  pos-到达位置
//返回: 无
//**********************************************//
void KnowMtyp(u8 sta,u8 Prg,u8 pos)	//机芯类型识别及参数赋值
{
	if(sta== MNA)					//闸杆到位后，准备判断机芯类型
		{
			if(mType== Prg)			//识别过程
				if(PosRal== pos)	//到达位置
					{ 
						switch(SpringTest(SPRI_LOG_PARA))
							{
								case SPRIFRCSTA_BIG: err_volu(ERR_SPRI_BIG); break;
								case SPRIFRCSTA_LIT: err_volu(ERR_SPRI_LIT); break;
								case SPRIFRCSTA_MID: err_volu(ERR_SPRI_MID); break;
								case SPRIFRCSTA_ERR: err_volu(ERR_SPRI_ERR); break;
								default: break;
							}
						if(SpeRinN < 80)  						//单个行程，电机轴的圈数少于80圈为1秒机芯
							{
								mType= M1S; 					//识别机芯类型-赋值
								mRunTe=M1SRUNTIME;   			//赋值电机运行超时时间
								if(!StmSp2) StmSp2=50;			//第一次落闸遇阻时间未识别时，强行赋值
								StmInt(M1S);					//获取遇阻灵敏度
								DwSloInt(5,15);					//获取开始减速、缓冲的参数
							}
						else if(SpeRinN < 130) 					//单个行程，电机轴的圈数少于130圈为1.8秒/3.8秒机芯
							{
								mType= M18S; 					//识别机芯类型-赋值
								mRunTe=M18SRUNTIME; 			//赋值电机运行超时时间
								if(!StmSp2) StmSp2=30;			//第一次落闸遇阻时间未识别时，强行赋值
								StmInt(M18S);					//获取遇阻灵敏度
								DwSloInt(10,35);				//获取开始减速、缓冲的参数
							}
						else			   						//识别异常时
							{
								mType= M18S; 					//识别机芯类型-赋值
								mRunTe=M18SRUNTIME; 			//赋值电机运行超时时间
								if(!StmSp2) StmSp2=50;			//第一次落闸遇阻时间未识别时，强行赋值
								StmInt(M18S);					//获取遇阻灵敏度
								DwSloInt(10,35);				//获取开始减速、缓冲的参数
							}
					}
		}
	else								//标记识别状态: 目前只能在落闸时识别机芯类型
		{
			if(mType < Prg)				//机芯类型未识别
				{
					if(PosRal== pos) 	//开始识别位置
						{
//							if(sta== MLN_UP) mType= MLN_UP;  //抬闸过程中识别机芯类型----取消，会影响落闸缓冲圈数获取精度
							if(sta== MLN_DW) mType= MLN_DW; 
						}
					mRunTe=MAXRUNTIME;		//机芯类型未知时，获取默认运行时间参数
					StmSp=STMMAXTIME;		//机芯类型未知时，获取默认灵敏度
				}
		}
	
}

//**********************************************//
//函数名:u8 StmCtr(u16 stmlm)
//作用:遇阻判断函数
//输入:灵敏度值
//返回:遇阻状态: 1-遇阻；   0-正常
//**********************************************//
u8 StmCtr(u16 stmlm)
{
	if(!mCorNum)	//为零时，电机起步结束，开始判断是否遇阻
		{
			if(StmNum > stmlm)  //实测测速环半圈的周期 > 设定的周期 = 遇阻
				{ if(Stm) Stm--;  else return 1; }		//遇阻防抖判断
			else Stm=2;
		}
	else Stm=2; 

	return 0;
}




//**********************************************//
//函数名:  u8 CalcuStm(u8 cmd)
//作用:自适应遇阻灵敏度，提高不同机芯兼容性
//输入:cmd-操作命令
//返回:状态/测速环半圈的平均周期
//**********************************************//
u8 CalcuStm(u8 cmd)
{
	u8 i= 0;
	switch(cmd)
		{
			// 	每次落闸时，计量测速环10圈的平均周期；
			case STMBUF_AVER_SPE:
				if(StmNumBuf[STMBUF_POIN]== STMBUF_POST)
					{
						StmNum3=0;
						for(i=0; i < (STMBUF_POST-1); i++)
							{
								if(StmNumBuf[i] > StmNum3) StmNum3= StmNumBuf[i];
							}
						StmNumBuf[STMBUF_DATA]= StmNum3;
						StmNumBuf[STMBUF_POIN]= STMBUF_POST+1;
						return StmNumBuf[STMBUF_DATA];			//返回测速环半圈的平均周期
					}
				else if(StmNumBuf[STMBUF_POIN] < STMBUF_POST)
					{
						if(StmNumBuf[STMBUF_POST]!= SpeRinN)
							{
								StmNumBuf[STMBUF_POST]= SpeRinN;
								StmNumBuf[StmNumBuf[STMBUF_POIN]]= StmNum2;
								StmNumBuf[STMBUF_POIN]++;
							}
					}
				return StmNumBuf[STMBUF_DATA];	//返回测速环半圈的平均周期
				break;

			// 	每正常落闸2次后，对比当前工作灵敏度，满足修正条件时，修正工作灵明度；//
			case STMBUF_CALC_SPE:		
				if(StmSp== STMMAXTIME + 1) StmInt(mType);		//遇阻后，在下一次落闸成功后重新赋值遇阻灵敏度
				if(StmSp== STMMAXTIME) break;					//上电第一次落闸，不计算
				
				if(StmNumBuf2[2] < 2)
					{
						if(StmSp2 > 20)
							{
								StmNumBuf2[StmNumBuf2[2]]= StmSp2;
								StmNumBuf2[2]++;
							}
					}
				if(StmNumBuf2[2]== 2)
					{
				//		if( (StmNumBuf2[0] > StmSp2 && StmNumBuf2[1] > StmSp2 ) || (StmNumBuf2[0] < StmSp2 && StmNumBuf2[1] < StmSp2 ) )
						if(StmNumBuf2[0] > StmSp2 && StmNumBuf2[1] > StmSp2)
							{
								StmNum3= StmNumBuf[0] + StmNumBuf[1];
								StmNum3/= 2;
								StmSp2= StmNum3;
							}
						memset(StmNumBuf2,0,sizeof(StmNumBuf2));
						return 1;
					}
				return 0;
				break;

				//数组1清零
			case STMBUF_CLR1:
				memset(StmNumBuf,0,sizeof(StmNumBuf));
				break;

				//数组2清零
			case STMBUF_CLR2:	
				StmSlow2= 0;					//遇阻灵敏度--落闸缓冲时缓存清零，重新学习					
				StmSp= STMMAXTIME+1;			//遇阻后暂时把遇阻灵明度调到最大；在下一次落闸成功后在计算遇阻灵敏度并赋值
				memset(StmNumBuf2,0,sizeof(StmNumBuf2));
				break;
			default: break;
		}
	return 0;
}





void mTaskControl()			//电机任务控制
{
	if(SPE!=SpeSta) 
		{ 
			SpeSta= SPE; StmNum2= StmNum; StmNum= 0; SpeRinN++; 
//			speak(3,1);
		}

	switch(mTask)
		{
			case MTASK_NA:  
				if(PosRal== POS_HOR) mTask= MTASK_DS; 
				if(PosRal== POS_VER) mTask= MTASK_US; 
				break;
/***********************************************************************************************/
//					抬闸		
/***********************************************************************************************/
			case MTASK_UP:
				InitRepairMotor();
                gRepairMotor.ExFlag = 1;
				gRepairMotor.Direction = POS_UP;				
				BrakeClr();
				KnowMtyp(MLN_UP,MTPYREAD,POS_HOR);
				//GetLimitValue(mType);//获取边界值
				SpringTest(SPRI_CLR_PARA);
				SpeRinN= 0;
				mCorNum= M_OF_TIME;
				mTask= MTASK_UP1;
				mPosD2= POS_UP;
				M_CLOS;
				break;
			case MTASK_UP1:
				if(!mCorNum) { MCR_UP; mCorNum= M_ON_TIME;  mTask= MTASK_UP2; }
				break;
			case MTASK_UP2:
				if(!mCorNum)
					{
						mRunNum= mRunTe;				//大电机运行时间限制
						mCorNum= M_CV_UP_TIME;			//正反转切换起步时间
						mTask= MTASK_UP3;
						if(mPosD== POS_DW) 				//在落闸过程中抬闸
							{
							 	LitM= M_EN;				//落/抬切换时，小电机先抬闸1秒，抵消落闸时杆的惯性
							}
						mPosD= POS_UP;
						M_OPEN;
					}
				break;
			case MTASK_UP3:
				if(!mCorNum) LitM= M_NA;
				if(!mRunNum) { mPosD=POS_INT; err_volu(ERR_UP_FAIL); }		//抬闸超时失败
				else if(StmCtr(StmSp + StmSlow+80))
					{
						err_volu(ERR_DW_DRAG); 
					}
				else;
				break;	
/***********************************************************************************************/
//					抬闸缓冲		
/***********************************************************************************************/
			case MTASK_US:
				KnowMtyp( MNA,MLN_UP,POS_VER);
				LitM= M_NA;
				BigM5u= M_NA;
				mCorNum= M_OF_TIME;
				mTask= MTASK_US1;
				mPosD= POS_VER;
				mPosD2= POS_VER;
				break;
			case MTASK_US1:
				if(!mCorNum) { MCR_UP; mCorNum= M_ON_TIME; mTask= MTASK_US2; }
				break;
			case MTASK_US2:
				if(!mCorNum)
					{
						M_SLOW;
						mRunNum= M_LIT_UP_TIME;
						mTask= MTASK_US3;
					}
				break;
			case MTASK_US3:
				if(!mRunNum)
					{
						LitM= M_NA;
						if(PosRal== POS_VER) mTask= MTASK_US5;
						else { mTask= MTASK_US4; mPosD=POS_INT; }
					}
				else if(PosRal!= POS_VER) { mPosD=POS_INT; Event(ITASK_INER_DW); BrakeClr(); }
				break;
			case MTASK_US4:
				if(PosRal== POS_VER) mTask= MTASK_US5; 
				 err_volu(ERR_US_FAIL);              	  //抬闸缓冲失败
				break;
			case MTASK_US5:
				 if(PosRal!= POS_VER) { mPosD=POS_INT; Event(ITASK_INER_DW); BrakeClr(); break; }
				 if(mPosD== POS_INT) mPosD= POS_VER;
//				 Timer2_Zero();
				break;
				
/***********************************************************************************************/
//					      落闸		
/***********************************************************************************************/				
			case MTASK_DW:
				InitRepairMotor();
                gRepairMotor.ExFlag = 0;
				gRepairMotor.Direction = POS_DW;                
				BrakeClr();
				KnowMtyp(MLN_DW,MTPYREAD,POS_VER);
				CalcuStm(STMBUF_CLR1);
				SpringTest(SPRI_CLR_PARA);
				SpeRinN= 0;
				mCorNum= M_OF_TIME;
				mTask= MTASK_DW1;
				mPosD= POS_DW;
				mPosD2= POS_DW;
				M_CLOS;
				break;
			case MTASK_DW1:
				if(!mCorNum) { MCR_DW; mCorNum= M_ON_TIME; mTask= MTASK_DW2; }
				break;
			case MTASK_DW2:
				if(!mCorNum)
					{
						mRunNum= mRunTe;
						mCorNum= M_CV_DW_TIME;
						mTask= MTASK_DW3;
						M_OPEN;
					}
				break;
			case MTASK_DW3:
				if(mType > MTPYREAD) 				 //已识别机芯类型时，操作如下
					{
						if(SpeRinN < 26) 			 //起步时，前26圈的遇阻灵敏度 = 默认灵敏度 + 工作灵敏度 
							StmSlow= STMMAXTIME;
						else if(SpeRinN > DwSlo)	 //开始缓冲阶段
							{
								LitM= M_EN;			 //缓冲
								SpringTest(SPRI_RED_PARA);	//获取弹簧的拉力数据
							}
						else if(SpeRinN > DwSloBri)	 //开始减速阶段
							{
								BigM5u= M_NA; 		 //减速
								SpringTest(SPRI_RED_PARA);	//获取弹簧的拉力数据
								StmSlow= StmSlow2;	 //落闸缓冲时，遇阻灵敏度调低(数值越大，灵敏度越低)
							}
						else StmSlow= 0;			 //在正常落闸阶段，遇阻灵敏度的减速、缓冲补偿清零
						if(!mCorNum) StmSp2= CalcuStm(STMBUF_AVER_SPE);	//获取落闸时平均速度
					}
				else 								 //未识别机芯类型时，操作如下
					{
						if(SpeRinN < 20) 			 //起步时，前20圈的遇阻灵明度 = 默认灵敏度的2倍
							StmSlow= STMMAXTIME;
						else StmSlow= 0;
						if(SpeRinN > 40) 			 //运行40圈后，用缓冲的方式落闸，确保首次上电落闸平稳
							{ 
								BigM5u= M_NA; LitM= M_EN; //首次上电落闸提前减速和缓冲
								SpringTest(SPRI_RED_PARA);	//获取弹簧的拉力数据
							}
						if(!mCorNum) StmSp2= CalcuStm(STMBUF_AVER_SPE);		//获取第一次落闸的平均速度
					}

				if(!mRunNum)   						//超时停止落闸
					{
						mPosD= POS_INT; 
						BigM5u= M_NA; 
						LitM= M_NA; 
						err_volu(ERR_DW_FAIL); 
					}
				else if(StmCtr(StmSp + StmSlow))   //落闸遇阻
					{
						if(iTask== ITASK_COER_UP) break;	//水平位强制抬闸任务时，不执行遇阻任务
						Event(ITASK_DRAG_UP); 
						err_volu(ERR_DW_DRAG); 
						CalcuStm(STMBUF_CLR2);	//遇阻后，遇阻灵敏度判断修正数组清零
						DgStaClr();
						if(ViceSta==VICE_STA_EN) ViceSta=VICE_STA_DRAG;
						break;
					}
				else;
				break;
				
/***********************************************************************************************/
//						  落闸缓冲
/***********************************************************************************************/	
			case MTASK_DS:
				SpringTest(SPRI_LOG_PARA1);
				if(CalcuStm(STMBUF_CALC_SPE)) StmInt(mType);	//每落闸两次后，计算遇阻灵敏度是否要修正
				KnowMtyp(MNA,MLN_DW,POS_HOR);
				LitM= M_NA;
				BigM5u= M_NA;
				mCorNum= M_OF_TIME;
				mTask= MTASK_DS1;
				mPosD= POS_HOR; 
				mPosD2= POS_HOR; 
				break;
			case MTASK_DS1:
				if(!mCorNum) { MCR_DW; mCorNum= M_ON_TIME; mTask= MTASK_DS2; }
				break;
			case MTASK_DS2:
				if(!mCorNum)
					{
						M_SLOW;
						mRunNum= M_LIT_DW_TIME;
						mTask= MTASK_DS3;
					}
				break;
			case MTASK_DS3:
				if(!mRunNum)
					{
						LitM= M_NA;
						if(PosRal== POS_HOR) mTask= MTASK_DS5;
						else { mTask= MTASK_DS4; mPosD=POS_INT; }
					}
				else if(PosRal!= POS_HOR) { mPosD= POS_INT; Event(ITASK_COER_UP); BrakeClr();}
				break;
			case MTASK_DS4:
				if(PosRal== POS_HOR) mTask= MTASK_DS5;
	 			 err_volu(ERR_DS_FAIL);  	//抬闸缓冲失败
				break;
			case MTASK_DS5:
				if(mPosD== POS_INT) mPosD= POS_HOR;
				if(PosRal!= POS_HOR) { mPosD= POS_INT; Event(ITASK_COER_UP); BrakeClr(); }
//				Timer2_Zero();
				break;
				
			default:  break;
		}
}

void Monitor_clk(void)	     //监控计数
{
    if(gRepairMotor.Times)
    {
        gRepairMotor.Times--;
    }
}

#define START_POSITION 35   //从第50次进入中断开始计算
#define END_POSITION (START_POSITION+TIMER_FRE*SPRING_CHECK_NUM ) //从第50次进入中断开始计算
#define TIMER_FRE  5        //计算周期
#define THREE_SPRING_LOWER_LIMIT 11//16+2 //如果统计总的圈数差大于这个值，说明已经不是三根弹簧了
#define TWO_SPRINT_LOWER_LIMIT 28//26+2 //如果统计总的圈数差大于这个值，说明已经不是两根弹簧了
#define BASIC_SPRING_NUM 39   //初始值
#define BASIC_OFFSET_250MS 6 //每250ms测速环转到的圈数
#define SPRING_CHECK_NUM 8//取样次数


void MonitorTask(void)
{

//    if(gRepairMotor.CurrentCounts == 0 && gRepairMotor.Direction == POS_DW)
//    { 
//        //打印当前测速环值
//        Uart_Print(3,gRepairMotor.Direction);   
//        gRepairMotor.CurrentCounts = 5;

//    }

    if(gRepairMotor.Times == 0 && gRepairMotor.ExFlag == 0 && gRepairMotor.Direction == POS_DW) //测速环转动才开始进来
    {
        gRepairMotor.CurrentCounts++;
        
        if((gRepairMotor.CurrentCounts >= START_POSITION) && (gRepairMotor.CurrentCounts%TIMER_FRE == 0) && (gRepairMotor.CurrentCounts < END_POSITION))//39
        {   
            gRepairMotor.CalcCounts++;
            if(SpeRinN > BASIC_SPRING_NUM + BASIC_OFFSET_250MS*gRepairMotor.CalcCounts)
            {
                gRepairMotor.FlagValue += 1;//这里应该不会出现，如果出现就是大问题
            }   

            gRepairMotor.LastSpeRin = SpeRinN;
            
            //计算当前多走了多少圈
            if(gRepairMotor.LastSpeRin > (BASIC_SPRING_NUM+BASIC_OFFSET_250MS*(gRepairMotor.CalcCounts-1)))
            {                
                gRepairMotor.AverageValue += gRepairMotor.LastSpeRin-(BASIC_SPRING_NUM+BASIC_OFFSET_250MS*(gRepairMotor.CalcCounts-1));                
            }
            
            //同样的，若是圈数走的太多，就直接报错
            if (gRepairMotor.FlagValue >= 2 || gRepairMotor.AverageValue > TWO_SPRINT_LOWER_LIMIT) //不需完全走完判定，可以直接认为是有问题的 
            {       
                DZP_ER;
                Event(ITASK_DG_UP);
                Uart_Print(3,25);  
                err_volu(ERR_SPRI_ERR);
                
            }
            
            //两根弹簧报警
            if(gRepairMotor.AverageValue >= THREE_SPRING_LOWER_LIMIT)
            {

                //打印当前测速环值  
                Uart_Print(3,21);  
                //报警，只有两根弹簧   
                err_volu(ERR_SPRI_LIT);
                DZP_ER;
            }
           

            if(gRepairMotor.CalcCounts == SPRING_CHECK_NUM)
            {
                gRepairMotor.ExFlag = 1;
            }
            
            //打印当前测速环值  
            Uart_Print(3,gRepairMotor.Direction);            

        }    
        gRepairMotor.Times = 5;
    }    
}


