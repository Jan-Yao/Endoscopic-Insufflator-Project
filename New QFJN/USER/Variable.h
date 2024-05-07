
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
	
	
	
VAR_RANGE   u16   cmd_pos ;                                              //当前指令指针位置
VAR_RANGE   u32   cmd_state;																						 //队列帧尾检测状态
VAR_RANGE   QUEUE que ; 	
VAR_RANGE   u8    cmd_buffer[64];

VAR_RANGE   u8     uLCD_Key;                                                  //LCD命令值
VAR_RANGE   u8     Speak_PWM_Count;                                           //speak count

VAR_RANGE   u8     uLED_State;                                                  //LED状态
VAR_RANGE   u8     uSmoke_State;                                                //排烟状态
VAR_RANGE   u8     uPre_Limit_State;                                            //压力红线检测状态
VAR_RANGE   u8     uJinYe_State;                                                //浸液检测状态

VAR_RANGE DataAD    uGas_Value;
VAR_RANGE DataAD    uFlo_Value;
VAR_RANGE DataAD    uPre_Value;
VAR_RANGE DataAD    uT_Value;
VAR_RANGE DataAD    uT1_Value;
VAR_RANGE DataAD    uT2_Value;

VAR_RANGE HeatStates    uAirPre;      //注气参数
VAR_RANGE HeatStates    uTempPre;    //温度参数

VAR_RANGE  u8     uPre_State ;
VAR_RANGE  u8     uTemp_State ;


VAR_RANGE  u16    PWM_buffer[61];
VAR_RANGE  u8     PWM_Calibration;
	

VAR_RANGE  u32    uBack_GasH ;   //气体总量
VAR_RANGE  u32    uTotal_GasH ;  //气体总量

VAR_RANGE  float  uTotal_GasF ;  //气体总量
VAR_RANGE  float  uValue_GasF ;  //单次流量





