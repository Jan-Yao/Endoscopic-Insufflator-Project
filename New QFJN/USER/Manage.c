
#include "MainWin.h"
/***********************************************************************************/
/*	功 能：	定时服务器（产生“定时到”事件）				*/
/*	输 入：									*/
/*	返 回：									*/
/*	注 释：	事件间隔大于设定的时间间隔					*/
/**********************************************************************************/
void TimerServer(void)
{
	static u16 LastTickCounter=0;
	u16 TimePass;
	u8  i;
	TimePass = TickCounter - LastTickCounter;							       
	if (TickCounter != LastTickCounter) {
		LastTickCounter = TickCounter;		                          //time count transmit
		if (TimerPool[0].Interval != 0) {
			TimerPool[0].TickCounter += TimePass;
			if (TimerPool[0].TickCounter >= TimerPool[0].Interval) {
				TimerPool[0].TickCounter = 0;								            // 确保事件间隔不小于设定的时间间隔
				(*TimerPool[0].TimerProc)(0);
			}
		}		
		for (i=1; i<MAX_TIMER; i++) {
			if (TimerPool[i].Interval != 0) {
				TimerPool[i].TickCounter += TimePass;
				if (TimerPool[i].TickCounter >= TimerPool[i].Interval) {
					TimerPool[i].TickCounter = 0;							            	// 确保事件间隔不小于设定的时间间隔
					(*TimerPool[i].TimerProc)(i);
				}
			}
		}
	}
	
}

/*************************************************************************************/
/*	功 能：	创建定时器							 */
/*	输 入：	hWindow	 ___ 定时器所属或创建定时器的窗口的句柄			*/
/*					interval ___ 定时间隔			*/																			       			
/*					TimeProc ___ 定时到后所触发（调用）的回调定时函数指针，若为0，则发送消息到当前 */
/*					窗口的窗口函数，定时函数原型 void TimerProc(hTimer)  */
/*	返 回：	定时器的标识句柄						*/																						
/*	注 释：	需要精确定时的操作不能采用此类定时器方式，应直接由定时中断调用	*/
/*************************************************************************************/


u8 SetTimer(u8 hWindow,u16 Interval,TIMERPROCPTR TimerProc)
{
	u8 i;
	for (i=0; i<MAX_TIMER; i++) {						// 在定时器池中搜寻一个可用的位置
		if (TimerPool[i].Interval == 0) {
			TimerPool[i].TickCounter = 0;
			TimerPool[i].hWindow = hWindow;
			TimerPool[i].Interval = Interval;
			TimerPool[i].TimerProc= TimerProc;
			return(i);												// 若定时器创建成功，则返回它的句柄（序号）
		}
	}
	return (-1);								          // 创建失败
}


/****************************************************************************************/
/*	功 能：	释放定时器             						*/
/*	输 入：									*/
/*	返 回：									*/
/*	注 释：									*/
/****************************************************************************************/

void KillTimer(u8 hTimer)
{
	TimerPool[hTimer].Interval 	= 0;	// 清空定时间隔
	TimerPool[hTimer].TimerProc = 0;	// 清空定时时间句柄

}

/****************************************************************************************/
/*	功 能：	注册窗口                        				*/
/*	输 入：									*/
/*	输 出：									 */
/*	注 释：									 */
/****************************************************************************************/
BOOL RegisterWindow(WINDOW * Window)
{
	if (hActiveWindow < 14){
		if( hActiveWindow>0 || (u16)(&OpenWndCollection[0]->WindowProc)>0)hActiveWindow++;
		OpenWndCollection[hActiveWindow] = Window;
		return( 1 );
	}
	else {
		return ( 0 );
	}
}
/****************************************************************************************/
/*	功 能：	打开一个新窗口							*/
/*	输 入：									*/
/*	输 出：									*/
/*	注 释：									 */
/****************************************************************************************/
void OpenWindow(WINDOW * Window)
{
	Window->Active = TRUE;
}
/****************************************************************************************/
/*	功 能：	关闭当前窗口      						  */
/*	输 入：									 */
/*	输 出：									  */
/*	注 释：									 */
/****************************************************************************************/
void CloseWindow(void)
{
	if (hActiveWindow > 0) {
		OpenWndCollection[hActiveWindow]->Active = FALSE;
		OpenWndCollection[hActiveWindow] = 0;
		hActiveWindow--;																						
	}
}
/****************************************************************************************/
/*	功 能：	获取活动窗口                      				 */
/*	输 入：									*/
/*	输 出：									 */
/*	注 释：									*/
/****************************************************************************************/
WINPROCPTR ActiveWindowProc(void)
{
	return (OpenWndCollection[hActiveWindow]->WindowProc);
}
/****************************************************************************************/
/*	功 能：	刷新活动窗口                      				 */
/*	输 入：									*/
/****************************************************************************************/
void RefreshWindow(void)
{
	(*ActiveWindowProc())(Win_Refresh, 0);
}

