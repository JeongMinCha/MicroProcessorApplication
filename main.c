/* ===========================================

	��ü ��Ʈ �輱 ��� ����
	2 Color Dot Matrix - PORTE / PORTA / PORTC
	TextLCD - PORTD / PORTG
	PushButton - PORTF
		PF0/PF1 - BT2/BT3, PF2/PF3 - BT6/BT7
	PIEZO - PB 5

 ============================================== */

/* ================================ */
/*			HEADER FILES			*/
/* ================================ */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "TOUR.h"
#include "ReservedText.h"
#include "TextLCD.h"

/* ==================================== */
/*			DEFINITION AREA				*/
/* ==================================== */
#define TRUE			1
#define FALSE			0

#define NUM_OBSTACLE	7

/* 2 Dot Color Matrix Definition */
#define COM_PORT		PORTE
#define COM_DDR			DDRE
#define RED_PORT		PORTA
#define RED_DDR			DDRA
#define GREEN_PORT		PORTC
#define GREEN_DDR		DDRC

/* Push Button - PORTF */
#define PUSH_DDR		DDRF
#define PUSH_PORT		PORTF
#define PUSH_PIN		PINF
#define S1				0x01
#define S2				0x02
#define S3				0x04
#define S4				0x08
#define S5				0x10
#define S6				0x20
#define S7				0x40
#define S8				0x80
#define PUSH_MASK		0b11001100

/* ==================================== */
/*			VARIABLE AREA				*/
/* ==================================== */
volatile unsigned int t2_velocity, t2_create, t2_level;	// Ÿ�̸� ���ͷ�Ʈ������ �ð� flag��


unsigned char beef[] = {	'T', 255,
							'O', 6, 'C', 64,
							0 };

