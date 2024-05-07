/************************************��Ȩ����********************************************
**                             ���ݴ�ʹ��Ƽ����޹�˾
**                             http://www.gz-dc.com
**-----------------------------------�ļ���Ϣ--------------------------------------------
** �ļ�����:   hmi_void.c
** �޸�ʱ��:   2018-05-18
** �ļ�˵��:   �û�MCU��������������
** ����֧�֣�  Tel: 020-82186683  Email: hmi@gz-dc.com Web:www.gz-dc.com
--------------------------------------------------------------------------------------*/
#include "hmi_void.h"
#include "Pwm_Control.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "i2c.h"

uint8  Control_Run_Count = 0;                                                       //Bili Fa Count
uint16 LCD_Run_count = 0;                                                           //LCD Count

uint8  cmd_buffer[CMD_MAX_SIZE];                                                     //ָ��� 
uint8  Power_State = 0;                                                              //����״̬
uint8  Gas_Sourse_State = 0;                                                         //��Դ״̬
uint8  Input_YeTi_State = 0;                                                         //��ye״̬
uint8  Heat_State = 0;                                                               //����״̬
uint8  Pre_Limit_State = 0;                                                          //ѹ������״̬
uint8  Pre_Limit_Count = 0;                                                          //ѹ�����Ƽ�ʱ
uint8  Pre_Value = 0;                                                                //ѹ���趨ֵ���ɰ������ڣ���ʼֵΪ5
float  Flo_Value = 0;                                                                //�����趨ֵ���ɰ������ڣ���ʼֵΪ1
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
uint16 current_screen_id = 0;                                                        //��ǰ����ID

//��Դ��ʾ
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