/****************************************************************************************/
/*	功 能： 运行指示灯闪烁  						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void TimerProc_Run(u8 hTimer)
{
	HAL_IWDG_Refresh(&hiwdg); 
	
	float AirPreP;
	
	uPre_Value.RunF = (float)uPre_Value.RunH;
	AirPreP  = uPre_Value.RunF - uFlo_Value.RunF;
	if(AirPreP < 0)  AirPreP = 0;
	uPre_Value.DisH = (u8)(AirPreP/10);
	
	
  if(uPre_Value.Once	> 2300){   //3000=80mmHg;2300=60mmHg;1800=48mmHg;2000=53mmHg
			uAirPre.State = 1;
			if(uTime_PWM > 0){
				uAirPre.PWM = 400;  //关闭注气体
			}
			else{
				uAirPre.PWM = 0;
			}
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,600);  //15的流量泄气
	}
	else if(uPre_Value.DisH > uPre_Value.SetH){  
		if(uPre_Value.SetH >= 10){  
			if(uPre_Value.DisH > uPre_Value.SetH*1.5){       //显示大于设定的1.5倍，但前提是设定值大于10
				uAirPre.State = 1;
				if(uTime_PWM > 0){
					uAirPre.PWM = 400;  //关闭注气体
				}
				else{
					uAirPre.PWM = 0;
				}
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,500);  //小流量泄气
			}
			else{
				if(uAirPre.State == 1){ //气体没有超压，关闭泄气
					uAirPre.State = 0;
					__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);
				}
			}
		}
		else{ 
			if(uPre_Value.DisH > uPre_Value.SetH+5){          //显示大于设定+5
				uAirPre.State = 1;
				if(uTime_PWM > 0){
					uAirPre.PWM = 400;  //关闭注气体
				}
				else{
					uAirPre.PWM = 0;
				}
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,450);  //小流量泄气
			}
			else{
				if(uAirPre.State == 1){ //气体没有超压，关闭泄气
					uAirPre.State = 0;
					__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);
				}
			}
		}
	}
	else{
		if(uAirPre.State == 1){ //气体没有超压，关闭泄气
			uAirPre.State = 0;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);
		}
	}	

}
/****************************************************************************************/
/*	功 能： 温度扫描 						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void TimerProc_Temp(u8 hTimer)
{
	
		float AirPreP;
	  u8    Input_Stat;
	
	
		if(uLED_State > 0){
			uLED_State = 0;
			LED_ON();
		}
		else{
			uLED_State = 1;
			LED_OFF();
		}
		
		
		uFlo_Value.DisF = uFlo_Value.RunF;
		if(uFlo_Value.DisF < 0)  uFlo_Value.DisF = 0;
		
		uPre_Value.RunF = (float)uPre_Value.RunH;
		AirPreP  = uPre_Value.RunF - uFlo_Value.RunF;
		if(AirPreP < 0)  AirPreP = 0;
		uPre_Value.DisH = (u8)(AirPreP/10);
		
		SetTextInt32(1,1,uPre_Value.DisH,0,2); 			
		SetTextFloat(1,2,uFlo_Value.DisF,1,1);
		
		
		if(uPre_Value.DisH > uPre_Value.SetH + 3){
				if(uPre_State != Pre_High){
					AnimationPlayFrame(1,High_Sign, ON);
					AnimationPlayFrame(1,Low_Sign, OFF);
					uPre_State = Pre_High;            
				}
		}
		else if(uPre_Value.DisH < uPre_Value.SetH - 3){
				if(uPre_State != Pre_Low){
					AnimationPlayFrame(1,Low_Sign, ON);
					AnimationPlayFrame(1,High_Sign, OFF);
					uPre_State = Pre_Low;
				}
		}
		else{
				if(uPre_State == Pre_High){
					AnimationPlayFrame(1,High_Sign, OFF);
				}
				else if(uPre_State == Pre_Low){
					AnimationPlayFrame(1,Low_Sign, OFF);
				}
				else{
				}
				uPre_State = Pre_Normal;
		}
		
		
		uValue_GasF = uFlo_Value.DisF * 0.0033;   // 2s 加减一次
		uTotal_GasF = uTotal_GasF + uValue_GasF;
		
		uBack_GasH = uTotal_GasH;
		uTotal_GasH = (u32)uTotal_GasF;
		if(uBack_GasH != uTotal_GasH){
			SetTextInt32(1, 11, uTotal_GasH, 0, 3);  
		}
		
		switch(uPre_Limit_State)
		{
			case PreSet_Normal:
//				SetTextFcolor(1,3,WHITE);                                     //显示白色
				break;
			case PreSet_Limit:
//				SetTextFcolor(1,3,RED);                                       //显示红色
				break;
			case PreSet_Over_cap:                                             //显示2S红色 to 显示白色
				uPre_Limit_State = Pre_Normal;
				SetTextFcolor(1,3,WHITE);                                       //显示白色
				break;
			default:
				break;
		}
		
		Input_Stat = Input_YeTi_Read();
		if(Input_Stat == uJinYe_State){           //进液检测
			if(uJinYe_State == 1){                  //Input_Stat = uJinYe_State = 1
				AnimationPlayFrame(1,Warn_Sign,ON);	
			}
			else{                                   //Input_Stat = uJinYe_State = 0
//				AnimationPlayFrame(1,Warn_Sign,OFF);
			}
		}
		else{                                     //Input_Stat != uJinYe_State
			uJinYe_State = Input_Stat;
			AnimationPlayFrame(1,Warn_Sign,OFF);	
		}
}

/****************************************************************************************/
/*	功 能： 毫秒延时  						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/

void DelayMS(u16 Data) 
{
    u16 i,j;  
    for(i = Data;i>0;i--)
        for(j=1000;j>0;j--) ; 
}
/****************************************************************************************/
/*	功 能： 毫秒延时  						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void DelayTime(u16 Data)
{
		u16 i,j;  
	
    for(i = Data;i>0;i--){
			for(j=10000;j>0;j--);
			HAL_IWDG_Refresh(&hiwdg);  
		}			
}


