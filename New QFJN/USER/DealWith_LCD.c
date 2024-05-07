#include "MainWin.h"


#define TX_8(P1)    SEND_DATA((P1)&0xFF)                    //���͵����ֽ�
#define TX_8N(P,N)  SendNU8((uint8 *)P,N)                 //����N���ֽ�
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)                 //����16λ����
#define TX_16N(P,N) SendNU16((uint16 *)P,N)              //����N��16λ����
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)     //����32λ����

#define SEND_DATA(P) SendChar(P)          //����һ���ֽ�
#define BEGIN_CMD()  TX_8(0XEE)            //֡ͷ
#define END_CMD()    TX_32(0XFFFCFFFF)       //֡β

/****************************************************************************************/
/*	�� �ܣ� ���ָ��					*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
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
/*	�� �ܣ� �Ӷ�����ȡһ������		*/
/*	ע �ͣ�	            	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
static void queue_pop(u8* _data)
{
    if(que._tail!=que._head)                                          //�ǿ�״̬
    {
        *_data = que._data[que._tail];
        que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
    }
}
/****************************************************************************************/
/*	�� �ܣ� ��ȡ��������Ч���ݸ���	*/
/*	ע �ͣ�	            	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
static u16 queue_size()
{
    return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}
/****************************************************************************************/
/*	�� �ܣ� ���ָ��					*/
/*	ע �ͣ�	������ֻ��һ���ֽ�û�б��ݣ�Ҳû��У��             	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/
void queue_reset()
{
    que._head = que._tail = 0;
    cmd_pos = cmd_state = 0;
}

/****************************************************************************************/
/*	�� �ܣ� ���ָ������ 						*/
/*	ע �ͣ�	cmd ָ����ջ�����  ;buf_len ָ����ջ�������С ; buf_len ָ����ջ�������С   	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ� ָ��ȣ�0��ʾ������������ָ��                                                                            */
/****************************************************************************************/
void queue_push(u8 _data)
{
    u16 pos = (que._head+1)%QUEUE_MAX_SIZE;
    if(pos!=que._tail)                                                //����״̬
    {
        que._data[que._head] = _data;
        que._head = pos;
    }
}
/****************************************************************************************/
/*	�� �ܣ� ��ָ�������ȡ��һ��������ָ��						*/
/*	ע �ͣ�	���ڽ��յ����ݣ�ͨ���˺�������ָ�����  �� _data ָ������        	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
/****************************************************************************************/

u16 queue_find_cmd(u8 *buffer,u16 buf_len)
{
    u16 cmd_size = 0;
    u8 _data = 0;

    while(queue_size()>0)
    {
        //ȡһ������
        queue_pop(&_data);

        if(cmd_pos==0&&_data!=CMD_HEAD)                               //ָ���һ���ֽڱ�����֡ͷ����������
        {
            continue;
        }
        //    LED2_ON;
        if(cmd_pos<buf_len)                                           //��ֹ���������
            buffer[cmd_pos++] = _data;

        cmd_state = ((cmd_state<<8)|_data);                           //ƴ�����4���ֽڣ����һ��32λ����

        //���4���ֽ���֡βƥ�䣬�õ�����֡
        if(cmd_state==CMD_TAIL)
        {
            //LED2_ON;
            cmd_size = cmd_pos;                                       //ָ���ֽڳ���
            cmd_state = 0;                                            //���¼��֡β��
            cmd_pos = 0;                                              //��λָ��ָ��
						uLCD_Key = 0;
#if(CRC16_ENABLE)
            //ȥ��ָ��ͷβEE��βFFFCFFFF����5���ֽڣ�ֻ�������ݲ���CRC
            if(!CheckCRC16(buffer+1,cmd_size-5))                      //CRCУ��
                return 0;

            cmd_size -= 2;                                            //ȥ��CRC16��2�ֽڣ�
#endif
            return cmd_size;
        }
    }
    return 0;                                                         //û���γ�������һ֡
}

/****************************************************************************************/
/*	�� �ܣ����ڷ������ַ���					*/
/*	ע �ͣ�	�ַ���    	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
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
/*	�� �ܣ� ��ָ�������ȡ��һ��������ָ��						*/
/*	ע �ͣ�	���ڽ��յ����ݣ�ͨ���˺�������ָ�����  �� _data ָ������        	*/
/*	�� �ڣ�                                           			*/
/*  �� �ڣ�                                                                             */
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
/*	�� �ܣ� �����ı�Ϊ����ֵ��Ҫ��FIRMWARE_VER>=908						*/
/*	ע �ͣ�	param  screen_id ����ID;param  control_id �ؼ�ID ;value �ı���ֵ; sign 0-�޷��ţ�1-�з���       	*/
/*	�� �ڣ� fill_zero ����λ��������ʱ��ಹ��                                          			*/
/*  �� �ڣ�                                                                             */
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
/*	�� �ܣ� �����ı������ȸ���ֵ��Ҫ��FIRMWARE_VER>=908   		*/
/*	ע �ͣ�	screen_id ����ID;param   control_id �ؼ�ID ;value �ı���ֵ; sign 0-�޷��ţ�1-�з���       	*/
/*	�� �ڣ� fill_zero ����λ��������ʱ��ಹ��                                          			*/
/*  �� �ڣ�                                                                             */
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
	 //��Ҫ���ִ�С��
		TX_8(((u8 *)&value)[3-i]);
	}
	END_CMD();
}
/*! 
*  \brief     �����ƶ�֡
*  \param  screen_id ����ID
*  \param  control_id �ؼ�ID
*  \param  frame_id ֡ID
*/

/*! 
*  \brief  �Զ����ڱ�������
*  \param  enable ʹ��
*  \param  bl_off_level ��������
*  \param  bl_on_level  ��������
*  \param  bl_on_time  ƫ��������
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
/*	�� �ܣ� \brief     �����ƶ�֡   		*/
/*	ע �ͣ�	\param  screen_id ����ID ;\param  control_id �ؼ�ID    	*/
/*	�� �ڣ�  \param  frame_id ֡ID                                     			*/
/*  �� �ڣ�                                                                             */
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
/*	�� �ܣ� \brief     �����ƶ�֡   		*/
/*	ע �ͣ�	\param  screen_id ����ID ;\param  control_id �ؼ�ID    	*/
/*	�� �ڣ�  \param  frame_id ֡ID                                     			*/
/*  �� �ڣ�                                                                             */
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
