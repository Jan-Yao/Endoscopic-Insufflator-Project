#include "PWM_Control.h"
#include "hmi_void.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "i2c.h"
#include "Temp.h"
#include "usart.h"
#include "iwdg.h"

uint8_t   Flo_count = 0;
uint8_t   Gas_count = 0;
//uint8_t   Pre_count = 0;
uint8_t   Warning_enabled_count = 0;
uint8_t   Warning_disable_count = 0;
uint8_t   Temp_count = 0;
//uint8_t   Pre_ADC_count = 0;
//uint8_t   Pre_Down_count = 0;
uint8_t   Buz_State = 0;
uint8_t   Pre_State = 0;
uint8_t   Display_Keep_Count = 0;
uint8_t   Display_Down_Count = 0;
//uint8_t   Output_Value = 0;
uint8_t   PWM_Calibration = 0;
uint8_t   Overshoot_Count = 0;
float     Flo_Error = 0;
int16_t   Pre_Error = 0;
int16_t   Pre_Error_Last = 0;
int16_t   Pre_Error_Now = 0;
float     Temp_ADC = 0;
float     Flo_Sum   = 0;
uint32_t  Gas_Sum   = 0;
uint16_t  Gas_ADC = 0;
//uint16_t  Pre_Sum   = 0;
//uint16_t  Pre_AVG   = 0;
//uint16_t  Pre_ADC_Sum   = 0;
uint16_t  Pre_Overshoot = 0;
uint16_t  Buz_count = 0;
uint16_t  Run_count = 0;
//uint16_t  Calibration_count = 0;
uint16_t  Beginning_count = 0;
int16_t   Pre_Error_AVG  = 0;
int16_t   Duty = 0;
int16_t   Duty_Beginning = 0;
uint8_t   Pre_buffer[60] = {0};
uint16_t  PWM_buffer[61] = {0};

void PWM_buffer_Init(void)
{
	PWM_buffer[0] = 0;
	for(int i = 1;i<61;i++)
	{
		PWM_buffer[i] = 7*i + 460;
	}
}

void Get_Temp_Value(void)
{
	float Temp_Error = 0;
	float Temp_Value_1 = 0;
	float Temp_Value_2 = 0;
	
	Temp_Value_1 = ADC_Buffer[3];
	Temp_Value_2 = ADC_Buffer[4];

	Temp_Error = Temp_Value_1 - Temp_Value_2;
	
	if(Temp_Error < 200 && Temp_Error > -200)
	{
		Temp_ADC = (Temp_Value_1+Temp_Value_2)/2;
	}
	else
	{
		Temp_ADC = 3300;
	}
}

uint16_t Get_Gas_Value(void)                                     
{
	Gas_ADC = ADC_Buffer[0];                                       //Gas ADC IN4
	
	Gas_Sum += Gas_ADC;                                       
	Gas_count++;
	if(Gas_count >= 100)                                            
	{
		Gas_count = 0;
		Gas_Value_ADC = Gas_Sum/100;                                 //1000ms updata to LCD Gas Value
		Gas_Sum = 0;
	}
	return Gas_ADC;
}

