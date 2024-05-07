#include "MainWin.h"

static void DealWithPwmData(void);

/****************************************************************************************/
/*	功 能： PID算法 ，PWM信号处理  						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void TimerProc_PWM(u8 hTimer)
{
		if(uAirPre.Time_Run > 0) uAirPre.Time_Run --;   
		if(uAirPre.Time_Run >= 5){                              //4.5s ON  0.5s OFF
			DealWithPwmData();
		}
		else if(uAirPre.Time_Run < 2){   //停止注入              //一个循环后 4.5s ON 0.5s OFF
			uAirPre.Time_Run = 50;
		}
		else{
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);   //停止注气输入
		}

//    DealWithPwmData();
	
//		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,uAirPre.PWM);   //加热输入
//		__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,uAirPre.PWM);   //泄气输入
//		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);   //注气输入
		
	
}
/****************************************************************************************/
/*	功 能： PID算法					*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：   //460已经关断，为了保险选400.                                                                          */
/****************************************************************************************/
static void DealWithPwmData(void)
{
	
	
			static u8 Curr = 0;
			float Pout,Iout,Dout,Temp,AirPreP;
			
			uAirPre.CurP    = (float)uPre_Value.RunH;    //取当前气压值，转换成(float)格式  ADC压力自带*10效果
			AirPreP         = (float)uPre_Value.SetH;    //设置气压值
			uAirPre.TargetP = AirPreP * 10;
	
			if((uAirPre.CurP > uAirPre.TargetP/3)&&(uFlo_Value.RunF < uFlo_Value.SetF)){  //当为达到设定流量 且 与设定压力值相差不大时
				uAirPre.TargetP = uAirPre.TargetP + uFlo_Value.RunF;   //根据流量补偿 目标时流量为0 压力设定值=运行值
			}
			else{                                   //转换成流量调节 设定的最大流量输出
				uAirPre.CurP   	= uFlo_Value.RunF;    //取当前流量值
				uAirPre.CurP    = uAirPre.CurP*10;
				uAirPre.TargetP = uFlo_Value.SetF;    //设置流量值
				uAirPre.TargetP = uAirPre.TargetP*10;
			}
	
			Temp = uAirPre.DiffT[Curr]; 
			if(Curr+1>=20)	Curr = 0;
			else 			      Curr += 1;
			
			uAirPre.SumDiff    -= uAirPre.DiffT[Curr];              //循环替代，保证时间上最近的20个数据求和
			uAirPre.DiffT[Curr] = uAirPre.TargetP - uAirPre.CurP;   //设置气压值—实际运行气压值
			uAirPre.SumDiff    += uAirPre.DiffT[Curr];
			
			Pout = KP*uAirPre.DiffT[Curr];				    //比例项输出  P*error
			Iout = KI*uAirPre.SumDiff;					      //积分项输出  I*sum.error
			Dout = KD*(uAirPre.DiffT[Curr] - Temp);		//微分项输出  D*error-error.last
		
			
			if(Iout > 50)	Iout = 50;                 //设置上下限，防止振荡
			if(Iout < -50)	Iout = -50;
			
			uAirPre.PID = Pout+Iout+Dout;
			
			if(uAirPre.PID < -20)       uAirPre.PID = -20;  //设置每次增减的上下限
			else if(uAirPre.PID >= 20)  uAirPre.PID = 20;
			else{
			}
		
			uAirPre.PWM_F += uAirPre.PID;
				
			if(uAirPre.PWM_F > 999)       uAirPre.PWM_F = 999;
			else if(uAirPre.PWM_F < 400)  uAirPre.PWM_F = 400;
			
			uAirPre.PWM = (u16)uAirPre.PWM_F;
			
//			uAirPre.PWM = 650;
			
		 __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);   //注气输入

			
}

/****************************************************************************************/
/*	功 能： PID算法 ，PWM信号处理  						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void TimerProcT_PWM(u8 hTimer)
{
		
			static u8 Curr = 0;
			float Pout,Iout,Dout,Temp;
	
			uTempPre.CurP   	= uT_Value.RunF;    //取当前流量值，转换成(float)格式
			uTempPre.CurP    = uTempPre.CurP*10;

	
			Temp = uTempPre.DiffT[Curr]; 
			if(Curr+1>=20)	Curr = 0;
			else 			      Curr += 1;
			
			uTempPre.SumDiff    -= uTempPre.DiffT[Curr];
			uTempPre.DiffT[Curr] = uTempPre.TargetP - uTempPre.CurP;   //设置气压值—实际运行气压值
			uTempPre.SumDiff    += uTempPre.DiffT[Curr];
			
			Pout = KP*uTempPre.DiffT[Curr];				    //比例项输出
			Iout = KI*uTempPre.SumDiff;					      //积分项输出
			Dout = KD*(uTempPre.DiffT[Curr] - Temp);		//微分项输出
		
			
			if(Iout > 50)	Iout = 50;
			if(Iout < -50)	Iout = -50;
			
			uTempPre.PID = Pout+Iout+Dout;
			
			if(uTempPre.PID < -30)       uTempPre.PID = -30;
			else if(uTempPre.PID >= 30)  uTempPre.PID = 30;
			else{
			}
		
			uTempPre.PWM_F += uTempPre.PID;
				
			if(uTempPre.PWM_F > 999)       uTempPre.PWM_F = 999;
			else if(uTempPre.PWM_F < 0)  uTempPre.PWM_F = 0;
			
			uTempPre.PWM = (u16)uTempPre.PWM_F;
			
			
		 __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,uTempPre.PWM);   //加热输入
		
	
}





