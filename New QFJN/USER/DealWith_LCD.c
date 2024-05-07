#include "MainWin.h"


#define TX_8(P1)    SEND_DATA((P1)&0xFF)                    //发送单个字节
#define TX_8N(P,N)  SendNU8((uint8 *)P,N)                 //发送N个字节
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)                 //发送16位整数
#define TX_16N(P,N) SendNU16((uint16 *)P,N)              //发送N个16位整数
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)     //发送32位整数

#define SEND_DATA(P) SendChar(P)          //发送一个字节
#define BEGIN_CMD()  TX_8(0XEE)            //帧头
#define END_CMD()    TX_32(0XFFFCFFFF)       //帧尾

/****************************************************************************************/
/*	功 能： 清空指令					*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void  AnalysisUart2(void)
{
	u8  size = 0;
	
	size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE);
  if((size > 0 && cmd_buffer[9] > 0)){
		uLCD_Key = cmd_buffer[6];
		
		if(cmd_buffer[9] == 2){
			uLCD_Key = uLCD_Key+0x80;
		}
		
		(*ActiveWindowProc())(Win_Key,uLCD_Key);  //Applies to the current window
		
	}
  
}
/****************************************************************************************/
/*	功 能： 从队列中取一个数据		*/
/*	注 释：	            	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
static void queue_pop(u8* _data)
{
    if(que._tail!=que._head)                                          //非空状态
    {
        *_data = que._data[que._tail];
        que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
    }
}
/****************************************************************************************/
/*	功 能： 获取队列中有效数据个数	*/
/*	注 释：	            	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
static u16 queue_size()
{
    return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}
/****************************************************************************************/
/*	功 能： 清空指令					*/
/*	注 释：	现在是只读一个字节没有备份，也没有校验             	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void queue_reset()
{
    que._head = que._tail = 0;
    cmd_pos = cmd_state = 0;
}

/****************************************************************************************/
/*	功 能： 添加指令数据 						*/
/*	注 释：	cmd 指令接收缓存区  ;buf_len 指令接收缓存区大小 ; buf_len 指令接收缓存区大小   	*/
/*	入 口：                                           			*/
/*  出 口： 指令长度，0表示队列中无完整指令                                                                            */
/****************************************************************************************/
void queue_push(u8 _data)
{
    u16 pos = (que._head+1)%QUEUE_MAX_SIZE;
    if(pos!=que._tail)                                                //非满状态
    {
        que._data[que._head] = _data;
        que._head = pos;
    }
}
/****************************************************************************************/
/*	功 能： 从指令队列中取出一条完整的指令						*/
/*	注 释：	串口接收的数据，通过此函数放入指令队列  ； _data 指令数据        	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/

u16 queue_find_cmd(u8 *buffer,u16 buf_len)
{
    u16 cmd_size = 0;
    u8 _data = 0;

    while(queue_size()>0)
    {
        //取一个数据
        queue_pop(&_data);

        if(cmd_pos==0&&_data!=CMD_HEAD)                               //指令第一个字节必须是帧头，否则跳过
        {
            continue;
        }
        //    LED2_ON;
        if(cmd_pos<buf_len)                                           //防止缓冲区溢出
            buffer[cmd_pos++] = _data;

        cmd_state = ((cmd_state<<8)|_data);                           //拼接最后4个字节，组成一个32位整数

        //最后4个字节与帧尾匹配，得到完整帧
        if(cmd_state==CMD_TAIL)
        {
            //LED2_ON;
            cmd_size = cmd_pos;                                       //指令字节长度
            cmd_state = 0;                                            //重新检测帧尾巴
            cmd_pos = 0;                                              //复位指令指针
						uLCD_Key = 0;
#if(CRC16_ENABLE)
            //去掉指令头尾EE，尾FFFCFFFF共计5个字节，只计算数据部分CRC
            if(!CheckCRC16(buffer+1,cmd_size-5))                      //CRC校验
                return 0;

            cmd_size -= 2;                                            //去掉CRC16（2字节）
#endif
            return cmd_size;
        }
    }
    return 0;                                                         //没有形成完整的一帧
}

/****************************************************************************************/
/*	功 能：串口发送送字符串					*/
/*	注 释：	字符串    	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void SendStrings(char *str)
{
    while(*str)
    {
        TX_8(*str);
        str++;
    }
}
/****************************************************************************************/
/*	功 能： 从指令队列中取出一条完整的指令						*/
/*	注 释：	串口接收的数据，通过此函数放入指令队列  ； _data 指令数据        	*/
/*	入 口：                                           			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void SetTextValue(u16 screen_id,u16 control_id,char *str)
{
		BEGIN_CMD();
    TX_8(0xB1);
    TX_8(0x10);
    TX_16(screen_id);
    TX_16(control_id);
    SendStrings(str);
    END_CMD();
	
}

/****************************************************************************************/
/*	功 能： 设置文本为整数值，要求FIRMWARE_VER>=908						*/
/*	注 释：	param  screen_id 画面ID;param  control_id 控件ID ;value 文本数值; sign 0-无符号，1-有符号       	*/
/*	入 口： fill_zero 数字位数，不足时左侧补零                                          			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void SetTextInt32(u16 screen_id,u16 control_id,u32 value,u8 sign,u8 fill_zero)
{
    BEGIN_CMD();
    TX_8(0xB1);
    TX_8(0x07);
    TX_16(screen_id);
    TX_16(control_id);
    TX_8(sign?0X01:0X00);
    TX_8((fill_zero&0x0f)|0x80);
    TX_32(value);
    END_CMD();
}
/****************************************************************************************/
/*	功 能： 设置文本单精度浮点值，要求FIRMWARE_VER>=908   		*/
/*	注 释：	screen_id 画面ID;param   control_id 控件ID ;value 文本数值; sign 0-无符号，1-有符号       	*/
/*	入 口： fill_zero 数字位数，不足时左侧补零                                          			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void SetTextFloat(u16 screen_id,u16 control_id,float value,u8 precision,u8 show_zeros)
{
	u8 i = 0;

	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x07);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8(0x02);
	TX_8((precision&0x0f)|(show_zeros?0x80:0x00));
	
	for (i=0;i<4;++i)
	{
	 //需要区分大小端
		TX_8(((u8 *)&value)[3-i]);
	}
	END_CMD();
}
/*! 
*  \brief     播放制定帧
*  \param  screen_id 画面ID
*  \param  control_id 控件ID
*  \param  frame_id 帧ID
*/

