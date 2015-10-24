#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit beep=P1^5;
uchar ds1302_display[]="20  -  -    :  :  ";  //��¼������ ʱ������Ϣ
uchar ds1302_day[][3]={{'M','O','N'},{'T','U','E'},{'W','E','N'},{'T','H','U'},{'F','R','I'},{'S','A','T'},{'S','U','N'}};	  //���δ�����һ��������
uchar ds18b20_temp_tip[]="Temp has exceed!";
/******************************
*  �������ܣ������ʱ��Լ1ms��
*  ��ڲ�����uint xms
*  ���ڲ�����
*******************************/
static void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
/*************************************
*  �������ܣ���ʮ������ת����ASCII��
*  ��ڲ�����uchar decimal
*  ���ڲ�����
*************************************/
uchar decimal_character(uchar decimal)
{
	return(decimal+48);
}	
int main()
{	
/***********�����ⲿ������������������****************/
	void lcd_initial();
	uchar bcd_decimal(uchar bcd);
	void lcd_writecommand(uchar command);
	void lcd_writedata(uchar dat);
	void ds1302_initial();
	uchar ds1302_read(uchar address);
	void ds1302_write(uchar address,uchar dat);
	void ds18b20_convert();
	uint ds18b20_deal();
//	void ds18b20_set_alarm();
//	void ds18b20_read_rom();
//	void ds18b20_search_alarm();
	void keyone_scan();
	void keyfive_scan();
	void set_cursor();
	void set_alarm();
	void ds1820_readalarm();
	extern uchar code ds1302_write_ckaddress[];
	extern uchar code ds1302_read_ckaddress[];
	extern bit keyone_flag;
	extern bit keyfive_flag;
	extern uchar a;
	extern uchar b;
/***********�����ⲿ������������������****************/
	uchar k,m,n;
	lcd_initial();
	ds1302_initial();

	while(1)
	{	
			keyone_scan();
			keyfive_scan();
//			keytwo_scan();
			if(keyone_flag==1)
			{
				ds1302_write(ds1302_write_ckaddress[0],0x80|ds1302_read(ds1302_read_ckaddress[0]));	 //�����⵽K1�������£�����Second�Ĵ��������λд1������standby����ģʽ
				set_cursor();
				keyone_flag=0;
				ds1302_write(ds1302_write_ckaddress[0],0x7f&ds1302_read(ds1302_read_ckaddress[0]));	//��Second�Ĵ��������λд0������start����ģʽ
			}
			if(keyfive_flag==1)
			{
				set_alarm();
				lcd_initial();
				keyfive_flag=0;  
			}
				
/********************��DS1302�ж�����ǰ��ʱ�����ݲ�ת����ASCII��洢��������******************************/
			ds1302_display[17]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))%10);  //��
			ds1302_display[16]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[0]))/10);
			ds1302_display[14]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))%10);  //��
			ds1302_display[13]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[1]))/10);
			ds1302_display[11]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))%10);  //ʱ
			ds1302_display[10]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[2]))/10);
			ds1302_display[9]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))%10);	  //��
			ds1302_display[8]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[3]))/10);
			ds1302_display[6]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))%10);	  //��
			ds1302_display[5]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[4]))/10);						
			ds1302_display[3]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))%10);	  //��
			ds1302_display[2]=decimal_character(bcd_decimal(ds1302_read(ds1302_read_ckaddress[6]))/10);
/*********************��DS1302�ж�����ǰ��ʱ�����ݲ�ת����ASCII��洢��������******************************/

/************����clock/calendar�Ĵ����ж�����ʱ����������д��LCD1602***********************/
		lcd_writecommand(0x80);	 
		for(k=0;k<10;k++)
		{
			lcd_writedata(ds1302_display[k]);
		}
		lcd_writecommand(0x80+0x40);
		for(m=10;m<18;m++)
		{
			lcd_writedata(ds1302_display[m]);
		}
/************����clock/calendar�Ĵ����ж�����ʱ����������д��LCD1602***********************/

        lcd_writecommand(0x80+0x0c);
		for(n=0;n<3;n++)
		{
			lcd_writedata(ds1302_day[bcd_decimal(ds1302_read(ds1302_read_ckaddress[5]))-1][n]);	//��day��Ϣд��LCD1602 
		}
		
/************����ds18b20�ж�������������д��LCD1602***************/
		ds18b20_convert();			 //ת����Ϻ�ds18b20������idle state,������Ҫ��������
		
		lcd_writecommand(0x80+0x4c);
		lcd_writedata(decimal_character(ds18b20_deal()/100));
		lcd_writedata(decimal_character(ds18b20_deal()/10%10));
		lcd_writedata(46);
		lcd_writedata(decimal_character(ds18b20_deal()%10));
/************����ds18b20�ж�������������д��LCD1602***************/
	
/*************DS13B20����¶��Ƿ񳬹��趨ֵ*********************/
		ds1820_readalarm();
		if((ds18b20_deal())>(a*10)||(ds18b20_deal())<(b*10))
		{
			uchar i;
//			delayms(30);
			lcd_writecommand(0x01);	 //����
			lcd_writecommand(0x80);
			for(i=0;i<16;i++)
			{
				lcd_writedata(ds18b20_temp_tip[i]);
			}
			lcd_writecommand(0x80+0x46); 
			lcd_writedata(decimal_character(ds18b20_deal()/100));
			lcd_writedata(decimal_character(ds18b20_deal()/10%10));
			beep=~beep;
			lcd_writedata(46);
			lcd_writedata(decimal_character(ds18b20_deal()%10));
			delayms(5000);
			lcd_writecommand(0x01);;	 //����
		}
	}
	return 0;
}


			
				
			
			
		
			
		
		