//ѹ��������ʾ
void Pre_Color_Display(void)
{
	switch(Pre_Limit_State)
	{
		case Pre_Normal:
			Pre_Limit_Count = 0;
			SetControlForeColor(1,3,WHITE);                                     //��ʾ��ɫ
			break;
		case Pre_Limit:
			Pre_Limit_Count = 0;
			SetControlForeColor(1,3,RED);                                       //��ʾ��ɫ
			break;
		case Pre_Over_cap:                                                    //��ʾ2S��ɫ to ��ʾ��ɫ
			Pre_Limit_Count++;
		  if(Pre_Limit_Count > 20)
			{
				Pre_Limit_Count = 0;
				Pre_Limit_State = Pre_Normal;
				SetControlForeColor(1,3,WHITE);                                   //��ʾ��ɫ
			}
			else
			{
				SetControlForeColor(1,3,RED);                                     //��ʾ��ɫ
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

//������ֵ��ʾ
void Total_Flo_Display(void)
{
	Total_Flo_Sum = Total_Flo_Sum + Flo_Value_Display; 
	if(Total_Flo_Sum > 599400)
	{
		Total_Flo_Sum = 599400;
	}
	Total_Flo = Total_Flo_Sum/600;
	SetTextInt32(1,11,Total_Flo,0,3);                                     //��ʾ������ֵ
}

//��ʾ���� 
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
*  \brief  ��Ϣ��������
*  \param msg ��������Ϣ
*  \param size ��Ϣ����
*/
void ProcessMessage( PCTRL_MSG msg, uint16 size )
{
    uint8 cmd_type = msg->cmd_type;                                                  //ָ������
    uint8 ctrl_msg = msg->ctrl_msg;                                                  //��Ϣ������
    uint8 control_type = msg->control_type;                                          //�ؼ�����
    uint16 screen_id = PTR2U16(&msg->screen_id);                                     //����ID
    uint16 control_id = PTR2U16(&msg->control_id);                                   //�ؼ�ID
    uint32 value = PTR2U32(msg->param);                                              //��ֵ


    switch(cmd_type)
    {  
    case NOTIFY_TOUCH_PRESS:                                                        //����������
    case NOTIFY_TOUCH_RELEASE:                                                      //�������ɿ�
        NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4)); 
        break;                                                                    
    case NOTIFY_WRITE_FLASH_OK:                                                     //дFLASH�ɹ�
        NotifyWriteFlash(1);                                                      
        break;                                                                    
    case NOTIFY_WRITE_FLASH_FAILD:                                                  //дFLASHʧ��
        NotifyWriteFlash(0);                                                      
        break;                                                                    
    case NOTIFY_READ_FLASH_OK:                                                      //��ȡFLASH�ɹ�
        NotifyReadFlash(1,cmd_buffer+2,size-6);                                     //ȥ��֡ͷ֡β
        break;                                                                    
    case NOTIFY_READ_FLASH_FAILD:                                                   //��ȡFLASHʧ��
        NotifyReadFlash(0,0,0);                                                   
        break;                                                                    
    case NOTIFY_READ_RTC:                                                           //��ȡRTCʱ��
        NotifyReadRTC(cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],cmd_buffer[8]);
        break;
    case NOTIFY_CONTROL:
        {
            if(ctrl_msg==MSG_GET_CURRENT_SCREEN)                                    //����ID�仯֪ͨ
            {
                NotifyScreen(screen_id);                                            //�����л������ĺ���
            }
            else
            {
                switch(control_type)
                {
                case kCtrlButton:                                                   //��ť�ؼ�
                    NotifyButton(screen_id,control_id,msg->param[1]);                  
                    break;                                                             
                case kCtrlText:                                                     //�ı��ؼ�
                    NotifyText(screen_id,control_id,msg->param);                       
                    break;                                                             
                case kCtrlProgress:                                                 //�������ؼ�
                    NotifyProgress(screen_id,control_id,value);                        
                    break;                                                             
                case kCtrlSlider:                                                   //�������ؼ�
                    NotifySlider(screen_id,control_id,value);                          
                    break;                                                             
                case kCtrlMeter:                                                    //�Ǳ�ؼ�
                    NotifyMeter(screen_id,control_id,value);                           
                    break;                                                             
                case kCtrlMenu:                                                     //�˵��ؼ�
                    NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);      
                    break;                                                              
                case kCtrlSelector:                                                 //ѡ��ؼ�
                    NotifySelector(screen_id,control_id,msg->param[0]);                
                    break;                                                              
                case kCtrlRTC:                                                      //����ʱ�ؼ�
                    NotifyTimer(screen_id,control_id);
                    break;
                default:
                    break;
                }
            } 
            break;  
        } 
    case NOTIFY_HandShake:                                                          //����֪ͨ                                                     
        NOTIFYHandShake();
        break;
    default:
        break;
    }
}
/*! 
*  \brief  ����֪ͨ
*/
void NOTIFYHandShake(void)
{
    //TODO: ����û�����
}

/*! 
*  \brief  �����л�֪ͨ
*  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
*  \param screen_id ��ǰ����ID
*/
void NotifyScreen(uint16 screen_id)
{
    current_screen_id = screen_id;                                                   //�ڹ��������п��������л�֪ͨ����¼��ǰ����ID
}

/*! 
*  \brief  ���������¼���Ӧ
*  \param press 1���´�������3�ɿ�������
*  \param x x����
*  \param y y����
*/
void NotifyTouchXY(uint8 press,uint16 x,uint16 y)
{ 
    //TODO: ����û�����
}


