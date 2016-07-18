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
	// 펄스 만들기
	// ------
	LCD_CTRL |= LCD_E;			// E = 1로 만든다. 초기값(이전 값)은 0이었어야 한다.
	asm("NOP");					// 아무것도 하지 않는 어셈블리 명령어.
	// 1싸이클의 아주 짧은 시간 지연을 준다.
	LCD_CTRL &= ~LCD_E;			// 다시 E = 0으로 만든다.
}

void sendLCDcommand(unsigned char command)
{
	LCD_CTRL &= ~LCD_RS;		// 명령을 전달할 때 RS = 0
	LCD_DATA = command;			// 전송할 명령 code 값을 data bus에 실어 놓는다.
	EnablePulse();				// enable pulse 를 내보낸다. 이 시점에서 명령이 전송된다.
	delay(20);					// 전송된 명령을 LCD가 받아들여 처리하는데 걸리는 시간이 있음.
}

void InitLCD(void)
{
	delay(20000);
	LCD_CTRL &= ~LCD_RW;		// LCD에 명령이나 자료를 전송할 때 항상 RW=0
	sendLCDcommand(0x38);		// function 설정 8bit data를 한 번에 처리.
	// 2 line, 5*7 dot.
	sendLCDcommand(0x0C);		// Display를 ON 시킨다. 이걸 안하면 글자 표시 안됨.
	sendLCDcommand(0x01);		// Clear Screen
	delay(1000);				// 1.53 ms 이상 되어야 함. -> 계산상 2 ms 이상으로 예측.
	// Clear Screen 명령은 실행시간이 길기 때문에 충분히 기다려야 함.
}

// x,y좌표에 num 숫자를 4자리수에 맞춰서 출력.
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
	LCD_CTRL |= LCD_RS;		// data를 보내기 위해 RS=1 만든다.
	LCD_DATA = data;			// 전송할 문자 code값을 data bus에 실어 놓는다.
	EnablePulse();			// enable pulse 를 내보낸다. 이 시점에서 명령이 전송된다.
	delay(20);				// data 전송 후 LCD가 처리하는 데 시간이 걸린다.
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
