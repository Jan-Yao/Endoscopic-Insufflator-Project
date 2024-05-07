#include "MainWin.h"

static void Get_Pre_Value(void);
static void Get_Flo_Value(void);
static void Get_Gas_Value(void);
static void Get_Temp_Value(void);
/****************************************************************************************/
/*	功 能： 压力信号处理  						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void TimerProc_Pres(u8 hTimer)
{
	
	Get_Pre_Value();
  Get_Flo_Value();
	Get_Gas_Value();
	Get_Temp_Value();
}
/****************************************************************************************/
/*	功 能： 气压传感器 						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/

static void Get_Pre_Value(void)
{

	u32  Data;
	
	if(ADC_Buffer[1] < 20)	ADC_Buffer[1] = 20;
	
	uPre_Value.Once = ADC_Buffer[1];                                       //采集外部气压
	uPre_Value.Sum = uPre_Value.Sum + uPre_Value.Once;
	
	if(uPre_Value.Sum > uPre_Value.Ave){
		uPre_Value.Sum = uPre_Value.Sum - uPre_Value.Ave;
	}
	uPre_Value.Ave = uPre_Value.Sum >> 3;
	
	Data = uPre_Value.Ave;
	
	if(Data < 300)  Data = 300;
	else            Data = uPre_Value.Ave;
	
	Data = (Data - 280) * 75 /264;
	
	uPre_Value.RunH = (u16)Data ;   //气压值*10
	

}
/****************************************************************************************/
/*	功 能： 流量传感器 						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
static void Get_Flo_Value(void)
{
	

	
	if(ADC_Buffer[2] < 598)  ADC_Buffer[2] = 598;                          //初始值在0V-0.6V之间,Give 598;Flo = 0
		
	
	uFlo_Value.Once = ADC_Buffer[2];                                       //采集外部气压
	uFlo_Value.Sum = uFlo_Value.Sum + uFlo_Value.Once;
	uFlo_Value.Sum = uFlo_Value.Sum - uFlo_Value.Ave;
	uFlo_Value.Ave = uFlo_Value.Sum >> 4;
	
	if(uFlo_Value.Ave < 663){    //流量2.5
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 598)/16; 
	}
	else if(uFlo_Value.Ave < 894){			//流量7
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 437)/56; 
	}
	else if(uFlo_Value.Ave < 1545){		//流量22.5
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 486)/50;
	}
	else{
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 571)/46;
	}
}
/****************************************************************************************/
/*	功 能： 外部气压传感器 						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
static void Get_Gas_Value(void)
{
	
	u8  Data;
	
	if(ADC_Buffer[0] < 200)  ADC_Buffer[0] = 200; 
	
	uGas_Value.Once = ADC_Buffer[0];                                       //采集外部气压
	uGas_Value.Sum = uGas_Value.Sum + uGas_Value.Once;
	uGas_Value.Sum = uGas_Value.Sum - uGas_Value.Ave;
	uGas_Value.Ave = uGas_Value.Sum >> 5;
	
	Data = (u8)((uGas_Value.Ave -200)/90);
	
	if(Data > 7) Data = 7;
	if(uGas_Value.Back != Data){
		AnimationPlayFrame(1,16,Data);
		uGas_Value.Back = Data;
	}
	
}
/****************************************************************************************/
/*	功 能： 外部温度值 						*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
static void Get_Temp_Value(void)
{
	float Temp_Error = 0;
	
	uT1_Value.Once = ADC_Buffer[3];                                       //采集外部气压
	uT1_Value.Sum = uT1_Value.Sum + uT1_Value.Once;
	uT1_Value.Sum = uT1_Value.Sum - uT1_Value.Ave;
	uT1_Value.Ave = uT1_Value.Sum >> 3;
	
	if(uT1_Value.Ave> 2943){       			//小于0度
		uT1_Value.RunF = ((float)uT1_Value.Ave - 2943)/11.45;
		uT1_Value.RunF = 0 - uT1_Value.RunF;
	}
	else if(uT1_Value.Ave> 2502){ 			 //小于20度
		uT1_Value.RunF = ((float)uT1_Value.Ave - 2502)/22.05;
		uT1_Value.RunF = 20 - uT1_Value.RunF;
	}
	else if(uT1_Value.Ave> 2045){					//小于35度
		uT1_Value.RunF = ((float)uT1_Value.Ave - 2045)/30.47;
		uT1_Value.RunF = 35 - uT1_Value.RunF;
	}
	else if(uT1_Value.Ave> 1558){					//小于50度
		uT1_Value.RunF = ((float)uT1_Value.Ave - 1558)/32.47;
		uT1_Value.RunF = 50 - uT1_Value.RunF;
	}
	else {
		uT1_Value.RunF = ((float)uT1_Value.Ave - 1257)/30.1;
		uT1_Value.RunF = 60 - uT1_Value.RunF;
	}
	
	
	
	uT2_Value.Once = ADC_Buffer[4];                                       //采集外部气压
	uT2_Value.Sum = uT2_Value.Sum + uT2_Value.Once;
	uT2_Value.Sum = uT2_Value.Sum - uT2_Value.Ave;
	uT2_Value.Ave = uT2_Value.Sum >> 3;
	
	if(uT2_Value.Ave> 2943){       			//小于0度
		uT2_Value.RunF = ((float)uT2_Value.Ave - 2943)/11.45;
		uT2_Value.RunF = 0 - uT2_Value.RunF;
	}
	else if(uT2_Value.Ave> 2502){ 			 //小于20度
		uT2_Value.RunF = ((float)uT2_Value.Ave - 2502)/22.05;
		uT2_Value.RunF = 20 - uT2_Value.RunF;
	}
	else if(uT2_Value.Ave> 2045){					//小于35度
		uT2_Value.RunF = ((float)uT2_Value.Ave - 2045)/30.47;
		uT2_Value.RunF = 35 - uT2_Value.RunF;
	}
	else if(uT2_Value.Ave> 1558){					//小于50度
		uT2_Value.RunF = ((float)uT2_Value.Ave - 1558)/32.47;
		uT2_Value.RunF = 50 - uT2_Value.RunF;
	}
	else {
		uT2_Value.RunF = ((float)uT2_Value.Ave - 1257)/30.1;
		uT2_Value.RunF = 60 - uT2_Value.RunF;
	}
	
	Temp_Error = uT1_Value.Ave - uT2_Value.Ave;
	if(Temp_Error < 200 && Temp_Error > -200){
		uT_Value.RunF = (uT1_Value.RunF+uT2_Value.RunF)/2;
		if((uT_Value.RunF < 60)&&(uT_Value.RunF > -20)){
			uTemp_State = 1;
		}
		else{
			uTemp_State = 0;
		}
	}
	else{
		uTemp_State = 0;
	}
}