void Gas_Input_Detect(void)
{
	if(Gas_Value_ADC <= 600)                                       //气源图标显示0格
	{ 
		Gas_Sourse_State = Gas_None;
		if(Power_State == ON)
		{
			Power_State = OFF;
			Buz_State = Buz_ON_Long;
		}
	}
	else if(Gas_Value_ADC <= 800)                                  //气源图标显示1格
	{
		Gas_Sourse_State = Gas_Level_1;
		if(Power_State == ON)
		{
			Buz_State = Buz_ON_Short;
		}
		else
		{
			Buz_State = Buz_OFF;
		}
	}
	else if(Gas_Value_ADC <= 1000)                                 //气源图标显示2格
	{
		Gas_Sourse_State = Gas_Level_2;
		if(Buz_State != Buz_Over_Pre)
		{
			Buz_State = Buz_OFF;
		}
	}
	else if(Gas_Value_ADC <= 1200)                                 //气源图标显示3格
	{                                  
		Gas_Sourse_State = Gas_Level_3;
		if(Buz_State != Buz_Over_Pre)
		{
			Buz_State = Buz_OFF;
		}
	}
	else if(Gas_Value_ADC <= 1400)                                 //气源图标显示4格
	{
		Gas_Sourse_State = Gas_Level_4;
		if(Buz_State != Buz_Over_Pre)
		{
			Buz_State = Buz_OFF;
		}
	}
	else if(Gas_Value_ADC <= 1600)                                 //气源图标显示5格
	{ 
		Gas_Sourse_State = Gas_Level_5;
		if(Buz_State != Buz_Over_Pre)
		{
			Buz_State = Buz_OFF;
		}
	}
	else if(Gas_Value_ADC <= 1800)                                 //气源图标显示6格
	{                                
		Gas_Sourse_State = Gas_Level_6;
		if(Buz_State != Buz_Over_Pre)
		{
			Buz_State = Buz_OFF;
		}
	}
	else                                                           //气源图标显示7格
	{                               
		Gas_Sourse_State = Gas_Level_7;
		if(Buz_State != Buz_Over_Pre)
		{
			Buz_State = Buz_OFF;
		}
	}
	
	switch(Buz_State)
	{
		case Buz_OFF:
			BUZ_OFF();
		  Buz_count = 0;
			break;
		case Buz_ON_Short:                                           //50ms ON 150ms OFF for cycle
			Buz_count++;                                               //2ms OFF 700ms
		  if(Buz_count > 20)
			{
				Buz_count = 0;
			}
			else if(Buz_count > 5)
			{
				BUZ_OFF();
			}
			else
			{
				if(__HAL_TIM_GetCompare(&htim3,TIM_CHANNEL_1) != 0)
				{
					BUZ_ON();
				}
			}
			break;
		case Buz_ON_Long:                                            //1000ms ON for once
			Buz_count++;
		  if(Buz_count >= 100)
		  {
				Buz_count = 0;
				BUZ_OFF();
				Buz_State = Buz_OFF;
				AnimationPlayFrame(1,18,0);                              //显示OFF
				AnimationPlayPrev(1,14);
			}
			else
			{
				BUZ_ON();
			}
		  break;
		case Buz_ON_Warn:                                            //200ms ON 100ms OFF 100ms ON for once                       
			Buz_count++;
		  if(Buz_count > 40)
			{
				Buz_count = 0;
				BUZ_OFF();
				Buz_State = Buz_OFF;
				AnimationPlayFrame(1,18,0);                              //显示OFF
				AnimationPlayPrev(1,14);
			}
			else if(Buz_count > 30)
			{
				BUZ_ON();
			}
		  else if(Buz_count > 20)
		  {
				BUZ_OFF();
			}
			else
			{
				BUZ_ON();
			}
			break;
		case Buz_Over_Pre:
			Buz_count++;
		  if(Buz_count >= 50)
		  {
				Buz_count = 0;
				BUZ_OFF();
				Buz_State = Buz_OFF;
			}
			else
			{
				BUZ_ON();
			}
			break;
		default:
			break;
	}
	Get_Gas_Value();                                 
}

int16_t Get_Pre_Value(void)
{
	int16_t Error = 0;
	if(ADC_Buffer[1] < 20)
	{
		ADC_Buffer[1] = 20;
	}
	Pre_Value_ADC = (uint8_t)((ADC_Buffer[1] - 20) * 4137 / 100 / 2800);
	if(Pre_Value > 1)
	{
		Error = Pre_Value - Pre_Value_ADC;
	}
	else
	{
		if(Pre_Value_ADC == 0)
		{
			Error = 2;
		}
		else
		{
			Error = Pre_Value - Pre_Value_ADC;
		}
	}
	return Error;
}

float Get_Flo_Value(void)
{
	float Error = 0;
	if(ADC_Buffer[2] < 600)                                        //初始值在0V-0.6V之间,Give 600;Flo = 0
	{
		ADC_Buffer[2] = 600;
	}
	Flo_Value_ADC = ((float)ADC_Buffer[2] - 600)/42;               //Flo ADC IN14
	
	Flo_Sum += Flo_Value_ADC;
	Flo_count++;
	if(Flo_count >= 20)
	{
		Flo_count = 0;
		Flo_Value_Display = Flo_Sum/20;                              //200ms updata to LCD Flo Value
		Flo_Sum = 0;
	}
	Error = Flo_Value - Flo_Value_ADC;
	return Error;
}

