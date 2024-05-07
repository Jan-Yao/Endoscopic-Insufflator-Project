


/***************************************************************/
/*	? ?:	????,???				*/
/***************************************************************/
typedef unsigned char  u8;
typedef unsigned short u16;
//typedef unsigned int   u16;
typedef unsigned long  u32;
typedef unsigned char	BOOL;
typedef void ( * WINPROCPTR)(u8,u8);
typedef void ( * TIMERPROCPTR)(u8);
/****************************************************************************************/
/*					?????					*/
/****************************************************************************************/
typedef struct tagTIMER{
					u8					hWindow;
					u16					TickCounter;      //定时counter值
					u16					Interval;         //定时间隔
					TIMERPROCPTR	    TimerProc;  // -> void
					}       TIMER;
/****************************************************************************************/
/*					??????																																*/
/****************************************************************************************/
typedef struct tagWINDOW	{
					WINPROCPTR	WindowProc;  // -> void
					u8	    Active;          // TURE or FALSE
					u16*    Address;         // -> void 的 address
					}	WINDOW;	
/****************************************************************************/
/*			??????                    			*/
/****************************************************************************/
// MODBUS ????
#define MAX_MODBUS	62   // ????
#define MODBUS_STY	0		 // ??
#define MODBUS_REC	1		 // ???
#define MODBUS_RECD	2		 // ???
#define MODBUS_TICK	3		 // ????
#define MODBUS_TRN	4		 // ???
#define MODBUS_TRND	5		 // ????
#define MODBUS_ERR	6
typedef struct tagMODBUS{
  u8  RecStatus;
	u8  ID;					// ??
	u8  MAX_Len;			// ???????
	u8  T_Index;		    // ??????
	u8  R_Index;			// ??????
	u16 RetCou;			    // ????
	u16 RetAdr;
	u8  RetBitS;
	u8  Buffer[MAX_MODBUS];
}MODBUS;

#define QUEUE_MAX_SIZE	512   // ????
typedef struct _QUEUE                                             
{                                                                 
    u16  _head;                                                       //队列头
    u16 _tail;                                                       //队列尾
    u8 _data[QUEUE_MAX_SIZE];                                       //队列数据缓存区
}QUEUE; 

typedef struct tagDataAD                                             
{   
		u8   		Back;                 //设置值
		u8   		SetH;                 //设置值
		u8   		DisH;								//显示值 
		u16  		RunH; 								//运行值,16进制显示小数点后一位
		float   SetF;                 //设置值
		float   DisF;								//显示值 
		float   RunF; 								//运行值
    u16  		Once;                //一次采样值
		u16  		Ave; 								//平均采样值
    u32  		Sum;                 //多次采样和
    
}DataAD; 


typedef struct tagHeatStates{
	 u8  State;         //状态

	 u16  PWM;       //主温度值
	 u16  Time_Run;  //运行时间
	
	 float	PWM_F;      //浮点型PWM值
	 float	PID;         //当前温度值
   float	CurP;         //当前温度值
   float	DiffT[20];   //当前温度值	20次温度差
   float	SumDiff;     //20此温度差和
   float	TargetP;     //设置温度值
}HeatStates;







