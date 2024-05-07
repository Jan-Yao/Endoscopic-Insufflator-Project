/************************************版权申明********************************************
**                             广州大彩光电科技有限公司
**                             http://www.gz-dc.com
**-----------------------------------文件信息--------------------------------------------
** 文件名称:   hmi_void.c
** 修改时间:   2018-05-18
** 文件说明:   用户MCU串口驱动函数库
** 技术支持：  Tel: 020-82186683  Email: hmi@gz-dc.com Web:www.gz-dc.com
--------------------------------------------------------------------------------------*/
#include "hmi_void.h"
#include "Pwm_Control.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "i2c.h"

uint8  Control_Run_Count = 0;                                                       //Bili Fa Count
uint16 LCD_Run_count = 0;                                                           //LCD Count

uint8  cmd_buffer[CMD_MAX_SIZE];                                                     //指令缓存 
uint8  Power_State = 0;                                                              //开机状态
uint8  Gas_Sourse_State = 0;                                                         //气源状态
uint8  Input_YeTi_State = 0;                                                         //进ye状态
uint8  Heat_State = 0;                                                               //加热状态
uint8  Pre_Limit_State = 0;                                                          //压力限制状态
uint8  Pre_Limit_Count = 0;                                                          //压力限制计时
uint8  Pre_Value = 0;                                                                //压力设定值，由按键调节，初始值为5
float  Flo_Value = 0;                                                                //流量设定值，由按键调节，初始值为1
uint8  Buz_Num = 0;
uint8  Pre_Value_ADC = 0;
uint8  Pre_Value_Display = 0;
uint8  Pre_Value_Now = 0; 
uint8  Pre_Keep_Count = 0;
float  Flo_Value_ADC = 0;
float  Flo_Value_Display = 0;
uint16 Gas_Value_ADC = 0;
uint16 Total_Flo = 0;
uint32 Total_Flo_Sum = 0;
uint16 current_screen_id = 0;                                                        //当前画面ID

//气源显示
void Gas_Display(void)
{
	switch(Gas_Sourse_State)
	{
		case Gas_None:
			AnimationPlayFrame(1,16,0);
		  break;
		case Gas_Level_1:
			AnimationPlayFrame(1,16,1);
			break;
		case Gas_Level_2:
			AnimationPlayFrame(1,16,2);
			break;
		case Gas_Level_3:
			AnimationPlayFrame(1,16,3);
			break;
		case Gas_Level_4:
			AnimationPlayFrame(1,16,4);
			break;
		case Gas_Level_5:
			AnimationPlayFrame(1,16,5);
			break;
		case Gas_Level_6:
			AnimationPlayFrame(1,16,6);
			break;
		case Gas_Level_7:
			AnimationPlayFrame(1,16,7);
			break;
		default:
			break;
	}
}

