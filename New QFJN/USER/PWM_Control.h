#ifndef _PWM_CONTROL_H
#define _PWM_CONTROL_H

#include "main.h"

#define Buz_OFF      0  
#define Buz_ON_Short 1
#define Buz_ON_Long  2 
#define Buz_ON_Warn  3 
#define Buz_Over_Pre 4 

#define Pre_Keep     0                                             
#define Pre_Up       1 
#define Pre_Down     2

#define Standby      0  
#define Start        1
#define Finsh        2 

typedef struct
{
	float target_val;    //设定值
	float err;           //Error 
	float err_last;      //last Error
	float Kp,Ki,Kd;      //系数
	float integral;      //sum
	float output_val;    //输出值
}PID;

extern uint8_t   Pre_State;
extern uint8_t   Buz_State;
extern uint8_t   PWM_Calibration;
extern float     Temp_ADC;
extern int16_t   Pre_Error;
extern int16_t   Pre_Error_AVG;
extern uint16_t  PWM_buffer[61];

void PWM_buffer_Init(void);
void Gas_Input_Detect(void);
void PWM_Control(void);
void Input_PWM_Calibration(void);

#endif