unsigned char song_fail[] = {	'T', 255,
							'O', 6, 'I', 2, 'R', 2, 'E', 2, 'R', 2, 'E', 2, 'R', 2, 'E', 2, 'R', 2, 'O', 5, 'I', 2, 'R', 2, 'E', 2, 'R', 2, 'E', 2, 'R', 2, 'E', 2, 'R', 2,
							'O', 4, 'I', 2, 'R', 2, 'E', 2, 'R', 2, 'I', 2, 'R', 2, 'E', 2, 'R', 2, 'O', 3, 'G', 6, 'R', 2, 'O', 2, 'G', 6, 'R', 2,
							'M',
							'O', 3, 'C', 2, 'R', 2, 'O', 4, 'C', 2, 'R', 2, 'O', 3, 'C', 2, 'R', 2, 'O', 4, 'C', 2, 'R', 2, 'O', 3, 'E', 2, 'R', 2, 'O', 4, 'E', 2, 'R', 2, 'O', 3, 'E', 2, 'R', 2, 'O', 4, 'E', 2, 'R', 2, 

							'O', 3, 'F', 2, 'R', 2, 'O', 4, 'F', 2, 'R', 2, 'O', 3, 'F', 2, 'R', 2, 'G', 2, 'R', 10, 'O', 2, 'G', 6, 'R', 2,
							'O', 3, 'C', 2, 'R', 2, 'O', 4, 'C', 2, 'R', 2, 'O', 3, 'C', 2, 'R', 2, 'O', 4, 'C', 2, 'R', 2, 'O', 3, 'E', 2, 'R', 2, 'O', 4, 'E', 2, 'R', 2, 'O', 3, 'E', 2, 'R', 2, 'O', 4, 'E', 2, 'R', 2, 
							'O', 3, 'F', 2, 'R', 2, 'O', 4, 'F', 2, 'R', 2, 'O', 3, 'F', 2, 'R', 2, 'G', 2, 'R', 18,

							'O', 3, 'C', 2, 'R', 2, 'O', 4, 'C', 2, 'R', 2, 'O', 3, 'C', 2, 'R', 2, 'O', 4, 'C', 2, 'R', 2, 'O', 3, 'E', 2, 'R', 2, 'O', 4, 'E', 2, 'R', 2, 'O', 3, 'E', 2, 'R', 2, 'O', 4, 'E', 2, 'R', 2, 
							'O', 3, 'F', 2, 'R', 2, 'O', 4, 'F', 2, 'R', 2, 'O', 3, 'F', 2, 'R', 2, 'G', 2, 'R', 10, 'O', 2, 'G', 6, 'R', 2,

							'O', 3, 'A', 2, 'R', 10, 'D', 7, 'R', 1, 'D', 2, 'R', 2, 'D', 2, 'R', 2, 'D', 2, 'R', 2, 
							'G', 2, 'R', 10, 'C', 7, 'R', 1, 'C', 2, 'R', 2, 'C', 2, 'R', 2, 'C', 2, 'R', 2, 
							'F', 2, 'R', 10, 'D', 7, 'R', 1, 'D', 4, 'G', 3, 'R', 1, 'G', 4, 

							'C', 2, 'R', 10, 'O', 2, 'H', 7, 'R', 1, 'I', 2, 'R', 2,
							'C', 2, 'R', 14,
							'O', 3, 'F', 7, 'R', 1, 'O', 4, 'C', 4, 'F', 4, 'B', 4, 'O', 5, 'D', 4, 'C', 4, 'O', 4, 'A', 4,

							'O', 3, 'E', 7, 'R', 1, 'O', 4, 'C', 4, 'E', 4, 'A', 4, 'O', 5, 'C', 4, 'O', 4, 'B', 4, 'G', 4,
							'O', 3, 'D', 7, 'R', 1, 'A', 4, 'O', 4, 'D', 4, 'K', 4, 'B', 4, 'A', 4, 'F', 4,
							'O', 3, 'C', 7, 'R', 1, 'G', 4, 'O', 4, 'C', 4, 'O', 3, 'D', 7, 'R', 1, 'E', 4, 'R', 4,

							'F', 7, 'R', 1, 'O', 4, 'C', 4, 'F', 4, 'B', 4, 'O', 5, 'D', 4, 'C', 4, 'O', 4, 'A', 4,
							'O', 3, 'E', 7, 'R', 1, 'O', 4, 'C', 4, 'E', 4, 'A', 4, 'O', 5, 'C', 4, 'O', 4, 'B', 4, 'G', 4,
							'O', 3, 'D', 7, 'R', 1, 'A', 4, 'O', 4, 'D', 4, 'R', 16,
							
							'O', 3, 'G', 7, 'R', 9, 'O', 2, 'G', 4, 'R', 12,
							'O', 3, 'G', 7, 'R', 1, 'O', 2, 'G', 7, 'R', 1, 'G', 7, 'R', 1, 'B', 7, 'R', 1,
							0 };


int score = 0;
int tmp1, tmp2;		// �ӵ��� �ð��� ���� �ֱ�, ���� �ð��� ���� �ֱ�.
int rand_num = 0;
int level = 1;
int num = 0;
int myRail = 1;

int jump = FALSE;
enum{GREEN,RED,ORANGE};

int myColor = GREEN;
int enemyColor = RED;

// Obstacle Structure
typedef struct _obstacle
{
	int type;
	int rail;
	int y;
	int life;
} Obstacle;

Obstacle obstacle[NUM_OBSTACLE];

/* ======================================== */
/*			FUNCTION AND SIGNAL			    */
/* ======================================== */
void Beef(unsigned long x)
{
	while(x-->0){
		TOUR_UPDATE();
	}
}