void Input_PWM_Value_Reassign(void)
{
	if(Flo_Value_ADC > 3.2 && Flo_Value_ADC < 5.2)
	{
		Beginning_count++;
		if(Beginning_count > 8)
		{
			Beginning_count = 3;
			Duty_Beginning = Duty*8/10 + Duty_Beginning*2/10;
		}
	}
	else
	{
		Beginning_count = 0;
	}
	
  if(Flo_Error > 1)
	{
		if(Pre_Value_ADC > 45)
		{
			Duty = Duty-9;
		}
		else if(Pre_Value_ADC > (Pre_Overshoot+2))
		{
			Duty = Duty-6;
		}
		else if(Pre_Value_ADC > Pre_Overshoot)
		{
			Duty = Duty-3;
		}
		else if(Pre_Value_ADC < (Pre_Overshoot-2))
		{
			if(Duty < PWM_buffer[6])
			{
				if(Run_count%4 == 0)
				{
					Duty = Duty+1;
				}
			}
			else if(Duty < PWM_buffer[10])
			{
				if(Run_count%2 == 0)
				{
					Duty = Duty+1;
				}
			}
			else if(Duty < PWM_buffer[25])
			{
				Duty = Duty+1;
			}
			else
			{
				Duty = Duty+5;
			}
		}
	}	

	if(Duty > 1000)
	{
		Duty = 1000;
	}
	else if(Duty < Duty_Beginning)
	{
		Duty = Duty_Beginning;
	}
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Duty);
}

void Input_PWM_Control(void)
{
	if(Run_count > 350)
	{
		Run_count = 0;
	}
	Run_count++;
	
	if(Run_count > 300)                                                 
	{
		Beginning_count = 0;
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);                     //After continuous work for 3s, to stop 700ms
	}
	else if(Run_count > 1)                                              //500ms for turn on BiLiFa and Flo_Value_ADC
	{
		Input_PWM_Value_Reassign();
	}
	else if(Run_count > 0)
	{
		Beginning_count = 0;
		if(Duty_Beginning < PWM_buffer[1])
		{
			Duty_Beginning = PWM_buffer[1];
		}
		Duty = Duty_Beginning;
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,Duty);
	}
}

void Input_PWM_Calibration(void)
{
	if(PWM_Calibration == Finsh)
	{
		if(__HAL_TIM_GetCompare(&htim3,TIM_CHANNEL_1) != 0)
		{
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);                   //IN_OFF
		  Control_OFF();
			HAL_IWDG_Refresh(&hiwdg);
			EEPROM_Write_PWM(0x30);	
			EEPROM_Read_PWM_Debug(Send_Write);                              //Send
		}
	}
	else
	{
		if(PWM_Calibration == Standby)
		{
			Flo_Value = 1;
			Control_ON(); 
		}
		Flo_Error = Get_Flo_Value();
//		Input_PWM_Value_Calibration();
	}
}

void Heat_Countrol(void)
{
	Get_Temp_Value();
	
	if(Temp_ADC > 3200)                                              //No heat
	{
		Heat_State = Heat_Disable;
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);                  //Heat_OFF
	}
	else if(Temp_ADC > 2500)
	{
		Heat_State = Heat_Enable;
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,1000);               //Heat_ON
	}
	else if(Temp_ADC < 1900)
	{
		Heat_State = Heat_Enable;
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);                  //Heat_OFF
	}
	else if(Temp_ADC < 1000)
	{
		Heat_State = Heat_Disable;
		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);                  //Heat_OFF
	}
}