//压力限制显示
void Pre_Color_Display(void)
{
	switch(Pre_Limit_State)
	{
		case Pre_Normal:
			Pre_Limit_Count = 0;
			SetControlForeColor(1,3,WHITE);                                     //显示白色
			break;
		case Pre_Limit:
			Pre_Limit_Count = 0;
			SetControlForeColor(1,3,RED);                                       //显示红色
			break;
		case Pre_Over_cap:                                                    //显示2S红色 to 显示白色
			Pre_Limit_Count++;
		  if(Pre_Limit_Count > 20)
			{
				Pre_Limit_Count = 0;
				Pre_Limit_State = Pre_Normal;
				SetControlForeColor(1,3,WHITE);                                   //显示白色
			}
			else
			{
				SetControlForeColor(1,3,RED);                                     //显示红色
			}
			break;
		default:
			break;
	}
	switch(Pre_State)
	{
		case Pre_Keep:
			Buz_Num = 0;
//		  Buz_State = Buz_OFF;
			AnimationPlayFrame(1,9,High_Display_OFF); 
      AnimationPlayFrame(1,10,Low_Display_OFF);	
			break;
		case Pre_Up:
			Buz_Num = 0;
//		  Buz_State = Buz_OFF;
			if((Pre_Value_Display < (Pre_Value-2)) && (Pre_Value > 2))
			{
				AnimationPlayFrame(1,9,High_Display_OFF); 
        AnimationPlayFrame(1,10,Low_Display_ON);
			}
			else
			{
				AnimationPlayFrame(1,9,High_Display_OFF); 
        AnimationPlayFrame(1,10,Low_Display_OFF);
			}
			break;
		case Pre_Down:
		  Buz_Num++;
		  if(Buz_Num > 5)
			{
				AnimationPlayFrame(1,9,High_Display_ON); 
        AnimationPlayFrame(1,10,Low_Display_OFF);	
			}
			else
			{
//				AnimationPlayFrame(1,9,High_Display_OFF); 
        AnimationPlayFrame(1,10,Low_Display_OFF);
			}
			if(Buz_Num > 50)
			{
				Buz_Num = 0;
				if(Buz_State == Buz_OFF)
				{
					Buz_State = Buz_Over_Pre;
				}
			}
			break;
		default:
			break;
	}
}

//总流量值显示
void Total_Flo_Display(void)
{
	Total_Flo_Sum = Total_Flo_Sum + Flo_Value_Display; 
	if(Total_Flo_Sum > 599400)
	{
		Total_Flo_Sum = 599400;
	}
	Total_Flo = Total_Flo_Sum/600;
	SetTextInt32(1,11,Total_Flo,0,3);                                     //显示总流量值
}

//显示数据 
void Pre_Display(void)
{
//	uint8_t Display_Error = 0;
//	
//	if(Pre_Error != 30)
//	{
//		Pre_Value_Now = Pre_Value-Pre_Error;
//	}
//	else
//	{
//		Pre_Value_Now = 0;
//	}
//	
//	if(Power_State == ON)
//	{
//		if(Pre_State != Pre_Up)
//		{
//			if((Pre_Error_AVG > 2) && (Pre_Error_AVG <= 2+(Flo_Value/10)) && (Pre_State == Pre_Keep))
//			{
//				Pre_Keep_Count = 0;
//				SetTextInt32(1,1,Pre_Value+2,0,2);
//			}
//			else
//			{
//				if((Pre_Error_AVG <= 2+(Flo_Value/10)) && (Pre_State == Pre_Keep))
//				{
//					Pre_Keep_Count++;
//					if(Pre_Keep_Count > 3)
//					{
//						Pre_Keep_Count = 3;
//						if((Pre_Value_ADC+2 == Pre_Value) || (Pre_Value_ADC+2 == Pre_Value-1))
//						{
//							SetTextInt32(1,1,Pre_Value_ADC+2,0,2);
//						}
//						else
//						{
//							if((Pre_Value_Now+2 == Pre_Value) || (Pre_Value_Now+2 == Pre_Value-1))
//							{
//								SetTextInt32(1,1,Pre_Value_Now+2,0,2);
//							}
//							else
//							{
//								SetTextInt32(1,1,Pre_Value_Now,0,2);
//							}
//						}
//					}
//				}
//				else
//				{
//					Pre_Keep_Count = 0;
//					if(Buz_Num > 5)
//					{
//						SetTextInt32(1,1,Pre_Value_ADC,0,2);
//					}
//				}
//			}
//		}
//		else
//		{
//			if(Pre_Error_AVG > 2+(Flo_Value/10))
//			{
//				if(Pre_Keep_Count == 0)
//				{
//					SetTextInt32(1,1,Pre_Value_ADC,0,2);
//				}
//			}
//			Pre_Keep_Count = 0;
//		}
//	}
//  else
//	{
//		Pre_Keep_Count = 0;
//		SetTextInt32(1,1,Pre_Value_ADC,0,2);
//	}
}

