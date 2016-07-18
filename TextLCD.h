#ifndef __TEXT_LCD_H__
#define __TEXT_LCD_H__

/*
 *	TextLCD를 위한 포트 배선

	1) PB0~7 을 TextLCD 의 D0~D7 에 연결한다.

	2) PORTD는 다음과 같이 연결한다.

		PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
		*   *   *   E   *   *   RW  RS

 *  (!)나머지 배선하지 않은 PORT D의 포트들은 Push Button에 배선.
 */

#define LCD_CTRL		PORTG
#define LCD_DATA		PORTD

#define LCD_RS		0x01		// PD0
#define LCD_RW		0x02		// PD1
#define LCD_E		0x04		// PD4

#define TRUE			1
#define FALSE		0

/* ======================================== */
/*  FUNCTION PROTOTYPE - for TextLCD		*/
/* ======================================== */
void delay(unsigned long x);
void EnablePusle(void);
void InitLCD(void);
void sendLCDcommand(unsigned char command);
void sendLCDdata(unsigned char data);
void DispString(char * str);
void DispInteger(int x, int y, int num);
void Locate(int x, int y);

#endif