void DisplayInMatrix(int rail, int y, int color, int type)
{
	int com_port=0;
	int color_port=0;
	int i=0;
	
	// y�� ��ŭ com_port ���� ���� ����Ʈ��. ��, y��ŭ �Ʒ��� �������� ���.
	for(i=0; i<y; i++){
		if(com_port == 0){
			com_port = 1;
			continue;
		}
		com_port <<= 1;
	}
	COM_PORT = com_port;

	// type �� 0�̸� ���ٿ� ���� ���.
	if(type == 0)
		COM_PORT = com_port + (com_port*2);

	switch(type)
	{
	// type �� 0�̸�, �۰� ���.(2ĭ��), ���� 1���� ��Ƹ���
	case -1: case 0:		
		switch(rail)
		{
		case 1: color_port = 0b00000011; break;
		case 2:	color_port = 0b00011000; break;
		case 3: color_port = 0b11000000; break;
		}
	break;
	// type �� 1�̸�, ũ�� ���.(3,4ĭ��), ���� 1���� ��Ƹ���
	case 1:
		switch(rail)
		{
		case 1:	color_port = 0b00000111; break;
		case 2:	color_port = 0b00111100; break;
		case 3:	color_port = 0b11100000; break;
		}
	break;
	// type �� 2�̸�, �ſ� ũ�� ǥ�� (6ĭ��), ���� 2���� ��Ƹ���
	case 2:
		switch(rail)
		{
		case 1:	color_port = 0b00111111; break;
		case 2: color_port = 0b11100111; break;
		case 3:	color_port = 0b11111100; break;
		}
	break;
	}

	switch(color)		// color �� ǥ��.
	{
	case GREEN:
		RED_PORT = 0;
		GREEN_PORT = color_port;
		break;
	case RED:
		RED_PORT = color_port;
		GREEN_PORT = 0;
		break;
	case ORANGE:
		RED_PORT = color_port;
		GREEN_PORT = color_port;
		break;
	}
	delay(20);			// ����ȭ�� ���� delay -> �ܻ��� ���ִµ� ������ �Ǿ���.
}

int RAND(int a, int b)	// a�� b ������ ������ ���� ��ȯ.
{
	// �����Լ��� ���� �õ�� OVERFLOW0 ��ƾ���� ó���Ͽ���.
	rand_num = rand();
	return rand_num%(b-a+1)+a;
}

void CreateObstacle(void)	// obstacle�� �����س��� �Լ�.
{
	obstacle[num].life = TRUE;
	obstacle[num].y = 1;
	obstacle[num].rail = RAND(1,3);
	obstacle[num].type = RAND(0,2);

	num++;
	if(num>NUM_OBSTACLE-1)
		num = 0;
}

void KillObstacle(int num)	// ���� obstacle�� ���̴� �Լ�.
{
	obstacle[num].life = FALSE;
	obstacle[num].y = 0;
	obstacle[num].rail = 0;
	obstacle[num].type = -1;
}

SIGNAL(SIG_OVERFLOW0)		// Ÿ�̸�/�����÷ο� ���ͷ�Ʈ �ñ׳� ��ƾ.
{
	int i=0;
	
	TCNT0 = 112;			// 256-144 = 112 -> 0.01 �ʸ��� ���ͷ�Ʈ �߻�.

	t2_velocity ++;			// �ӵ� ��� flag
	t2_create ++;			// ���� �ֱ� ��� flag
	t2_level ++;			// ���� ��� flag

	srand(rand_num ++);		// rand_num�� ������ ������ => �ż��� rand_num���� �޶��� �����Ƿ� ��� ���ϴ� �õ尪���� ����.

	if(level>10){			// level�� 10�� ������ �� �̻� �ӵ��� �����ֱⰡ �������� �ʴ´�.
		tmp1 = 20;
		tmp2 = 100;
	}
	else{
		tmp1 = 50-level*3;	// �ӵ��� �ֱ⸦ ��Ÿ���� ����
		tmp2 = 250-level*15;// ������ �ֱ⸦ ���³��� ����
	}

	if(t2_velocity >= tmp1)	// �ӵ�
	{
		t2_velocity = 0;
		score += level;
		for(i=0; i<NUM_OBSTACLE; i++)
		{
			if(obstacle[i].life==TRUE){
				obstacle[i].y ++;
			}
		
			if(obstacle[i].y > 8)
				KillObstacle(i);
		}
	}
	if(t2_create >= tmp2)	// �����ֱ�
	{
		t2_create = 0;
		CreateObstacle();
	}
	if(t2_level == 1500)	// 15�ʸ��� ���� 1�� ����.
	{
		t2_level = 0;
		level ++;
	}
}

void Init_Timer(void)		// �ð� �ʱ�ȭ �Լ�.
{
	TCCR0 = 0x07;
	TCNT0 = 112;
	TIMSK = 0x01;
}

/* ==================================== */
/*			MAIN FUNC AREA				*/
/* ==================================== */

