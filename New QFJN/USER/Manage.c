
#include "MainWin.h"
/***********************************************************************************/
/*	�� �ܣ�	��ʱ����������������ʱ�����¼���				*/
/*	�� �룺									*/
/*	�� �أ�									*/
/*	ע �ͣ�	�¼���������趨��ʱ����					*/
/**********************************************************************************/
void TimerServer(void)
{
	static u16 LastTickCounter=0;
	u16 TimePass;
	u8  i;
	TimePass = TickCounter - LastTickCounter;							       
	if (TickCounter != LastTickCounter) {
		LastTickCounter = TickCounter;		                          //time count transmit
		if (TimerPool[0].Interval != 0) {
			TimerPool[0].TickCounter += TimePass;
			if (TimerPool[0].TickCounter >= TimerPool[0].Interval) {
				TimerPool[0].TickCounter = 0;								            // ȷ���¼������С���趨��ʱ����
				(*TimerPool[0].TimerProc)(0);
			}
		}		
		for (i=1; i<MAX_TIMER; i++) {
			if (TimerPool[i].Interval != 0) {
				TimerPool[i].TickCounter += TimePass;
				if (TimerPool[i].TickCounter >= TimerPool[i].Interval) {
					TimerPool[i].TickCounter = 0;							            	// ȷ���¼������С���趨��ʱ����
					(*TimerPool[i].TimerProc)(i);
				}
			}
		}
	}
	
}

/*************************************************************************************/
/*	�� �ܣ�	������ʱ��							 */
/*	�� �룺	hWindow	 ___ ��ʱ�������򴴽���ʱ���Ĵ��ڵľ��			*/
/*					interval ___ ��ʱ���			*/																			       			
/*					TimeProc ___ ��ʱ���������������ã��Ļص���ʱ����ָ�룬��Ϊ0��������Ϣ����ǰ */
/*					���ڵĴ��ں�������ʱ����ԭ�� void TimerProc(hTimer)  */
/*	�� �أ�	��ʱ���ı�ʶ���						*/																						
/*	ע �ͣ�	��Ҫ��ȷ��ʱ�Ĳ������ܲ��ô��ඨʱ����ʽ��Ӧֱ���ɶ�ʱ�жϵ���	*/
/*************************************************************************************/


u8 SetTimer(u8 hWindow,u16 Interval,TIMERPROCPTR TimerProc)
{
	u8 i;
	for (i=0; i<MAX_TIMER; i++) {						// �ڶ�ʱ��������Ѱһ�����õ�λ��
		if (TimerPool[i].Interval == 0) {
			TimerPool[i].TickCounter = 0;
			TimerPool[i].hWindow = hWindow;
			TimerPool[i].Interval = Interval;
			TimerPool[i].TimerProc= TimerProc;
			return(i);												// ����ʱ�������ɹ����򷵻����ľ������ţ�
		}
	}
	return (-1);								          // ����ʧ��
}


/****************************************************************************************/
/*	�� �ܣ�	�ͷŶ�ʱ��             						*/
/*	�� �룺									*/
/*	�� �أ�									*/
/*	ע �ͣ�									*/
/****************************************************************************************/

void KillTimer(u8 hTimer)
{
	TimerPool[hTimer].Interval 	= 0;	// ��ն�ʱ���
	TimerPool[hTimer].TimerProc = 0;	// ��ն�ʱʱ����

}