/*! 
*  \brief  ��������
*/ 
void UpdateUI(void)
{
	  //����0
		if(current_screen_id == 0)                                               //��ʾ������������  must be here because put befor while(1), data may be lost
		{
			SetTextValue(0,2,(uchar*)"ES031103");                                  //Send ������������ until Screen turn to 0x01            
			SetTextValue(0,3,(uchar*)"YJ01");
			SetTextInt32(1,3,Pre_Value,0,2);                                       //Send ����1������ 
			SetTextInt32(1,4,Flo_Value,1,1);
			AnimationPlayFrame(1,9,High_Display_OFF); 
      AnimationPlayFrame(1,10,Low_Display_OFF);			
			AnimationPlayFrame(1,24,Warning_Display_OFF);	
			AnimationPlayFrame(1,25,Heating_OFF);	
      SetPowerSaving(0x00,0xff,0x32,0x1E);			                            //�ر��Զ����ڱ�������
			GetScreen();                                                          //Because the LCD does not send a switch command, so need to read
		}
	  //����1
    if(current_screen_id == 1)
    {
//			Pre_Display();                                                      //��ʾ���� 
			SetTextInt32(1,1,Pre_Value_Display,0,2);
			SetTextFloat(1,2,Flo_Value_Display,1,1);
			Pre_Color_Display();                                                  //��ʾѹ����ɫ and ѹ����ʾ����
			SetTextInt32(1,3,Pre_Value,0,2);                                      //��ʾԤ��ֵ                                    
			SetTextFloat(1,4,Flo_Value,1,1);
			Total_Flo_Display();
      Gas_Display();                                                        //��ʾ��Դͼ��
			SetTextInt32(1,21,Temp_ADC,0,3);                                      //��ʾ�¶� 
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
*  \brief  ��ť�ؼ�֪ͨ
*  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param state ��ť״̬��0����1����
*/
void NotifyButton(uint16 screen_id, uint16 control_id, uint8  state)
{
    LED_Toggle();	
    if((current_screen_id == 1) && (state == 1))      //��ť����
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
		else if((current_screen_id == 1) && (state == 2))      //��ť����
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
*  \brief  �ı��ؼ�֪ͨ
*  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
*  \details  �ı��ؼ����������ַ�����ʽ�·���MCU������ı��ؼ������Ǹ���ֵ��
*  \details  ����Ҫ�ڴ˺����н��·��ַ�������ת�ظ���ֵ��
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param str �ı��ؼ�����
*/
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str)
{
    //TODO: ����û����� 
}                                                                                

/*!                                                                              
*  \brief  �������ؼ�֪ͨ                                                       
*  \details  ����GetControlValueʱ��ִ�д˺���                                  
*  \param screen_id ����ID                                                      
*  \param control_id �ؼ�ID                                                     
*  \param value ֵ                                                              
*/                                                                              
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value)           
{  
    //TODO: ����û�����    
}                                                                                

/*!                                                                              
*  \brief  �������ؼ�֪ͨ                                                       
*  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���                  
*  \param screen_id ����ID                                                      
*  \param control_id �ؼ�ID                                                     
*  \param value ֵ                                                              
*/                                                                              
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value)             
{                                                             
    //TODO: ����û����� 
}

/*! 
*  \brief  �Ǳ�ؼ�֪ͨ
*  \details  ����GetControlValueʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param value ֵ
*/
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value)
{
    //TODO: ����û�����
}

/*! 
*  \brief  �˵��ؼ�֪ͨ
*  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param item �˵�������
*  \param state ��ť״̬��0�ɿ���1����
*/
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8 item, uint8 state)
{
    //TODO: ����û�����
}

/*! 
*  \brief  ѡ��ؼ�֪ͨ
*  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param item ��ǰѡ��
*/
void NotifySelector(uint16 screen_id, uint16 control_id, uint8  item)
{
    //TODO: ����û�����
}

/*! 
*  \brief  ��ʱ����ʱ֪ͨ����
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*/
void NotifyTimer(uint16 screen_id, uint16 control_id)
{
    //TODO: ����û����� 
}

/*! 
*  \brief  ��ȡ�û�FLASH״̬����
*  \param status 0ʧ�ܣ�1�ɹ�
*  \param _data ��������
*  \param length ���ݳ���
*/
void NotifyReadFlash(uint8 status,uint8 *_data,uint16 length)
{
    //TODO: ����û�����
}

/*! 
*  \brief  д�û�FLASH״̬����
*  \param status 0ʧ�ܣ�1�ɹ�
*/
void NotifyWriteFlash(uint8 status)
{
    //TODO: ����û�����
}

/*! 
*  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
*  \param year �꣨BCD��
*  \param month �£�BCD��
*  \param week ���ڣ�BCD��
*  \param day �գ�BCD��
*  \param hour ʱ��BCD��
*  \param minute �֣�BCD��
*  \param second �루BCD��
*/
void NotifyReadRTC(uint8 year,uint8 month,uint8 week,uint8 day,uint8 hour,uint8 minute,uint8 second)
{
    //TODO: ����û����� 
}
