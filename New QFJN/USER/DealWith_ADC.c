#include "MainWin.h"

static void Get_Pre_Value(void);
static void Get_Flo_Value(void);
static void Get_Gas_Value(void);
static void Get_Temp_Value(void);
/****************************************************************************************/
/*	�� �ܣ� ѹ���źŴ���  						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
void TimerProc_Pres(u8 hTimer)
{
	
	Get_Pre_Value();
  Get_Flo_Value();
	Get_Gas_Value();
	Get_Temp_Value();
}
/****************************************************************************************/
/*	�� �ܣ� ��ѹ������ 						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/

static void Get_Pre_Value(void)
{

	u32  Data;
	
	if(ADC_Buffer[1] < 20)	ADC_Buffer[1] = 20;
	
	uPre_Value.Once = ADC_Buffer[1];                                       //�ɼ��ⲿ��ѹ
	uPre_Value.Sum = uPre_Value.Sum + uPre_Value.Once;
	
	if(uPre_Value.Sum > uPre_Value.Ave){
		uPre_Value.Sum = uPre_Value.Sum - uPre_Value.Ave;
	}
	uPre_Value.Ave = uPre_Value.Sum >> 3;
	
	Data = uPre_Value.Ave;
	
	if(Data < 300)  Data = 300;
	else            Data = uPre_Value.Ave;
	
	Data = (Data - 280) * 75 /264;
	
	uPre_Value.RunH = (u16)Data ;   //��ѹֵ*10
	

}
/****************************************************************************************/
/*	�� �ܣ� ���������� 						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
static void Get_Flo_Value(void)
{
	

	
	if(ADC_Buffer[2] < 598)  ADC_Buffer[2] = 598;                          //��ʼֵ��0V-0.6V֮��,Give 598;Flo = 0
		
	
	uFlo_Value.Once = ADC_Buffer[2];                                       //�ɼ��ⲿ��ѹ
	uFlo_Value.Sum = uFlo_Value.Sum + uFlo_Value.Once;
	uFlo_Value.Sum = uFlo_Value.Sum - uFlo_Value.Ave;
	uFlo_Value.Ave = uFlo_Value.Sum >> 4;
	
	if(uFlo_Value.Ave < 663){    //����2.5
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 598)/16; 
	}
	else if(uFlo_Value.Ave < 894){			//����7
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 437)/56; 
	}
	else if(uFlo_Value.Ave < 1545){		//����22.5
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 486)/50;
	}
	else{
		uFlo_Value.RunF = ((float)uFlo_Value.Ave - 571)/46;
	}
}
/****************************************************************************************/
/*	�� �ܣ� �ⲿ��ѹ������ 						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
static void Get_Gas_Value(void)
{
	
	u8  Data;
	
	if(ADC_Buffer[0] < 200)  ADC_Buffer[0] = 200; 
	
	uGas_Value.Once = ADC_Buffer[0];                                       //�ɼ��ⲿ��ѹ
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
/*	�� �ܣ� �ⲿ�¶�ֵ 						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
static void Get_Temp_Value(void)
{
	float Temp_Error = 0;
	
	uT1_Value.Once = ADC_Buffer[3];                                       //�ɼ��ⲿ��ѹ
	uT1_Value.Sum = uT1_Value.Sum + uT1_Value.Once;
	uT1_Value.Sum = uT1_Value.Sum - uT1_Value.Ave;
	uT1_Value.Ave = uT1_Value.Sum >> 3;
	
	if(uT1_Value.Ave> 2943){       			//С��0��
		uT1_Value.RunF = ((float)uT1_Value.Ave - 2943)/11.45;
		uT1_Value.RunF = 0 - uT1_Value.RunF;
	}
	else if(uT1_Value.Ave> 2502){ 			 //С��20��
		uT1_Value.RunF = ((float)uT1_Value.Ave - 2502)/22.05;
		uT1_Value.RunF = 20 - uT1_Value.RunF;
	}
	else if(uT1_Value.Ave> 2045){					//С��35��
		uT1_Value.RunF = ((float)uT1_Value.Ave - 2045)/30.47;
		uT1_Value.RunF = 35 - uT1_Value.RunF;
	}
	else if(uT1_Value.Ave> 1558){					//С��50��
		uT1_Value.RunF = ((float)uT1_Value.Ave - 1558)/32.47;
		uT1_Value.RunF = 50 - uT1_Value.RunF;
	}
	else {
		uT1_Value.RunF = ((float)uT1_Value.Ave - 1257)/30.1;
		uT1_Value.RunF = 60 - uT1_Value.RunF;
	}
	
	
	
	uT2_Value.Once = ADC_Buffer[4];                                       //�ɼ��ⲿ��ѹ
	uT2_Value.Sum = uT2_Value.Sum + uT2_Value.Once;
	uT2_Value.Sum = uT2_Value.Sum - uT2_Value.Ave;
	uT2_Value.Ave = uT2_Value.Sum >> 3;
	
	if(uT2_Value.Ave> 2943){       			//С��0��
		uT2_Value.RunF = ((float)uT2_Value.Ave - 2943)/11.45;
		uT2_Value.RunF = 0 - uT2_Value.RunF;
	}
	else if(uT2_Value.Ave> 2502){ 			 //С��20��
		uT2_Value.RunF = ((float)uT2_Value.Ave - 2502)/22.05;
		uT2_Value.RunF = 20 - uT2_Value.RunF;
	}
	else if(uT2_Value.Ave> 2045){					//С��35��
		uT2_Value.RunF = ((float)uT2_Value.Ave - 2045)/30.47;
		uT2_Value.RunF = 35 - uT2_Value.RunF;
	}
	else if(uT2_Value.Ave> 1558){					//С��50��
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



