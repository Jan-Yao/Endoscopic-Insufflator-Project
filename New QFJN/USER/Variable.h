
#ifdef MAIN
	#define VAR_RANGE
#else
	#define VAR_RANGE		extern
#endif

	
VAR_RANGE   u16       TickCounter;
VAR_RANGE   u16       WinCounter;
VAR_RANGE   u16       WinCounter_last;
	
VAR_RANGE   u8        TickRecevice;   						 // ??????
VAR_RANGE   TIMER     TimerPool[MAX_TIMER];       // ????
VAR_RANGE   MODBUS    ModbusMessage;              //????


VAR_RANGE   u8      hActiveWindow;	                  //????????
VAR_RANGE WINDOW	* OpenWndCollection[15];    		//??????



VAR_RANGE   u8        uTime_Run;
VAR_RANGE   u8        uTime_Temp;
VAR_RANGE   u8        uTime_PWM;
VAR_RANGE   u8        uTime_Pres;
VAR_RANGE   u8        uTimeT_PWM;
	
	
	
VAR_RANGE   u16   cmd_pos ;                                              //��ǰָ��ָ��λ��
VAR_RANGE   u32   cmd_state;																						 //����֡β���״̬
VAR_RANGE   QUEUE que ; 	
VAR_RANGE   u8    cmd_buffer[64];

VAR_RANGE   u8     uLCD_Key;                                                  //LCD����ֵ
VAR_RANGE   u8     Speak_PWM_Count;                                           //speak count

VAR_RANGE   u8     uLED_State;                                                  //LED״̬
VAR_RANGE   u8     uSmoke_State;                                                //����״̬
VAR_RANGE   u8     uPre_Limit_State;                                            //ѹ�����߼��״̬
VAR_RANGE   u8     uJinYe_State;                                                //��Һ���״̬

VAR_RANGE DataAD    uGas_Value;
VAR_RANGE DataAD    uFlo_Value;
VAR_RANGE DataAD    uPre_Value;
VAR_RANGE DataAD    uT_Value;
VAR_RANGE DataAD    uT1_Value;
VAR_RANGE DataAD    uT2_Value;

VAR_RANGE HeatStates    uAirPre;      //ע������
VAR_RANGE HeatStates    uTempPre;    //�¶Ȳ���

VAR_RANGE  u8     uPre_State ;
VAR_RANGE  u8     uTemp_State ;


VAR_RANGE  u16    PWM_buffer[61];
VAR_RANGE  u8     PWM_Calibration;
	

VAR_RANGE  u32    uBack_GasH ;   //��������
VAR_RANGE  u32    uTotal_GasH ;  //��������

VAR_RANGE  float  uTotal_GasF ;  //��������
VAR_RANGE  float  uValue_GasF ;  //��������





