/***************************************************************************************/
/*	ע �ͣ�		����һЩ������������ʽ������																						*/
/****************************************************************************************/

#define   MAX_TIMER		  10               //???????

#define   TRUE 	 	      0xFF
#define   FALSE 	   	  0

#define   Win_Key		      1               //��������
#define   Win_Refresh		  2               //ˢ�´���
#define   Win_Trigger		  3               //��ƽ����

#define 	Standby      0  
#define 	Start        1
#define 	Finsh        2 

#define 	CMD_HEAD 0XEE                                                  //֡ͷ
#define 	CMD_TAIL 0XFFFCFFFF                                            //֡β
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

#define   KP         	 0.03		//����ϵ��
#define   KI         	 0.003	  //����ϵ��
#define   KD         	 2	    //΢��ϵ��
#define   KC         	 0.01	  //ά�ֹ���ϵ��


#define 	Pre_Normal  0
#define 	Pre_Low     1
#define 	Pre_High    2

#define   PreSet_Normal    0                                             
#define   PreSet_Limit     1 
#define   PreSet_Over_cap  2 

#define RED       0xF800               //��ɫ
#define WHITE     0xFFFF               //��ɫ
