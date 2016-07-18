#ifndef __TEXT_LCD_H__
#define __TEXT_LCD_H__

/*
 *	TextLCD�� ���� ��Ʈ �輱

	1) PB0~7 �� TextLCD �� D0~D7 �� �����Ѵ�.

	2) PORTD�� ������ ���� �����Ѵ�.

		PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
		*   *   *   E   *   *   RW  RS

 *  (!)������ �輱���� ���� PORT D�� ��Ʈ���� Push Button�� �輱.
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