int main(void)
{
	unsigned long x = 0;
	int i=0, j=0;

	int menu_pos = 1;				// �޴� Ŀ�� ��ġ		- TextLCD
	int howto_pos = 0;				// ���� ��� Ŀ�� ��ġ	- TextLCD

	int state = 0;					// �޴� ���¿� ���� flag
	int gameover=0;					// gameover�� ���� flag
	int pause = 0;					// pause�� ���� flag

	int saved_score=0;				// ������ ����.

	int key = 0, past_key = 0;		// key���� �޾� ���� ���� ����.
	
	// Dot Matrix ��Ʈ �ʱ�ȭ 
	COM_DDR		=	0xFF;
	COM_PORT		=	0;
	RED_DDR		=	0xFF;
	RED_PORT		=	0;
	GREEN_DDR	=	0xFF;
	GREEN_PORT	=	0;
	// Text LCD ��Ʈ �ʱ�ȭ
	DDRD = 0xFF;		DDRG = 0xFF;
	PORTD = 0x00;	PORTG = 0x00;
	// PORT F - Push Button
	PUSH_DDR = 0x00;

	InitLCD();		// Initializtion TextLCD
	Init_Timer();	// Initializtion Time/Overflow Interrupt
	
	sei();			// SET Global Interrupt Enable

	// ������ �ΰ��� ��, ���α׷��� �����ϱ� ��, ���� ó�� Beef���� 3�� ����.
	for(i=0; i<3; i++){
		TOUR_INITIALIZE();
		TOUR_PLAY(beef);
		Beef(50000);
		TOUR_STOP();
	}
	
// ���α׷� ����!
	while(1)
	{
		// ���α׷� ���� ���� �ʱ�ȭ.
		state = 0;	// ���� �ʱ�ȭ.
		// Dot Matrix �ʱ�ȭ
		COM_PORT = 0x00;
		RED_PORT = 0x00;
		GREEN_PORT = 0x00;

/* ==================================== */
/*			0. Menu Display				*/
/* ==================================== */

		while(1)
		{
			if (state != 0)
				break;
			
			// �޴� ���÷���
			Locate(0,0);
			DispString(title);				// ... ù�ٿ��� ���� ���.
			Locate(1,0);
			DispString(menu_str[menu_pos]);	// ... �ι�° �ٿ��� �� �޴� ���� ���.
			
			key = PUSH_PIN;

			if((past_key == 0x00) && (key != 0x00))
			{
				switch(key)
				{
				case S2:		// left
					InitLCD();
					if(menu_pos > 1){
						menu_pos--;
						// Beef�� ���
						TOUR_INITIALIZE();
						TOUR_PLAY(beef);
						Beef(10000);
						TOUR_STOP();
					}
					break;

				case S4:		//right
					InitLCD();
					if(menu_pos < MENU_POS_MAX-1){
						menu_pos++;
						// Beef�� ���
						TOUR_INITIALIZE();
						TOUR_PLAY(beef);
						Beef(10000);
						TOUR_STOP();
					}
					break;

				case S3:		//select
					InitLCD();
					state = menu_pos;	// ���� menu_pos�� �޴��� ����.
					break;
				}
			}
			past_key = key;// key state update
		}// Menu Display End

/* ==================================== */
/*			1. Game Start				*/
/* ==================================== */

		/* ���� ���� �� �ʱ�ȭ */
		InitLCD();		// TextLCD �ʱ�ȭ
		score = 0;		// ���� �ʱ�ȭ
		level = 1;		// ���� �ʱ�ȭ
		// �ð� �÷��׵� �ʱ�ȭ
		t2_velocity = 0;
		t2_create = 0;
		t2_level = 0;
		// Dot Matrix �ʱ�ȭ
		COM_PORT = 0x00;
		RED_PORT = 0x00;
		GREEN_PORT = 0x00;
		// OBSTACLE ���� �ʱ�ȭ
		for(i=0; i<NUM_OBSTACLE; i++)
			KillObstacle(i);

		while(1)
		{
			if(state != 1){
				InitLCD();
				break;
			}

			if(gameover != 0){
				InitLCD();
				break;
			}
			
			// Text LCD Display
			Locate(0,0);
			DispString("Level:");
			DispInteger(0,6,level);				// ... Level ù ��° �ٿ� ���
			Locate(1,0);
			DispString("Score:");
			DispInteger(1,6,score);				// ... Score �� ��° �ٿ� ���.

			// �ڱ� �ڽ� ���.
			for(i=0; i<10; i++){
				DisplayInMatrix(myRail, 8, myColor, -1); 
			}

			// ��ֹ��� �ɸ��� gameover flag on.
			for(i=0; i<NUM_OBSTACLE; i++)
			{
				if(obstacle[i].life == TRUE)
				{
					switch(obstacle[i].type)
					{
					case 0:		// type 0-> 2x2 ���簢�� ���� ���� ��ֹ�
						if(obstacle[i].rail == myRail && jump == 0 && (obstacle[i].y == 7 || obstacle[i].y==8))
							gameover = 1;
					break;
						
					case 1:		// type 1-> 1�� ¥��, 1 ������ ���� ���� ��ֹ�
						if(obstacle[i].rail == myRail && obstacle[i].y == 8)
							gameover = 1;
					break;

					case 2:		// type 2-> 1�� ¥��, 2 ������ ���� ���� ��ֹ�
						if(obstacle[i].y == 8)
						{
							switch(obstacle[i].rail)
							{
							case 1:
								if(myRail == 1 || myRail == 2)
									gameover = 1;
							break;
							case 2:
								if(myRail == 1 || myRail == 3)
									gameover = 1;
							break;
							case 3:
								if(myRail == 2 || myRail == 3)
									gameover = 1;
							break;
							}
						}
					break;
					}
					// ��ֹ��� Dot Matrix�� ����ϱ�.
					// 10���� �ݺ�����ϴ� ������ ��ֹ��� ǥ�ø� ���� �� �����ϰ� �ϱ� ����.
					for(j=0; j<10; j++)
						DisplayInMatrix(obstacle[i].rail, obstacle[i].y, enemyColor, obstacle[i].type);
				}
			}

			key = PUSH_PIN;

			/* ���� ������  */
			if(key != 0x00){
				// S2(3)�� ������ ����, jump flag on�ϰ�, �� �ڽ��� ������������ ���. 
				if(key == S1){
					jump = 1;			
					myColor = ORANGE;
				}
			}
			else{// �ƹ� Ű�� �� ������ �ִ� ���ȿ���, jump flag off�ϰ�, �� �ڽ��� �ʷϻ����� ���. (�⺻ ����)
				jump = 0;
				myColor = GREEN;
			}

			if((past_key == 0x00) && (key != 0x00))
			{
				switch(key)
				{
				case S2:		// left
					if(myRail > 1)
					{
						myRail --;
						// Beef�� ����
						TOUR_INITIALIZE();
						TOUR_PLAY(beef);
						Beef(10000);
						TOUR_STOP();
					}
					break;

				case S4:		// right
					if(myRail < 3)
					{
						myRail ++;
						// Beef��.����
						TOUR_INITIALIZE();
						TOUR_PLAY(beef);
						Beef(10000);
						TOUR_STOP();
					}
					break;

				case S3:		// select
					InitLCD();
					pause = 1;	// ���� �߿��� S7�� ������ PAUSE ���� ����.
					break;

				default:
					break;
				}
			}
			past_key = key;		// key state update			

			// PAUSE ���� ����.
			if(pause)
			{
				while(1)
				{
					TIMSK = 0;			// While Pause, timer/overflow interrupt should not be enabled.
					if(pause == 0)
					{
						TIMSK = 0x01;	// Out of Pause, Re-enable timer/overflow interrupt.
						InitLCD();
						break;
					}

					// ���ù� ���.
					Locate(0,0);
					DispString("PAUSE");
					Locate(1,0);
					DispString("exit 3 restart 7");

					// PAUSE ���¿����� ORANGE �� ���� ���!
					COM_PORT=0xFF;
					RED_PORT=0xFF;
					GREEN_PORT = 0xFF;

					key = PUSH_PIN;

					if((past_key == 0x00) && (key != 0x00))
					{
						if(key == S3)	// S7�� ������, �ٽ� �����.(RESUME)
							pause = 0;
						if(key == S1)	// S3�� ������, �޴� ȭ������ ���ư�.(���� �ߵ� ����)
						{
							pause = 0;
							state = 0;
						}
					}
					past_key = key;
				} // Pause ��ƾ�� while(1) ��.
			} // Pause ��ƾ ��.
		}// �� ���ӿ� ���� while(1) ��.

		// Game Over �� ���� ��ƾ
		// GameOver flag�� on�̸�, Matrix�� �Ӱ� ��¦�̸鼭 ���� ���� �� ���.
		if(gameover == 1)
		{
			InitLCD();
			saved_score=score;		// score�� ����ؼ� �����ϰ� �����Ƿ� �ٸ� ������ ����.
			TOUR_INITIALIZE();
			TOUR_PLAY(song_fail);

			// Dot Matrix�� �Ӱ� ��¦�̴� �۾�
			x = 500000;
			while(x-->0){	
				TOUR_UPDATE();	// ���� ���� �� ���.
				if(x<100000){
					COM_PORT = 0xFF;
					RED_PORT = 0xFF;
				}
				else if(x<200000){
					COM_PORT = 0x00;
					RED_PORT = 0x00;
					GREEN_PORT = 0x00;
				}
				else if(x<300000){
					COM_PORT = 0xFF;
					RED_PORT = 0xFF;
				}
				else if(x<400000){
					COM_PORT = 0x00;
					RED_PORT = 0x00;
					GREEN_PORT = 0x00;
				}
				else{
					COM_PORT = 0xFF;
					RED_PORT = 0xFF;
				}
			}
			TOUR_STOP(); // �� ��¦������ ���� ������ ����.
		}

		while(1)
		{
			if(gameover == FALSE){
				InitLCD();
				break;
			}

			// ���� RED ���!
			COM_PORT = 0xFF;
			RED_PORT = 0xFF;
			
			// ���� �� ������� ǥ�� TextLCD�� ���.
			Locate(0,0);
			DispString("Your Score:");
			DispInteger(0,12,saved_score);		// ... ù��° �ٿ� ����� ���� ���.

			Locate(1,0);
			DispString("Press 7 to exit");		// ... GAMEOVER ���¸� ������ ���� ���ù� ���.

			key = PUSH_PIN;

			if((past_key == 0x00) && (key == S3))	// S7�� ������ GAMOVER flag off.
				gameover=0;
			past_key = key;// key state update
		}

/* ==================================== */
/*			2. How to play				*/
/* ==================================== */

		InitLCD();
		while(1)
		{
			if(state != 2){
				InitLCD();
				break;
			}

			// ���ٿ� ���� ���� instruction�� ���.
			Locate(0,0);
			DispString(howto_str[howto_pos]);
			Locate(1,0);
			DispString(howto_str[howto_pos+1]);

			key = PUSH_PIN;

			if((past_key == 0x00) && (key != 0x00))
			{
				switch(key)
				{
				case S2:		// left
					if(howto_pos > 0)
					{
						InitLCD();
						howto_pos --;
					}
					break;

				case S4:		// right
					if(howto_pos < HOWTO_POS_MAX-2)
					{
						InitLCD();
						howto_pos ++;
					}
					break;

				case S3:		// Menu
					state = 0;	// �޴� ���÷��� ȭ������ ���ư���.
					break;
				}
			}
			past_key = key;// key state update	
		}

/* ================================ */
/*			3. Records				*/
/* ================================ */

		while(1)
		{
			if(state != 3){
				InitLCD();
				break;
			}

			Locate(0,0);
			DispString("Saved Score:");
			DispInteger(0,12,saved_score);		// ù��° �ٿ� ����� ������ ���.
			Locate(1,0);
			DispString("Press 7 to exit");		// S7�� ���� ������� ���ù� ���.

			key = PUSH_PIN;

			if(key == S3){
				state = 0;						// �޴� ���÷��� ȭ������ ���ư���.
			}
		}

/* ============================ */
/*			4. Exit				*/
/* ============================ */

		/* ��.��.��.�� */
		while(1)
		{
			if(state != 4){
				InitLCD();
				break;
			}
			Locate(0,0);
			DispString("Repower to start");
		}
	} // while(1) End.
} // int main(void) End.