/*! 
*  \brief  消息处理流程
*  \param msg 待处理消息
*  \param size 消息长度
*/
void ProcessMessage( PCTRL_MSG msg, uint16 size )
{
    uint8 cmd_type = msg->cmd_type;                                                  //指令类型
    uint8 ctrl_msg = msg->ctrl_msg;                                                  //消息的类型
    uint8 control_type = msg->control_type;                                          //控件类型
    uint16 screen_id = PTR2U16(&msg->screen_id);                                     //画面ID
    uint16 control_id = PTR2U16(&msg->control_id);                                   //控件ID
    uint32 value = PTR2U32(msg->param);                                              //数值


    switch(cmd_type)
    {  
    case NOTIFY_TOUCH_PRESS:                                                        //触摸屏按下
    case NOTIFY_TOUCH_RELEASE:                                                      //触摸屏松开
        NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4)); 
        break;                                                                    
    case NOTIFY_WRITE_FLASH_OK:                                                     //写FLASH成功
        NotifyWriteFlash(1);                                                      
        break;                                                                    
    case NOTIFY_WRITE_FLASH_FAILD:                                                  //写FLASH失败
        NotifyWriteFlash(0);                                                      
        break;                                                                    
    case NOTIFY_READ_FLASH_OK:                                                      //读取FLASH成功
        NotifyReadFlash(1,cmd_buffer+2,size-6);                                     //去除帧头帧尾
        break;                                                                    
    case NOTIFY_READ_FLASH_FAILD:                                                   //读取FLASH失败
        NotifyReadFlash(0,0,0);                                                   
        break;                                                                    
    case NOTIFY_READ_RTC:                                                           //读取RTC时间
        NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
        break;
    case NOTIFY_CONTROL:
        {
            if(ctrl_msg==MSG_GET_CURRENT_SCREEN)                                    //画面ID变化通知
            {
                NotifyScreen(screen_id);                                            //画面切换调动的函数
            }
            else
            {
                switch(control_type)
                {
                case kCtrlButton:                                                   //按钮控件
                    NotifyButton(screen_id,control_id,msg->param[1]);                  
                    break;                                                             
                case kCtrlText:                                                     //文本控件
                    NotifyText(screen_id,control_id,msg->param);                       
                    break;                                                             
                case kCtrlProgress:                                                 //进度条控件
                    NotifyProgress(screen_id,control_id,value);                        
                    break;                                                             
                case kCtrlSlider:                                                   //滑动条控件
                    NotifySlider(screen_id,control_id,value);                          
                    break;                                                             
                case kCtrlMeter:                                                    //仪表控件
                    NotifyMeter(screen_id,control_id,value);                           
                    break;                                                             
                case kCtrlMenu:                                                     //菜单控件
                    NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);      
                    break;                                                              
                case kCtrlSelector:                                                 //选择控件
                    NotifySelector(screen_id,control_id,msg->param[0]);                
                    break;                                                              
                case kCtrlRTC:                                                      //倒计时控件
                    NotifyTimer(screen_id,control_id);
                    break;
                default:
                    break;
                }
            } 
            break;  
        } 
    case NOTIFY_HandShake:                                                          //握手通知                                                     
        NOTIFYHandShake();
        break;
    default:
        break;
    }
}
/*! 
*  \brief  握手通知
*/
void NOTIFYHandShake(void)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  画面切换通知
*  \details  当前画面改变时(或调用GetScreen)，执行此函数
*  \param screen_id 当前画面ID
*/
void NotifyScreen(uint16 screen_id)
{
    current_screen_id = screen_id;                                                   //在工程配置中开启画面切换通知，记录当前画面ID
}

/*! 
*  \brief  触摸坐标事件响应
*  \param press 1按下触摸屏，3松开触摸屏
*  \param x x坐标
*  \param y y坐标
*/
void NotifyTouchXY(uint8 press,uint16 x,uint16 y)
{ 
    //TODO: 添加用户代码
}


