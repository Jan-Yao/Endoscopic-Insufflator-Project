#include "MainWin.h"

static void WindowProc(u8,u8);
static void DispatchKey(u8 Key);
static void RefreshState(void);
/****************************************************************************************/
/*	描 述：	在本界面等待APB命令和监视令牌的到来                                       */ 
/*	注 释：	在本界面：                          */
/****************************************************************************************/
static WINDOW Window = {   WindowProc,
	                         FALSE,
							(u16 *)winWork,
                           };
/****************************************************************************************/
/*	功 能：	窗口加载函数																																*/
/*	输 入：																																							*/
/*	返 回：																																							*/
/*	注 释：	                                                    												*/
/****************************************************************************************/
void winWork(void)
{
	
	
	if (Window.Active)return;
	if (RegisterWindow(&Window)) {
		  OpenWindow( &Window );
	}	
	
		Control_ON();
		uSmoke_State = 0;	  //打开控制阀
		uTime_PWM  		 = SetTimer(0,  10,TimerProc_PWM);    //刷新鼠标
		uAirPre.Time_Run = 50;//注气时间5S。
	  
	   uAirPre.PWM_F = uFlo_Value.SetF*5+450;
		  __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);   //注气输入
	  
}
/****************************************************************************************/
/*	功 能：																																			        */
/*	输 入：																										                      	*/
/*	输 出：																																							*/
/*	注 释：																																							*/
/****************************************************************************************/
static void WindowProc(u8 Type,u8 nRecive)
{ 
	switch ( Type ){					// 根据信息类型，提取及处理相应的信息
		case 1:
			DispatchKey(nRecive);
			break;
		case 2:
			RefreshState();
	
			break;	
	
		default:
			break;
	}	
}
/****************************************************************************************/
/*	功 能：	分发键盘输入																																*/
/*	输 入：																																							*/
/*	返 回：																																							*/
/*	注 释：																																							*/
/****************************************************************************************/
static void DispatchKey(u8 Key)
{
	switch ( Key ){					// 根据信息类型，提取及处理相应的信息	
	
			case  0x05:       	//气压设置减
				if(uPre_Limit_State == PreSet_Normal){
					if(uPre_Value.SetH < 30){
						uPre_Value.SetH ++;
						SetTextInt32(1,3,uPre_Value.SetH,0,2);
						if (uPre_Value.SetH == 15){
							uPre_Limit_State = PreSet_Limit;  //触发压力上限限制
							SetTextFcolor(1, 3, RED);      //显示压力红线
						}
					}	
				}
				else{
					uPre_Limit_State = PreSet_Over_cap;   //解除压力上限限制
				}
							
					break;				
			case  0x06:       	//气压设置加
				uPre_Limit_State = PreSet_Normal;
				if(uPre_Value.SetH > 1){
					uPre_Value.SetH --;
					SetTextInt32(1,3,uPre_Value.SetH,0,2);
					if (uPre_Value.SetH == 15){
						uPre_Limit_State = PreSet_Limit;  //触发压力上限限制
						SetTextFcolor(1, 3, RED);      //显示压力红线
					}
					else if (uPre_Value.SetH == 14){
						SetTextFcolor(1, 3, WHITE);      //显示压力红线
					}
				}
					break;		
			case  0x07:       	//流量设置减,
				if(uFlo_Value.SetF < 60){
					uFlo_Value.SetF =  uFlo_Value.SetF + 0.1;
					SetTextFloat(1,4,uFlo_Value.SetF,1,1);
				}
			
					break;
			case  0x08:      	//流量设置加，
				if(uFlo_Value.SetF >= 0.2){
					uFlo_Value.SetF =  uFlo_Value.SetF - 0.1;
					SetTextFloat(1,4,uFlo_Value.SetF,1,1);
				}
			
					break;	
			case  0x0C:     	 //气体总量清零
				uBack_GasH = 0;
				uTotal_GasF = 0;
			  uTotal_GasH = 0;
				SetTextInt32(1, 11, uTotal_GasH, 0, 3); 
			
					break;		
			case  0x0D:   		//启动
				Control_Paiyan_OFF();
			  __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
				AnimationPlayFrame(1,Smoke_Sign,OFF);
				AnimationPlayFrame(1,Work_Sign,OFF);
				CloseWindow();
					break;	
			case  0x0F:   		//排烟
				if(uSmoke_State == 0){
					if(uFlo_Value.SetF >= 0){
						uSmoke_State = 1;	
						Control_Paiyan_ON();
						__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,500);
						AnimationPlayFrame(1,Smoke_Sign,ON);
					}
					else{
						uSmoke_State = 0;	
						Control_Paiyan_OFF();
						__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
						AnimationPlayFrame(1,Smoke_Sign,OFF);
						BUZ_ON();
						DelayTime(200); 
						BUZ_OFF();
					}
				}
				else{
					uSmoke_State = 0;	
					Control_Paiyan_OFF();
					__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
					AnimationPlayFrame(1,Smoke_Sign,OFF);
					
				}
					break;
			case  0x13:   		//低压模式
				uPre_Limit_State = PreSet_Normal;
			  SetTextFcolor(1, 3, WHITE);
				uPre_Value.SetH = 5;	
				uFlo_Value.SetF = 4.0;
				SetTextInt32(1,3,uPre_Value.SetH,0,2);                                      //显示预设值                                    
				SetTextFloat(1,4,uFlo_Value.SetF,1,1);
					break;
			case  0x14:   		//标准模式
				uPre_Limit_State = PreSet_Normal;
			  SetTextFcolor(1, 3, WHITE);
				uPre_Value.SetH = 13;	
				uFlo_Value.SetF = 20.0;	
				SetTextInt32(1,3,uPre_Value.SetH,0,2);                                      //显示预设值                                    
				SetTextFloat(1,4,uFlo_Value.SetF,1,1);
					break;

			case  0x87:       	//流量设置减,
				if(uFlo_Value.SetF < 60){
					uFlo_Value.SetF =  uFlo_Value.SetF + 1;
					if(uFlo_Value.SetF > 60) uFlo_Value.SetF = 60; 
					SetTextFloat(1,4,uFlo_Value.SetF,1,1);
				}
			
					break;
			case  0x88:      	//流量设置加，
				if(uFlo_Value.SetF > 1){
					uFlo_Value.SetF =  uFlo_Value.SetF - 1;
					SetTextFloat(1,4,uFlo_Value.SetF,1,1);
				}
				
				
			default:
					break;
	}
}
/****************************************************************************************/
/*	功 能：	刷新状态											*/
/*	输 入：												*/
/****************************************************************************************/
static void RefreshState(void)
{
	if(WinCounter >= 2000){           //20s周期
		WinCounter = 0;
	}
	if((WinCounter%20 == 0) && (WinCounter != WinCounter_last)){          //200m检测一次
		if(uGas_Value.Back == 0){      //低气源报警和自动关机  高优先级
			Speak_PWM_Count = 0;
			Speak_Off();
			BUZ_ON();
			DelayTime(500); 
			BUZ_OFF();
			Control_Paiyan_OFF();
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
			AnimationPlayFrame(1,Smoke_Sign,OFF);
			AnimationPlayFrame(1,Work_Sign,OFF);
			AnimationPlayFrame(1,ON_and_OFF,OFF);
			CloseWindow();
		}
		else if(uGas_Value.Back == 1){ //ON=0.2s OFF=2s
			if(WinCounter%200 == 0){
				BUZ_ON();
			}
			else{
				BUZ_OFF();
			}
		}
		else{                         //超压报警 低优先级
			if(uPre_State == Pre_High){
        Speak_PWM_Count++;	
        Speak_On();			
				if(Speak_PWM_Count <= 1){
					Speak_PWM_On();                   //speak PWM ON
					Speak_PWM_Set(3029,71,1515);		  //330Hz	
				}
				else if(Speak_PWM_Count <= 2){
					Speak_PWM_Off();                 //speak PWM OFF
//					Speak_Off();
				}
				else if(Speak_PWM_Count <= 3){
					Speak_PWM_On();                   //speak PWM ON
					Speak_PWM_Set(3830,71,1916);      //261Hz
				}
				else{
					Speak_PWM_Off();                 //speak PWM OFF
//					Speak_Off();
					if(Speak_PWM_Count >= 100){
						Speak_PWM_Count = 0;
					}	
				}
			}
			else{
				Speak_PWM_Count = 0;
				Speak_Off();
				Speak_PWM_Off(); 
				BUZ_OFF();
			}
		}
		WinCounter_last = WinCounter;
	}
	
	if(uTemp_State == 1){
		if(uTimeT_PWM == 0){
			uTimeT_PWM  		 = SetTimer(0,  20,TimerProcT_PWM);    //刷新鼠标
			AnimationPlayFrame(1,Heat_Sign,ON);
		}
	}
	else{
		if(uTimeT_PWM > 0){
			KillTimer(uTimeT_PWM);
			uTimeT_PWM = 0;
			uTemp_State = 0; 
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);  //关闭加热
			AnimationPlayFrame(1,Heat_Sign,OFF);	
		}
	}

}
