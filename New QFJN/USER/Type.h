


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
					u16					TickCounter;      //��ʱcounterֵ
					u16					Interval;         //��ʱ���
					TIMERPROCPTR	    TimerProc;  // -> void
					}       TIMER;
/****************************************************************************************/
/*					??????																																*/
/****************************************************************************************/
typedef struct tagWINDOW	{
					WINPROCPTR	WindowProc;  // -> void
					u8	    Active;          // TURE or FALSE
					u16*    Address;         // -> void �� address
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
    u16  _head;                                                       //����ͷ
    u16 _tail;                                                       //����β
    u8 _data[QUEUE_MAX_SIZE];                                       //�������ݻ�����
}QUEUE; 

typedef struct tagDataAD                                             
{   
		u8   		Back;                 //����ֵ
		u8   		SetH;                 //����ֵ
		u8   		DisH;								//��ʾֵ 
		u16  		RunH; 								//����ֵ,16������ʾС�����һλ
		float   SetF;                 //����ֵ
		float   DisF;								//��ʾֵ 
		float   RunF; 								//����ֵ
    u16  		Once;                //һ�β���ֵ
		u16  		Ave; 								//ƽ������ֵ
    u32  		Sum;                 //��β�����
    
}DataAD; 


typedef struct tagHeatStates{
	 u8  State;         //״̬

	 u16  PWM;       //���¶�ֵ
	 u16  Time_Run;  //����ʱ��
	
	 float	PWM_F;      //������PWMֵ
	 float	PID;         //��ǰ�¶�ֵ
   float	CurP;         //��ǰ�¶�ֵ
   float	DiffT[20];   //��ǰ�¶�ֵ	20���¶Ȳ�
   float	SumDiff;     //20���¶Ȳ��
   float	TargetP;     //�����¶�ֵ
}HeatStates;