/****************************************************************************************/
/*	�� �ܣ�	ע�ᴰ��                        				*/
/*	�� �룺									*/
/*	�� ����									 */
/*	ע �ͣ�									 */
/****************************************************************************************/
BOOL RegisterWindow(WINDOW * Window)
{
	if (hActiveWindow < 14){
		if( hActiveWindow>0 || (u16)(&OpenWndCollection[0]->WindowProc)>0)hActiveWindow++;
		OpenWndCollection[hActiveWindow] = Window;
		return( 1 );
	}
	else {
		return ( 0 );
	}
}
/****************************************************************************************/
/*	�� �ܣ�	��һ���´���							*/
/*	�� �룺									*/
/*	�� ����									*/
/*	ע �ͣ�									 */
/****************************************************************************************/
void OpenWindow(WINDOW * Window)
{
	Window->Active = TRUE;
}
/****************************************************************************************/
/*	�� �ܣ�	�رյ�ǰ����      						  */
/*	�� �룺									 */
/*	�� ����									  */
/*	ע �ͣ�									 */
/****************************************************************************************/
void CloseWindow(void)
{
	if (hActiveWindow > 0) {
		OpenWndCollection[hActiveWindow]->Active = FALSE;
		OpenWndCollection[hActiveWindow] = 0;
		hActiveWindow--;																						
	}
}
/****************************************************************************************/
/*	�� �ܣ�	��ȡ�����                      				 */
/*	�� �룺									*/
/*	�� ����									 */
/*	ע �ͣ�									*/
/****************************************************************************************/
WINPROCPTR ActiveWindowProc(void)
{
	return (OpenWndCollection[hActiveWindow]->WindowProc);
}
/****************************************************************************************/
/*	�� �ܣ�	ˢ�»����                      				 */
/*	�� �룺									*/
/****************************************************************************************/
void RefreshWindow(void)
{
	(*ActiveWindowProc())(Win_Refresh, 0);
}

