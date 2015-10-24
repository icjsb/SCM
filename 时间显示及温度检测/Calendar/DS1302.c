#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
/************************
*  DS1302��������λ����
*************************/
sbit ds1302data=P3^4; //ds1302����ͨ����
sbit ds1302rst=P3^5;  //ds1302��λ�ź�
sbit ds1302sclk=P3^6;  //ds1302ͬ��ʱ���ź�
/*********************************************
   ���� DS1302 clock/calendar �Ĵ�����ʼ��ֵ
   ����д DS1302 clock/calendar �Ĵ����ĵ�ַ
**********************************************/
uchar code ds1302_initialcalendar[]={0,0,12,11,10,4,12}; //2012��10��8������һ12��00
uchar code ds1302_write_ckaddress[]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //дclock/calendar��ַ
uchar code ds1302_read_ckaddress[]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  //��clock/calendar��ַ
/*************************************************
*  �������ܣ���ds1302дһ���ֽ����ݣ���ַ�����ݣ�
*  ��ڲ�����uchar content
*  ���ڲ�����
**************************************************/
void ds1302_writebyte(uchar content) 
{
	uchar i;
	ds1302sclk=0;
	_nop_();
	ds1302rst=1;
	for(i=0;i<8;i++)
	{		
		ds1302sclk=0;
		_nop_();
		ds1302data=content&0x01;
		_nop_();
		ds1302sclk=1;		//ÿ��д��󱣳�ds1302sclkΪ�ߵ�ƽ״̬
		content>>=1;
	}
}
/************************************************************
*  �������ܣ���ds1302��дһ���ֽڵ�ַ��Ȼ����дһ���ֽ�����
*  ��ڲ�����uchar address,uchar dat
*  ���ڲ�����
*************************************************************/
void ds1302_write(uchar address,uchar dat)
{
	ds1302rst=0;
	ds1302_writebyte(address);
	ds1302_writebyte(dat);
	ds1302rst=0;
}
/*************************************
*  �������ܣ���ds1302����һ���ֽ�����
*  ��ڲ�����
*  ���ڲ�����uchar dat
**************************************/
uchar ds1302_readbyte()
{
	uchar i,dat=0;
	for(i=0;i<8;i++)
	{
		ds1302sclk=0;	
	 	dat>>=1;
		if(ds1302data)
			dat|=0x80;
		ds1302sclk=1;
		_nop_();
	}
	return dat;
}
/**********************************
*  �������ܣ���ds1302����һ���ֽ�
*  ��ڲ�����uchar address
*  ���ڲ�����uchar dat
***********************************/
uchar ds1302_read(uchar address)
{
	
	uchar dat;
	ds1302rst=0;
	ds1302_writebyte(address);
	dat=ds1302_readbyte();
	ds1302rst=0;
	return dat;
}
/***********************************
*  �������ܣ���ʮ������ת����BCD��
*  ��ڲ�����uchar decimal
*  ���ڲ�����
************************************/
uchar decimal_bcd(uchar decimal)
{
	return((decimal/10)*16+(decimal%10)); //����������ȼ�������
}
/*******************************************
*  �������ܣ���BCD��ʮ������ת����ʮ������
*  ��ڲ�����uchar bcd
*  ���ڲ�����
*******************************************/
uchar bcd_decimal(uchar bcd)		 ////����������ȼ�������
{
	return((bcd/16)*10+(bcd&0x0f));

}
/********************************
*  �������ܣ���ds1302���г�ʼ��
*  ��ڲ�����
*  ���ڲ�����
********************************/
void ds1302_initial()
{
	uchar i;
	ds1302_write(0x8e,0x00); //дControl�Ĵ�����ַ/�����ֽ�,��Control�Ĵ��������λwp����														  
	for(i=0;i<7;i++)
	{
		ds1302_write(ds1302_write_ckaddress[i],decimal_bcd(ds1302_initialcalendar[i]));
		_nop_();
	}
}
	
	
	
	
	

	




