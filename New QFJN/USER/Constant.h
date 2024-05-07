/***************************************************************************************/
/*	注 释：		定义一些常数的助记形式、常量																						*/
/****************************************************************************************/

#define   MAX_TIMER		  10               //???????

#define   TRUE 	 	      0xFF
#define   FALSE 	   	  0

#define   Win_Key		      1               //按键功能
#define   Win_Refresh		  2               //刷新窗口
#define   Win_Trigger		  3               //电平触发

#define 	Standby      0  
#define 	Start        1
#define 	Finsh        2 

#define 	CMD_HEAD 0XEE                                                  //帧头
#define 	CMD_TAIL 0XFFFCFFFF                                            //帧尾
#define 	CMD_MAX_SIZE 64   


#define 	OFF  0                                             
#define 	ON   1  

                                             
#define 	Low_Sign    10  
#define 	High_Sign   9
#define 	Warn_Sign   24
#define 	Heat_Sign   25
#define 	Work_Sign   18
#define 	Smoke_Sign  17
#define 	ON_and_OFF  14

#define   KP         	 0.03		//比例系数
#define   KI         	 0.003	  //积分系数
#define   KD         	 2	    //微分系数
#define   KC         	 0.01	  //维持功率系数


#define 	Pre_Normal  0
#define 	Pre_Low     1
#define 	Pre_High    2

#define   PreSet_Normal    0                                             
#define   PreSet_Limit     1 
#define   PreSet_Over_cap  2 

#define RED       0xF800               //红色
#define WHITE     0xFFFF               //白色
