#include "MainWin.h"

static void DealWithPwmData(void);

/****************************************************************************************/
/*	�� �ܣ� PID�㷨 ��PWM�źŴ���  						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
void TimerProc_PWM(u8 hTimer)
{
		if(uAirPre.Time_Run > 0) uAirPre.Time_Run --;   
		if(uAirPre.Time_Run >= 5){                              //4.5s ON  0.5s OFF
			DealWithPwmData();
		}
		else if(uAirPre.Time_Run < 2){   //ֹͣע��              //һ��ѭ���� 4.5s ON 0.5s OFF
			uAirPre.Time_Run = 50;
		}
		else{
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);   //ֹͣע������
		}

//    DealWithPwmData();
	
//		__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,uAirPre.PWM);   //��������
//		__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,uAirPre.PWM);   //й������
//		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);   //ע������
		
	
}
/****************************************************************************************/
/*	�� �ܣ� PID�㷨					*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�   //460�Ѿ��ضϣ�Ϊ�˱���ѡ400.                                                                          */
/****************************************************************************************/
static void DealWithPwmData(void)
{
	
	
			static u8 Curr = 0;
			float Pout,Iout,Dout,Temp,AirPreP;
			
			uAirPre.CurP    = (float)uPre_Value.RunH;    //ȡ��ǰ��ѹֵ��ת����(float)��ʽ  ADCѹ���Դ�*10Ч��
			AirPreP         = (float)uPre_Value.SetH;    //������ѹֵ
			uAirPre.TargetP = AirPreP * 10;
	
			if((uAirPre.CurP > uAirPre.TargetP/3)&&(uFlo_Value.RunF < uFlo_Value.SetF)){  //��Ϊ�ﵽ�趨���� �� ���趨ѹ��ֵ����ʱ
				uAirPre.TargetP = uAirPre.TargetP + uFlo_Value.RunF;   //������������ Ŀ��ʱ����Ϊ0 ѹ���趨ֵ=����ֵ
			}
			else{                                   //ת������������ �趨������������
				uAirPre.CurP   	= uFlo_Value.RunF;    //ȡ��ǰ����ֵ
				uAirPre.CurP    = uAirPre.CurP*10;
				uAirPre.TargetP = uFlo_Value.SetF;    //��������ֵ
				uAirPre.TargetP = uAirPre.TargetP*10;
			}
	
			Temp = uAirPre.DiffT[Curr]; 
			if(Curr+1>=20)	Curr = 0;
			else 			      Curr += 1;
			
			uAirPre.SumDiff    -= uAirPre.DiffT[Curr];              //ѭ���������֤ʱ���������20���������
			uAirPre.DiffT[Curr] = uAirPre.TargetP - uAirPre.CurP;   //������ѹֵ��ʵ��������ѹֵ
			uAirPre.SumDiff    += uAirPre.DiffT[Curr];
			
			Pout = KP*uAirPre.DiffT[Curr];				    //���������  P*error
			Iout = KI*uAirPre.SumDiff;					      //���������  I*sum.error
			Dout = KD*(uAirPre.DiffT[Curr] - Temp);		//΢�������  D*error-error.last
		
			
			if(Iout > 50)	Iout = 50;                 //���������ޣ���ֹ��
			if(Iout < -50)	Iout = -50;
			
			uAirPre.PID = Pout+Iout+Dout;
			
			if(uAirPre.PID < -20)       uAirPre.PID = -20;  //����ÿ��������������
			else if(uAirPre.PID >= 20)  uAirPre.PID = 20;
			else{
			}
		
			uAirPre.PWM_F += uAirPre.PID;
				
			if(uAirPre.PWM_F > 999)       uAirPre.PWM_F = 999;
			else if(uAirPre.PWM_F < 400)  uAirPre.PWM_F = 400;
			
			uAirPre.PWM = (u16)uAirPre.PWM_F;
			
//			uAirPre.PWM = 650;
			
		 __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);   //ע������

			
}

/****************************************************************************************/
/*	�� �ܣ� PID�㷨 ��PWM�źŴ���  						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
void TimerProcT_PWM(u8 hTimer)
{
		
			static u8 Curr = 0;
			float Pout,Iout,Dout,Temp;
	
			uTempPre.CurP   	= uT_Value.RunF;    //ȡ��ǰ����ֵ��ת����(float)��ʽ
			uTempPre.CurP    = uTempPre.CurP*10;

	
			Temp = uTempPre.DiffT[Curr]; 
			if(Curr+1>=20)	Curr = 0;
			else 			      Curr += 1;
			
			uTempPre.SumDiff    -= uTempPre.DiffT[Curr];
			uTempPre.DiffT[Curr] = uTempPre.TargetP - uTempPre.CurP;   //������ѹֵ��ʵ��������ѹֵ
			uTempPre.SumDiff    += uTempPre.DiffT[Curr];
			
			Pout = KP*uTempPre.DiffT[Curr];				    //���������
			Iout = KI*uTempPre.SumDiff;					      //���������
			Dout = KD*(uTempPre.DiffT[Curr] - Temp);		//΢�������
		
			
			if(Iout > 50)	Iout = 50;
			if(Iout < -50)	Iout = -50;
			
			uTempPre.PID = Pout+Iout+Dout;
			
			if(uTempPre.PID < -30)       uTempPre.PID = -30;
			else if(uTempPre.PID >= 30)  uTempPre.PID = 30;
			else{
			}
		
			uTempPre.PWM_F += uTempPre.PID;
				
			if(uTempPre.PWM_F > 999)       uTempPre.PWM_F = 999;
			else if(uTempPre.PWM_F < 0)  uTempPre.PWM_F = 0;
			
			uTempPre.PWM = (u16)uTempPre.PWM_F;
			
			
		 __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,uTempPre.PWM);   //��������
		
	
}





