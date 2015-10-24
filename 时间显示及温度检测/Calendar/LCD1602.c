#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
/************************
*  LCD1602��������λ����
*************************/
sbit lcden=P2^7;  //lcd1602ѡ�ź�
sbit lcdrs=P2^6;  //lcd1602����/�����ź�
sbit lcdrw=P2^5;  //lcd1602��д�ź�
/******************************
*  �������ܣ������ʱ��Լ1ms��
* ��ڲ�����uint xms
*  ���ڲ�����
*******************************/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
/*********************************
*  �������ܣ���LCDдһ���ֽ�����
*  ��ڲ�����uchar command
*  ���ڲ�����
**********************************/
void lcd_writecommand(uchar command)
{
	lcdrs=0;
	lcdrw=0;
	lcden=0;
	P0=command;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}
/********************************
*  �������ܣ���LCDдһ���ֽ����� 
*  ��ڲ�����uchar dat
*  ���ڲ�����
*********************************/
void lcd_writedata(uchar dat)
{
	lcdrs=1;
	lcdrw=0;
	lcden=0;
	P0=dat;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}
/***************************
*  �������ܣ���LCD���г�ʼ�� 
*  ��ڲ�����
*  ���ڲ�����
****************************/
void lcd_initial()
{
//	delayms(15);
//	lcd_writecommand(0x38);
//	delayms(5);
//	lcd_writecommand(0x38);
//	delayms(5);
	lcd_writecommand(0x38);	  //��ʾģʽ���� 16x2��ʾ 5x7���� 8λ���ݽӿ�
	delayms(10);
	lcd_writecommand(0x08);	  //����ʾ
	delayms(10);
	lcd_writecommand(0x01);	  //��ʾ���� ������ָ�����㣬������ʾ���㣩
	delayms(10);
	lcd_writecommand(0x06);	  //дһ���ַ����ַָ���һ���ҹ���һ
	delayms(10);
	lcd_writecommand(0x0c);	  //����ʾ������ʾ��꣬����˸
}

	
		