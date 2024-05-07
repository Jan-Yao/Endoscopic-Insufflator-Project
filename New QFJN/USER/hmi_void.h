/*! 
*  \file hmi_void.h
*  \brief �û�MCU��������������
*  \version 1.0
*  \date 2012-2018
*  \copyright ���ݴ�ʹ��Ƽ����޹�˾
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

/*RGB565����*/
#define BLACK     0x0000                                              //��ɫ
#define NAVY      0x000F                                              //����ɫ
#define DGREEN    0x03E0                                              //����ɫ
#define DCYAN     0x03EF                                              //����ɫ
#define MAROON    0x7800                                              //���ɫ
#define PURPLE    0x780F                                              //��ɫ
#define OLIVE     0x7BE0                                              //�����
#define LGRAY     0xC618                                              //�Ұ�ɫ
#define DGRAY     0x7BEF                                              //���ɫ
#define BLUE      0x001F                                              //��ɫ
#define GREEN     0x07E0                                              //��ɫ
#define CYAN      0x07FF                                              //��ɫ
#define RED       0xF800                                              //��ɫ
#define MAGENTA   0xF81F                                              //Ʒ��ɫ
#define YELLOW    0xFFE0                                              //��ɫ
#define WHITE     0xFFFF                                              //��ɫ

/*Display Flag*/
#define Low_Display_OFF  1                                             
#define Low_Display_ON   0  
#define High_Display_OFF 1 
#define High_Display_ON  0
#define Warning_Display_OFF 1 
#define Warning_Display_ON  0
#define Heating_OFF 1 
#define Heating_ON  0

/*Button ����*/
enum Button
{
	Button_Pre_Plus = 5,                                                //ѹ��ֵ�Ӱ�ť
	Button_Pre_Decr,                                                    //ѹ��ֵ����ť
	Button_Flo_Plus,                                                    //����ֵ�Ӱ�ť
	Button_Flo_Decr,                                                    //����ֵ����ť
	Button_Clear_Total = 12,                                            //�ܺ�������0��ť
	Button_ON_OFF,                                                      //START/STOP
	Button_Smoke = 15,                                                  //���̰�ť
	Button_Low_Pre = 19,                                                //��ѹģʽ��ť
	Button_High_Pre,                                                    //��׼ģʽ��ť
};

#define TIME_10MS  1                                                  //10����(1����λ)
#define TIME_100MS 10                                                 //100����(10����λ)

extern uint8  Control_Run_Count;
extern uint16  LCD_Run_count;                                         //��ʱ������

extern uint8  cmd_buffer[CMD_MAX_SIZE];                               //ָ���
extern uint8  Power_State;                                            //����״̬
extern uint8  Gas_Sourse_State;                                       //��Դ״̬
extern uint8  Input_YeTi_State;                                       //��ye״̬
extern uint8  Heat_State;                                             //����״̬
extern uint8  Pre_Limit_State;
extern uint8  Pre_Value;                                              //ѹ���趨ֵ
extern float  Flo_Value;                                              //�����趨ֵ
extern uint8  Pre_Value_ADC;
extern uint8  Pre_Value_Display;
extern float  Flo_Value_ADC;
extern float  Flo_Value_Display;
extern uint16 Gas_Value_ADC;
extern uint16 current_screen_id;                                      //��ǰ����ID
	
/*! 
*  \brief  ��������
*/ 
void UpdateUI(void);

#endif