/****************************************************************************************/
/*	�� �ܣ� ����ָʾ����˸  						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
void TimerProc_Run(u8 hTimer)
{
	HAL_IWDG_Refresh(&hiwdg); 
	
	float AirPreP;
	
	uPre_Value.RunF = (float)uPre_Value.RunH;
	AirPreP  = uPre_Value.RunF - uFlo_Value.RunF;
	if(AirPreP < 0)  AirPreP = 0;
	uPre_Value.DisH = (u8)(AirPreP/10);
	
	
  if(uPre_Value.Once	> 2300){   //3000=80mmHg;2300=60mmHg;1800=48mmHg;2000=53mmHg
			uAirPre.State = 1;
			if(uTime_PWM > 0){
				uAirPre.PWM = 400;  //�ر�ע����
			}
			else{
				uAirPre.PWM = 0;
			}
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,600);  //15������й��
	}
	else if(uPre_Value.DisH > uPre_Value.SetH){  
		if(uPre_Value.SetH >= 10){  
			if(uPre_Value.DisH > uPre_Value.SetH*1.5){       //��ʾ�����趨��1.5������ǰ�����趨ֵ����10
				uAirPre.State = 1;
				if(uTime_PWM > 0){
					uAirPre.PWM = 400;  //�ر�ע����
				}
				else{
					uAirPre.PWM = 0;
				}
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,500);  //С����й��
			}
			else{
				if(uAirPre.State == 1){ //����û�г�ѹ���ر�й��
					uAirPre.State = 0;
					__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);
				}
			}
		}
		else{ 
			if(uPre_Value.DisH > uPre_Value.SetH+5){          //��ʾ�����趨+5
				uAirPre.State = 1;
				if(uTime_PWM > 0){
					uAirPre.PWM = 400;  //�ر�ע����
				}
				else{
					uAirPre.PWM = 0;
				}
				__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,uAirPre.PWM);
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,450);  //С����й��
			}
			else{
				if(uAirPre.State == 1){ //����û�г�ѹ���ر�й��
					uAirPre.State = 0;
					__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);
				}
			}
		}
	}
	else{
		if(uAirPre.State == 1){ //����û�г�ѹ���ر�й��
			uAirPre.State = 0;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,0);
		}
	}	

}
/****************************************************************************************/
/*	�� �ܣ� �¶�ɨ�� 						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
void TimerProc_Temp(u8 hTimer)
{
	
		float AirPreP;
	  u8    Input_Stat;
	
	
		if(uLED_State > 0){
			uLED_State = 0;
			LED_ON();
		}
		else{
			uLED_State = 1;
			LED_OFF();
		}
		
		
		uFlo_Value.DisF = uFlo_Value.RunF;
		if(uFlo_Value.DisF < 0)  uFlo_Value.DisF = 0;
		
		uPre_Value.RunF = (float)uPre_Value.RunH;
		AirPreP  = uPre_Value.RunF - uFlo_Value.RunF;
		if(AirPreP < 0)  AirPreP = 0;
		uPre_Value.DisH = (u8)(AirPreP/10);
		
		SetTextInt32(1,1,uPre_Value.DisH,0,2); 			
		SetTextFloat(1,2,uFlo_Value.DisF,1,1);
		
		
		if(uPre_Value.DisH > uPre_Value.SetH + 3){
				if(uPre_State != Pre_High){
					AnimationPlayFrame(1,High_Sign, ON);
					AnimationPlayFrame(1,Low_Sign, OFF);
					uPre_State = Pre_High;            
				}
		}
		else if(uPre_Value.DisH < uPre_Value.SetH - 3){
				if(uPre_State != Pre_Low){
					AnimationPlayFrame(1,Low_Sign, ON);
					AnimationPlayFrame(1,High_Sign, OFF);
					uPre_State = Pre_Low;
				}
		}
		else{
				if(uPre_State == Pre_High){
					AnimationPlayFrame(1,High_Sign, OFF);
				}
				else if(uPre_State == Pre_Low){
					AnimationPlayFrame(1,Low_Sign, OFF);
				}
				else{
				}
				uPre_State = Pre_Normal;
		}
		
		
		uValue_GasF = uFlo_Value.DisF * 0.0033;   // 2s �Ӽ�һ��
		uTotal_GasF = uTotal_GasF + uValue_GasF;
		
		uBack_GasH = uTotal_GasH;
		uTotal_GasH = (u32)uTotal_GasF;
		if(uBack_GasH != uTotal_GasH){
			SetTextInt32(1, 11, uTotal_GasH, 0, 3);  
		}
		
		switch(uPre_Limit_State)
		{
			case PreSet_Normal:
//				SetTextFcolor(1,3,WHITE);                                     //��ʾ��ɫ
				break;
			case PreSet_Limit:
//				SetTextFcolor(1,3,RED);                                       //��ʾ��ɫ
				break;
			case PreSet_Over_cap:                                             //��ʾ2S��ɫ to ��ʾ��ɫ
				uPre_Limit_State = Pre_Normal;
				SetTextFcolor(1,3,WHITE);                                       //��ʾ��ɫ
				break;
			default:
				break;
		}
		
		Input_Stat = Input_YeTi_Read();
		if(Input_Stat == uJinYe_State){           //��Һ���
			if(uJinYe_State == 1){                  //Input_Stat = uJinYe_State = 1
				AnimationPlayFrame(1,Warn_Sign,ON);	
			}
			else{                                   //Input_Stat = uJinYe_State = 0
//				AnimationPlayFrame(1,Warn_Sign,OFF);
			}
		}
		else{                                     //Input_Stat != uJinYe_State
			uJinYe_State = Input_Stat;
			AnimationPlayFrame(1,Warn_Sign,OFF);	
		}
}

/****************************************************************************************/
/*	�� �ܣ� ������ʱ  						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/

void DelayMS(u16 Data) 
{
    u16 i,j;  
    for(i = Data;i>0;i--)
        for(j=1000;j>0;j--) ; 
}
/****************************************************************************************/
/*	�� �ܣ� ������ʱ  						*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
void DelayTime(u16 Data)
{
		u16 i,j;  
	
    for(i = Data;i>0;i--){
			for(j=10000;j>0;j--);
			HAL_IWDG_Refresh(&hiwdg);  
		}			
}