void PWM_Control(void)
{
	Heat_Countrol();
	
	if(Input_YeTi_Read() == 1)
	{
		Warning_enabled_count = 0;
		Warning_disable_count++;
		if(Warning_disable_count > 50)
		{
			Warning_disable_count = 50;
			Input_YeTi_State = Input_YeTi_Enable;
//			if(Buz_State == Buz_OFF)
//			{
//				BUZ_Toggle();
//			}
		}
	}
	else
	{
		Warning_disable_count = 0;
		Warning_enabled_count++;
		if(Warning_enabled_count > 50)
		{
			Warning_enabled_count = 50;
			Input_YeTi_State = Input_YeTi_Disable;
//			if(Buz_State == Buz_OFF)
//			{
//				BUZ_OFF();
//			}
		}
	}
	
	
	if(Run_count > 300 || Run_count == 0)
	{
		Pre_Error = Get_Pre_Value();
	}
	else
	{
		Get_Pre_Value();
	}
	Flo_Error = Get_Flo_Value();
	
	if(Power_State == ON)
	{
		Control_ON();                                                  //kai qi
		
		if(Run_count > 350 || Run_count == 0)                          //Pre chuandi 
		{
			Pre_Error_Last = Pre_Error_Now;
		  Pre_Error_Now = Pre_Error;
		}
		
		if(Pre_Error > 1)
		{
			Pre_State = Pre_Up;
			if(Pre_Error_Last <= 0)
			{
				Pre_Overshoot = Pre_Value + (5*Pre_Error_Now+5)/10;        //over  Pre
			}
			else
			{
				if((Pre_Error_Now-Pre_Error_Last)*(Pre_Error_Now-Pre_Error_Last) > ((Pre_Error_Last/2)*(Pre_Error_Last/2)))   //Error2 > (last/2)2
				{
					Pre_Overshoot = Pre_Value + (5*Pre_Error_Now*Pre_Error_Now/(Pre_Error_Last/2)+5)/10;                        //Pre_Overshoot = Pre_Value+0.5*NOW*NOW/(LAST/2)
				}
				else
				{
					if(Pre_Error_Last > 2)
					{
						Pre_Overshoot = Pre_Overshoot + ((5*Pre_Error_Now*(Pre_Error_Now-(Pre_Error_Last/2))/(Pre_Error_Last/2))+5)/10; //Pre_Overshoot = Pre_Overshoot+0.5*NOW*(NOW-LAST/2)/(LAST/2)
					}
				}
			}
			if(Pre_Overshoot > 38)
			{
				Pre_Overshoot = 38;
			}
			else if(Pre_Overshoot < 3)
			{
				Pre_Overshoot = 3;
			}
		}
		else if(Pre_Error < -2)
		{
			Pre_State = Pre_Down;
		}
		else
		{
			Pre_State = Pre_Keep;
		}
			
		switch(Pre_State)
		{
			case Pre_Keep:
				Run_count = 310;
			  Overshoot_Count = 0;
			
			  Display_Down_Count = 0;
			  Display_Keep_Count++;
				if(Display_Keep_Count > 20)
				{
					Display_Keep_Count = 20;
					Pre_Value_Display = Pre_Value_ADC;
				}
				
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);              //Out_OFF
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);              //IN_OFF
				break;
			case Pre_Up:
				Display_Keep_Count = 0;
			  Display_Down_Count = 0;
        __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);              //Out_OFF   
        if(Pre_Value_ADC > 45)
				{
					Overshoot_Count++;
				}
				else
				{
					Overshoot_Count = 0;
				}	
				
				if(Overshoot_Count > 3)
				{
					Run_count = 310;
					Overshoot_Count = 3;
				}

				if(Run_count > 330 || Run_count == 0) 
				{
					Pre_Value_Display = Pre_Value_ADC;
				}
			  Input_PWM_Control();                                      //Constant flow intake
				break;
			case Pre_Down:
				Run_count = 310;
			  Overshoot_Count = 0;
			
			  Display_Keep_Count = 0;
			  Display_Down_Count++;
				if(Display_Down_Count > 45)
				{
					Display_Down_Count = 45;
					Pre_Value_Display = Pre_Value_ADC;
					if(Pre_Value_ADC > 45 || ((Pre_Value_ADC > 2*Pre_Value) && (Pre_Value > 10)))
					{
						__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,600);        //Out_ON
					}
				}
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);              //IN_OFF
				break;
			default:
			  break;
		}
		SetTextInt32(1,22,Duty_Beginning,0,2); 
		SetTextInt32(1,23,Duty,0,3);
	}
	else
	{
//		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);                 //Heat_OFF
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);                   //IN_OFF
		__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);                   //Out_OFF
		Control_OFF();
		Pre_State = Pre_Keep;
		Run_count = 0;
		Pre_Error_Last = 0;
		Pre_Error_Now = 0;
		Pre_Overshoot = 0;
		Display_Keep_Count = 0;
		Display_Down_Count = 0;
		Overshoot_Count = 0;
		Duty_Beginning = 0;
		Beginning_count = 0;
		Pre_Value_Display = 0;
	}
}
