/************************************************************************/
/*	描 述：		声明各模块函数的原型																											*/
/************************************************************************/


void  TimerServer(void);
void  KillTimer(u8 hTimer);

void  CloseWindow(void);
void  OpenWindow(WINDOW * Window);
BOOL  RegisterWindow(WINDOW * Window);  //??????
WINPROCPTR  ActiveWindowProc(void);     //??????
void RefreshWindow(void);
u8    SetTimer(u8 hWindow,u16 Interval,TIMERPROCPTR TimerProc);
BOOL  ModBusCRC(u8 *pStream,u8 uLen,BOOL uDir);


void  AnalysisUart2(void);

void  TimerProc_Run(u8 Timer);
void  TimerProc_Temp(u8 Timer);
void  TimerProc_PWM(u8 Timer);
void  TimerProc_Pres(u8 Timer);
void  TimerProcT_PWM(u8 Timer);

void queue_reset(void);
void queue_push(u8 _data);
u16  queue_find_cmd(u8 *cmd,u16 buf_len);

void DelayMS(u16 Data);
void DelayTime(u16 Data);

void PWM_buffer_Init(void);


void SetTextValue(u16 screen_id,u16 control_id,char *str);
void SetTextInt32(u16 screen_id,u16 control_id,u32 value,u8 sign,u8 fill_zero);
void SetTextFloat(u16 screen_id,u16 control_id,float value,u8 precision,u8 show_zeros);
void SetPowerSaving(u8 enable, u8 bl_off_level, u8 bl_on_level, u8  bl_on_time);
void AnimationPlayFrame(u16 screen_id,u16 control_id,u8 frame_id);
void SetTextFcolor(u16 screen_id, u16 control_id, u16 color);

void winMain(void);
void winWork(void);




