/* ===========================================

	전체 포트 배선 사용 개요
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
volatile unsigned int t2_velocity, t2_create, t2_level;	// 타이머 인터럽트에서의 시간 flag들


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
int tmp1, tmp2;		// 속도의 시간에 대한 주기, 생성 시간에 대한 주기.
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
	
	// y번 만큼 com_port 값을 왼쪽 시프트함. 즉, y만큼 아래로 내려가서 출력.
	for(i=0; i<y; i++){
		if(com_port == 0){
			com_port = 1;
			continue;
		}
		com_port <<= 1;
	}
	COM_PORT = com_port;

	// type 이 0이면 두줄에 걸쳐 출력.
	if(type == 0)
		COM_PORT = com_port + (com_port*2);

	switch(type)
	{
	// type 이 0이면, 작게 출력.(2칸씩), 레일 1개를 잡아먹음
	case -1: case 0:		
		switch(rail)
		{
		case 1: color_port = 0b00000011; break;
		case 2:	color_port = 0b00011000; break;
		case 3: color_port = 0b11000000; break;
		}
	break;
	// type 이 1이면, 크게 출력.(3,4칸씩), 레일 1개를 잡아먹음
	case 1:
		switch(rail)
		{
		case 1:	color_port = 0b00000111; break;
		case 2:	color_port = 0b00111100; break;
		case 3:	color_port = 0b11100000; break;
		}
	break;
	// type 이 2이면, 매우 크게 표시 (6칸씩), 레일 2개를 잡아먹음
	case 2:
		switch(rail)
		{
		case 1:	color_port = 0b00111111; break;
		case 2: color_port = 0b11100111; break;
		case 3:	color_port = 0b11111100; break;
		}
	break;
	}

	switch(color)		// color 별 표시.
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
	delay(20);			// 안정화를 위한 delay -> 잔상을 없애는데 도움이 되었음.
}

int RAND(int a, int b)	// a와 b 사이의 임의의 숫자 반환.
{
	// 랜드함수에 대한 시드는 OVERFLOW0 루틴에서 처리하였음.
	rand_num = rand();
	return rand_num%(b-a+1)+a;
}

void CreateObstacle(void)	// obstacle을 생성해내는 함수.
{
	obstacle[num].life = TRUE;
	obstacle[num].y = 1;
	obstacle[num].rail = RAND(1,3);
	obstacle[num].type = RAND(0,2);

	num++;
	if(num>NUM_OBSTACLE-1)
		num = 0;
}

void KillObstacle(int num)	// 지정 obstacle을 죽이는 함수.
{
	obstacle[num].life = FALSE;
	obstacle[num].y = 0;
	obstacle[num].rail = 0;
	obstacle[num].type = -1;
}

SIGNAL(SIG_OVERFLOW0)		// 타이머/오버플로우 인터럽트 시그널 루틴.
{
	int i=0;
	
	TCNT0 = 112;			// 256-144 = 112 -> 0.01 초마다 인터럽트 발생.

	t2_velocity ++;			// 속도 계산 flag
	t2_create ++;			// 생성 주기 계산 flag
	t2_level ++;			// 레벨 상승 flag

	srand(rand_num ++);		// rand_num은 영원히 증가함 => 매순간 rand_num값은 달라져 있으므로 계속 변하는 시드값으로 적절.

	if(level>10){			// level이 10을 넘으면 더 이상 속도나 생성주기가 빨라지지 않는다.
		tmp1 = 20;
		tmp2 = 100;
	}
	else{
		tmp1 = 50-level*3;	// 속도의 주기를 나타내는 변수
		tmp2 = 250-level*15;// 생성의 주기를 나태내는 변수
	}

	if(t2_velocity >= tmp1)	// 속도
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
	if(t2_create >= tmp2)	// 생성주기
	{
		t2_create = 0;
		CreateObstacle();
	}
	if(t2_level == 1500)	// 15초마다 레벨 1씩 증가.
	{
		t2_level = 0;
		level ++;
	}
}

void Init_Timer(void)		// 시간 초기화 함수.
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

	int menu_pos = 1;				// 메뉴 커서 위치		- TextLCD
	int howto_pos = 0;				// 게임 방법 커서 위치	- TextLCD

	int state = 0;					// 메뉴 상태에 대한 flag
	int gameover=0;					// gameover에 대한 flag
	int pause = 0;					// pause에 대한 flag

	int saved_score=0;				// 저장할 점수.

	int key = 0, past_key = 0;		// key값을 받아 오기 위한 변수.
	
	// Dot Matrix 포트 초기화 
	COM_DDR		=	0xFF;
	COM_PORT		=	0;
	RED_DDR		=	0xFF;
	RED_PORT		=	0;
	GREEN_DDR	=	0xFF;
	GREEN_PORT	=	0;
	// Text LCD 포트 초기화
	DDRD = 0xFF;		DDRG = 0xFF;
	PORTD = 0x00;	PORTG = 0x00;
	// PORT F - Push Button
	PUSH_DDR = 0x00;

	InitLCD();		// Initializtion TextLCD
	Init_Timer();	// Initializtion Time/Overflow Interrupt
	
	sei();			// SET Global Interrupt Enable

	// 전원이 인가된 후, 프로그램이 시작하기 전, 제일 처음 Beef음을 3번 낸다.
	for(i=0; i<3; i++){
		TOUR_INITIALIZE();
		TOUR_PLAY(beef);
		Beef(50000);
		TOUR_STOP();
	}
	
// 프로그램 시작!
	while(1)
	{
		// 프로그램 시작 후의 초기화.
		state = 0;	// 상태 초기화.
		// Dot Matrix 초기화
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
			
			// 메뉴 디스플레이
			Locate(0,0);
			DispString(title);				// ... 첫줄에는 제목 출력.
			Locate(1,0);
			DispString(menu_str[menu_pos]);	// ... 두번째 줄에는 각 메뉴 문장 출력.
			
			key = PUSH_PIN;

			if((past_key == 0x00) && (key != 0x00))
			{
				switch(key)
				{
				case S2:		// left
					InitLCD();
					if(menu_pos > 1){
						menu_pos--;
						// Beef음 출력
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
						// Beef음 출력
						TOUR_INITIALIZE();
						TOUR_PLAY(beef);
						Beef(10000);
						TOUR_STOP();
					}
					break;

				case S3:		//select
					InitLCD();
					state = menu_pos;	// 현재 menu_pos의 메뉴로 진입.
					break;
				}
			}
			past_key = key;// key state update
		}// Menu Display End

/* ==================================== */
/*			1. Game Start				*/
/* ==================================== */

		/* 게임 시작 전 초기화 */
		InitLCD();		// TextLCD 초기화
		score = 0;		// 점수 초기화
		level = 1;		// 레벨 초기화
		// 시간 플래그들 초기화
		t2_velocity = 0;
		t2_create = 0;
		t2_level = 0;
		// Dot Matrix 초기화
		COM_PORT = 0x00;
		RED_PORT = 0x00;
		GREEN_PORT = 0x00;
		// OBSTACLE 전부 초기화
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
			DispInteger(0,6,level);				// ... Level 첫 번째 줄에 출력
			Locate(1,0);
			DispString("Score:");
			DispInteger(1,6,score);				// ... Score 두 번째 줄에 출력.

			// 자기 자신 출력.
			for(i=0; i<10; i++){
				DisplayInMatrix(myRail, 8, myColor, -1); 
			}

			// 장애물에 걸리면 gameover flag on.
			for(i=0; i<NUM_OBSTACLE; i++)
			{
				if(obstacle[i].life == TRUE)
				{
					switch(obstacle[i].type)
					{
					case 0:		// type 0-> 2x2 정사각형 점프 가능 장애물
						if(obstacle[i].rail == myRail && jump == 0 && (obstacle[i].y == 7 || obstacle[i].y==8))
							gameover = 1;
					break;
						
					case 1:		// type 1-> 1줄 짜리, 1 레일을 가로 막는 장애물
						if(obstacle[i].rail == myRail && obstacle[i].y == 8)
							gameover = 1;
					break;

					case 2:		// type 2-> 1줄 짜리, 2 레일을 가로 막는 장애물
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
					// 장애물들 Dot Matrix에 출력하기.
					// 10번을 반복출력하는 이유는 장애물의 표시를 조금 더 선명하게 하기 위함.
					for(j=0; j<10; j++)
						DisplayInMatrix(obstacle[i].rail, obstacle[i].y, enemyColor, obstacle[i].type);
				}
			}

			key = PUSH_PIN;

			/* 점프 구현부  */
			if(key != 0x00){
				// S2(3)를 누르는 동안, jump flag on하고, 나 자신을 오렌지색으로 출력. 
				if(key == S1){
					jump = 1;			
					myColor = ORANGE;
				}
			}
			else{// 아무 키도 안 누르고 있는 동안에는, jump flag off하고, 나 자신은 초록색으로 출력. (기본 설정)
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
						// Beef음 내기
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
						// Beef음.내기
						TOUR_INITIALIZE();
						TOUR_PLAY(beef);
						Beef(10000);
						TOUR_STOP();
					}
					break;

				case S3:		// select
					InitLCD();
					pause = 1;	// 게임 중에서 S7을 누르면 PAUSE 상태 진입.
					break;

				default:
					break;
				}
			}
			past_key = key;		// key state update			

			// PAUSE 상태 진입.
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

					// 지시문 출력.
					Locate(0,0);
					DispString("PAUSE");
					Locate(1,0);
					DispString("exit 3 restart 7");

					// PAUSE 상태에서는 ORANGE 색 전부 출력!
					COM_PORT=0xFF;
					RED_PORT=0xFF;
					GREEN_PORT = 0xFF;

					key = PUSH_PIN;

					if((past_key == 0x00) && (key != 0x00))
					{
						if(key == S3)	// S7을 누르면, 다시 재시작.(RESUME)
							pause = 0;
						if(key == S1)	// S3을 누르면, 메뉴 화면으로 돌아감.(게임 중도 포기)
						{
							pause = 0;
							state = 0;
						}
					}
					past_key = key;
				} // Pause 루틴의 while(1) 끝.
			} // Pause 루틴 끝.
		}// 본 게임에 대한 while(1) 끝.

		// Game Over 에 대한 루틴
		// GameOver flag가 on이면, Matrix를 붉게 반짝이면서 게임 오버 음 출력.
		if(gameover == 1)
		{
			InitLCD();
			saved_score=score;		// score는 계속해서 증가하고 있으므로 다른 변수에 저장.
			TOUR_INITIALIZE();
			TOUR_PLAY(song_fail);

			// Dot Matrix를 붉게 반짝이는 작업
			x = 500000;
			while(x-->0){	
				TOUR_UPDATE();	// 게임 오버 음 출력.
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
			TOUR_STOP(); // 다 반짝였으면 게임 오버음 종료.
		}

		while(1)
		{
			if(gameover == FALSE){
				InitLCD();
				break;
			}

			// 전부 RED 출력!
			COM_PORT = 0xFF;
			RED_PORT = 0xFF;
			
			// 점수 및 나가라는 표시 TextLCD에 출력.
			Locate(0,0);
			DispString("Your Score:");
			DispInteger(0,12,saved_score);		// ... 첫번째 줄에 저장될 점수 출력.

			Locate(1,0);
			DispString("Press 7 to exit");		// ... GAMEOVER 상태를 나가기 위한 지시문 출력.

			key = PUSH_PIN;

			if((past_key == 0x00) && (key == S3))	// S7을 누르면 GAMOVER flag off.
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

			// 두줄에 걸쳐 게임 instruction을 출력.
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
					state = 0;	// 메뉴 디스플레이 화면으로 돌아가라.
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
			DispInteger(0,12,saved_score);		// 첫번째 줄에 저장된 점수를 출력.
			Locate(1,0);
			DispString("Press 7 to exit");		// S7을 눌러 나가라는 지시문 출력.

			key = PUSH_PIN;

			if(key == S3){
				state = 0;						// 메뉴 디스플레이 화면으로 돌아가라.
			}
		}

/* ============================ */
/*			4. Exit				*/
/* ============================ */

		/* 무.한.루.프 */
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
