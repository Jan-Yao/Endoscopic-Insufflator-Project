/*! 
*  \file hmi_void.h
*  \brief 用户MCU串口驱动函数库
*  \version 1.0
*  \date 2012-2018
*  \copyright 广州大彩光电科技有限公司
*/

#ifndef _HMI_VOID_H
#define _HMI_VOID_H
#include "cmd_process.h"
#include "cmd_queue.h"

#define OFF  0                                             
#define ON   1 

#define Input_YeTi_Disable  0                                             
#define Input_YeTi_Enable   1 

#define Heat_Disable  0                                             
#define Heat_Enable   1 

#define Pre_Normal    0                                             
#define Pre_Limit     1 
#define Pre_Over_cap  2 

#define Gas_None     0  
#define Gas_Level_1  1
#define Gas_Level_2  2 
#define Gas_Level_3  3 
#define Gas_Level_4  4 
#define Gas_Level_5  5
#define Gas_Level_6  6 
#define Gas_Level_7  7 

/*RGB565对照*/
#define BLACK     0x0000                                              //黑色
#define NAVY      0x000F                                              //深蓝色
#define DGREEN    0x03E0                                              //深绿色
#define DCYAN     0x03EF                                              //深青色
#define MAROON    0x7800                                              //深红色
#define PURPLE    0x780F                                              //紫色
#define OLIVE     0x7BE0                                              //橄榄绿
#define LGRAY     0xC618                                              //灰白色
#define DGRAY     0x7BEF                                              //深灰色
#define BLUE      0x001F                                              //蓝色
#define GREEN     0x07E0                                              //绿色
#define CYAN      0x07FF                                              //青色
#define RED       0xF800                                              //红色
#define MAGENTA   0xF81F                                              //品红色
#define YELLOW    0xFFE0                                              //黄色
#define WHITE     0xFFFF                                              //白色

/*Display Flag*/
#define Low_Display_OFF  1                                             
#define Low_Display_ON   0  
#define High_Display_OFF 1 
#define High_Display_ON  0
#define Warning_Display_OFF 1 
#define Warning_Display_ON  0
#define Heating_OFF 1 
#define Heating_ON  0

/*Button 对照*/
enum Button
{
	Button_Pre_Plus = 5,                                                //压力值加按钮
	Button_Pre_Decr,                                                    //压力值减按钮
	Button_Flo_Plus,                                                    //流量值加按钮
	Button_Flo_Decr,                                                    //流量值减按钮
	Button_Clear_Total = 12,                                            //总耗气量清0按钮
	Button_ON_OFF,                                                      //START/STOP
	Button_Smoke = 15,                                                  //除烟按钮
	Button_Low_Pre = 19,                                                //低压模式按钮
	Button_High_Pre,                                                    //标准模式按钮
};

#define TIME_10MS  1                                                  //10毫秒(1个单位)
#define TIME_100MS 10                                                 //100毫秒(10个单位)

extern uint8  Control_Run_Count;
extern uint16  LCD_Run_count;                                         //定时器节拍

extern uint8  cmd_buffer[CMD_MAX_SIZE];                               //指令缓存
extern uint8  Power_State;                                            //开机状态
extern uint8  Gas_Sourse_State;                                       //气源状态
extern uint8  Input_YeTi_State;                                       //进ye状态
extern uint8  Heat_State;                                             //加热状态
extern uint8  Pre_Limit_State;
extern uint8  Pre_Value;                                              //压力设定值
extern float  Flo_Value;                                              //流量设定值
extern uint8  Pre_Value_ADC;
extern uint8  Pre_Value_Display;
extern float  Flo_Value_ADC;
extern float  Flo_Value_Display;
extern uint16 Gas_Value_ADC;
extern uint16 current_screen_id;                                      //当前画面ID
	
/*! 
*  \brief  更新数据
*/ 
void UpdateUI(void);

#endif
