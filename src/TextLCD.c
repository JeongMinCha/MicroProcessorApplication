#include <avr/io.h>
#include <stdlib.h>
#include "TextLCD.h"

void delay(unsigned long x)
{
	while(x--);
}

void EnablePulse(void)
{
	// ------
	// �޽� �����
	// ------
	LCD_CTRL |= LCD_E;			// E = 1�� �����. �ʱⰪ(���� ��)�� 0�̾���� �Ѵ�.
	asm("NOP");					// �ƹ��͵� ���� �ʴ� ����� ��ɾ�.
	// 1����Ŭ�� ���� ª�� �ð� ������ �ش�.
	LCD_CTRL &= ~LCD_E;			// �ٽ� E = 0���� �����.
}

void sendLCDcommand(unsigned char command)
{
	LCD_CTRL &= ~LCD_RS;		// ����� ������ �� RS = 0
	LCD_DATA = command;			// ������ ��� code ���� data bus�� �Ǿ� ���´�.
	EnablePulse();				// enable pulse �� ��������. �� �������� ����� ���۵ȴ�.
	delay(20);					// ���۵� ����� LCD�� �޾Ƶ鿩 ó���ϴµ� �ɸ��� �ð��� ����.
}

void InitLCD(void)
{
	delay(20000);
	LCD_CTRL &= ~LCD_RW;		// LCD�� ����̳� �ڷḦ ������ �� �׻� RW=0
	sendLCDcommand(0x38);		// function ���� 8bit data�� �� ���� ó��.
	// 2 line, 5*7 dot.
	sendLCDcommand(0x0C);		// Display�� ON ��Ų��. �̰� ���ϸ� ���� ǥ�� �ȵ�.
	sendLCDcommand(0x01);		// Clear Screen
	delay(1000);				// 1.53 ms �̻� �Ǿ�� ��. -> ���� 2 ms �̻����� ����.
	// Clear Screen ����� ����ð��� ��� ������ ����� ��ٷ��� ��.
}

// x,y��ǥ�� num ���ڸ� 4�ڸ����� ���缭 ���.
void DispInteger(int x, int y, int num)
{
	unsigned char d3, d2, d1, d0;

	d3 = num/1000;
	num = num%1000;
	d2 = num/100;
	num = num%100;
	d1 = num/10;
	d0 = num%10;

	if(d3 == 0 && d2 == 0 && d1 == 0 && d0 == 0)
		InitLCD();	

	Locate(x,y);
	sendLCDdata(d3+0x30);
	Locate(x,y+1);
	sendLCDdata(d2+0x30);
	Locate(x,y+2);
	sendLCDdata(d1+0x30);
	Locate(x,y+3);
	sendLCDdata(d0+0x30);
	
}

void sendLCDdata(unsigned char data)
{
	LCD_CTRL |= LCD_RS;		// data�� ������ ���� RS=1 �����.
	LCD_DATA = data;			// ������ ���� code���� data bus�� �Ǿ� ���´�.
	EnablePulse();			// enable pulse �� ��������. �� �������� ����� ���۵ȴ�.
	delay(20);				// data ���� �� LCD�� ó���ϴ� �� �ð��� �ɸ���.
}

void DispString(char * str)
{
	while(*str)
	{
		sendLCDdata(*str);
		str ++;
	}
}

void Locate(int x, int y)
{
	unsigned char RamAddr;
		
	if(x==0)
		RamAddr = 0x80 + y;	// line1 D.D.RAM address
	else
		RamAddr = 0xC0 + y; // line2 D.D.RAM address

	sendLCDcommand(RamAddr);
}