/*! 
*  \brief  更新数据
*/ 
void UpdateUI(void)
{
	  //画面0
		if(current_screen_id == 0)                                               //显示开机画面数据  must be here because put befor while(1), data may be lost
		{
			SetTextValue(0,2,(uchar*)"ES031103");                                  //Send 开机画面数据 until Screen turn to 0x01            
			SetTextValue(0,3,(uchar*)"YJ01");
			SetTextInt32(1,3,Pre_Value,0,2);                                       //Send 画面1的数据 
			SetTextInt32(1,4,Flo_Value,1,1);
			AnimationPlayFrame(1,9,High_Display_OFF); 
      AnimationPlayFrame(1,10,Low_Display_OFF);			
			AnimationPlayFrame(1,24,Warning_Display_OFF);	
			AnimationPlayFrame(1,25,Heating_OFF);	
      SetPowerSaving(0x00,0xff,0x32,0x1E);			                            //关闭自动调节背光亮度
			GetScreen();                                                          //Because the LCD does not send a switch command, so need to read
		}
	  //画面1
    if(current_screen_id == 1)
    {
//			Pre_Display();                                                      //显示数据 
			SetTextInt32(1,1,Pre_Value_Display,0,2);
			SetTextFloat(1,2,Flo_Value_Display,1,1);
			Pre_Color_Display();                                                  //显示压力颜色 and 压力显示报警
			SetTextInt32(1,3,Pre_Value,0,2);                                      //显示预设值                                    
			SetTextFloat(1,4,Flo_Value,1,1);
			Total_Flo_Display();
      Gas_Display();                                                        //显示气源图标
			SetTextInt32(1,21,Temp_ADC,0,3);                                      //显示温度 
			if(Input_YeTi_State == Input_YeTi_Disable)
			{
				AnimationPlayFrame(1,24,Warning_Display_OFF);	
			}
			else if(Input_YeTi_State == Input_YeTi_Enable)
			{
				AnimationPlayFrame(1,24,Warning_Display_ON);
//        SetBuzzer(10);				
			}
			
			if(Heat_State == Heat_Disable)
			{
				AnimationPlayFrame(1,25,Heating_OFF);	
			}
			else if(Heat_State == Heat_Enable)
			{
				AnimationPlayFrame(1,25,Heating_ON);	
			}
    }
}

/*! 
*  \brief  按钮控件通知
*  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param state 按钮状态：0弹起，1按下
*/
void NotifyButton(uint16 screen_id, uint16 control_id, uint8  state)
{
    LED_Toggle();	
    if((current_screen_id == 1) && (state == 1))      //按钮按下
		{
			switch(control_id)
			{
				case Button_Pre_Plus:
					if(Pre_Limit_State == Pre_Normal)
					{
						if(Pre_Value < 30)
						{
							Pre_Value++;
							if(Pre_Value == 15)
							{
								Pre_Limit_State = Pre_Limit;
							}						
						}
						else
						{
							Pre_Value = 30;
						}
					}
					else 
					{
//						Pre_Value = 15;
						Pre_Limit_State = Pre_Over_cap;
					}
					break;
				case Button_Pre_Decr:
					Pre_Limit_State = Pre_Normal;
					if(Pre_Value > 1)
					{
						Pre_Value--;
						if(Pre_Value == 15)
						{
							Pre_Limit_State = Pre_Limit;
						}
					}
					else
					{
						Pre_Value = 1;
					}
					break;
				case Button_Flo_Plus:
				  if(Flo_Value < 60)
					{
						Flo_Value += 0.1;
					}
					if(Flo_Value >= 60)
					{
						Flo_Value = 60;
					}
				  break;
				case Button_Flo_Decr:
				  if(Flo_Value > 1)
					{
						Flo_Value -= 0.1;
					}
					if(Flo_Value <= 1)
					{
						Flo_Value = 1;
					}
				  break;
				case Button_Clear_Total:
          Total_Flo = 0;
				  Total_Flo_Sum = 0;
				  break;
				case Button_ON_OFF:
					if(Power_State == OFF)
					{
						if(Gas_Sourse_State > Gas_None)
						{
							Power_State = ON;
						}
						else
						{
							Buz_State = Buz_ON_Warn;
						}
					}
					else
					{
						Power_State = OFF;
						__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
            __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);						
						Control_OFF();
						EEPROM_WriteOneByte(0x10,Pre_Value);                                          //Pre Default value = 5
						DelayMS(100);                                                                 //wait 5ms to write
						EEPROM_WriteFloat(0x20,Flo_Value);
					}
				  break;
				case Button_Smoke:
					Control_Paiyan_Toggle();						
				  break;
				case Button_Low_Pre:
					Pre_Limit_State = Pre_Normal;
					Pre_Value = 7;
			    Flo_Value = 3;
				  break;
				case Button_High_Pre:
					Pre_Limit_State = Pre_Normal;
					Pre_Value = 12;
			    Flo_Value = 20;
				  break;
				default:
					break;
			}
		}
		else if((current_screen_id == 1) && (state == 2))      //按钮按下
		{
			switch(control_id)
			{
				case Button_Flo_Plus:
				  if(Flo_Value < 60)
					{
						Flo_Value++;
					}
					if(Flo_Value >= 60)
					{
						Flo_Value = 60;
					}
				  break;
				case Button_Flo_Decr:
				  if(Flo_Value > 1)
					{
						Flo_Value--;
					}
					if(Flo_Value <= 1)
					{
						Flo_Value = 1;
					}
				  break;
				default:
					break;
			}
		}	
}