/*! 
*  \brief  自动调节背光亮度
*  \param  enable 使能
*  \param  bl_off_level 待机亮度
*  \param  bl_on_level  激活亮度
*  \param  bl_on_time  偏移量个数
*/
void SetPowerSaving(u8 enable, u8 bl_off_level, u8 bl_on_level, u8  bl_on_time)
{
    BEGIN_CMD();
    TX_8(0x77);
    TX_8(enable);
    TX_8(bl_off_level);
    TX_8(bl_on_level);
    TX_8(bl_on_time);
    END_CMD();
}

/****************************************************************************************/
/*	功 能： \brief     播放制定帧   		*/
/*	注 释：	\param  screen_id 画面ID ;\param  control_id 控件ID    	*/
/*	入 口：  \param  frame_id 帧ID                                     			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void AnimationPlayFrame(u16 screen_id,u16 control_id,u8 frame_id)
{
    BEGIN_CMD();
    TX_8(0xB1);
    TX_8(0x23);
    TX_16(screen_id);
    TX_16(control_id);
    TX_8(frame_id);
    END_CMD();
}
/****************************************************************************************/
/*	功 能： \brief     播放制定帧   		*/
/*	注 释：	\param  screen_id 画面ID ;\param  control_id 控件ID    	*/
/*	入 口：  \param  frame_id 帧ID                                     			*/
/*  出 口：                                                                             */
/****************************************************************************************/
void SetTextFcolor(u16 screen_id, u16 control_id, u16 color)
{
	BEGIN_CMD();
    TX_8(0xB1);
    TX_8(0x19);
    TX_16(screen_id);
    TX_16(control_id);
    TX_16(color);
    END_CMD();
}