/*! 
*  \brief  文本控件通知
*  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
*  \details  文本控件的内容以字符串形式下发到MCU，如果文本控件内容是浮点值，
*  \details  则需要在此函数中将下发字符串重新转回浮点值。
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param str 文本控件内容
*/
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str)
{
    //TODO: 添加用户代码 
}                                                                                

/*!                                                                              
*  \brief  进度条控件通知                                                       
*  \details  调用GetControlValue时，执行此函数                                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/                                                                              
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value)           
{  
    //TODO: 添加用户代码    
}                                                                                

/*!                                                                              
*  \brief  滑动条控件通知                                                       
*  \details  当滑动条改变(或调用GetControlValue)时，执行此函数                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/                                                                              
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value)             
{                                                             
    //TODO: 添加用户代码 
}

/*! 
*  \brief  仪表控件通知
*  \details  调用GetControlValue时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param value 值
*/
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  菜单控件通知
*  \details  当菜单项按下或松开时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 菜单项索引
*  \param state 按钮状态：0松开，1按下
*/
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8 item, uint8 state)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  选择控件通知
*  \details  当选择控件变化时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 当前选项
*/
void NotifySelector(uint16 screen_id, uint16 control_id, uint8  item)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  定时器超时通知处理
*  \param screen_id 画面ID
*  \param control_id 控件ID
*/
void NotifyTimer(uint16 screen_id, uint16 control_id)
{
    //TODO: 添加用户代码 
}

/*! 
*  \brief  读取用户FLASH状态返回
*  \param status 0失败，1成功
*  \param _data 返回数据
*  \param length 数据长度
*/
void NotifyReadFlash(uint8 status,uint8 *_data,uint16 length)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  写用户FLASH状态返回
*  \param status 0失败，1成功
*/
void NotifyWriteFlash(uint8 status)
{
    //TODO: 添加用户代码
}

/*! 
*  \brief  读取RTC时间，注意返回的是BCD码
*  \param year 年（BCD）
*  \param month 月（BCD）
*  \param week 星期（BCD）
*  \param day 日（BCD）
*  \param hour 时（BCD）
*  \param minute 分（BCD）
*  \param second 秒（BCD）
*/
void NotifyReadRTC(uint8 year,uint8 month,uint8 week,uint8 day,uint8 hour,uint8 minute,uint8 second)
{
    //TODO: 添加用户代码 
}
