/*
	TOUR 라이브러리 사용 방법:

	0. 본문 맨 위의 'TOUR 설정 부분'을 찾은 다음
	   각 상수값에 대한 설명을 잘 읽고
	   자신이 사용할 환경에 맞는 적절한 값으로 변경하세요.
	   
	   - 주의: Piezo는 AVR 내장 카운터 중 를 사용하므로 정해진 특정 핀(PB5)에만 꽂을 수 있습니다.

	1. 소리를 낼 '악보'에 해당하는 unsigned char 배열을 선언하고
	   각 명령어를 작성하여 배열 초기식을 채우세요.

	2. 자신의 main() 안에 있는 메인 while문을 시작하기 전 부분(DDRA, srand() 이런거 적는 부분)에
	   TOUR_INITIALIZE() 매크로 함수 호출 구문을 추가하세요.

	3. 자신의 main() 안에 있는 메인 while문에
	   TOUR_UPDATE() 매크로 함수 호출 구문을 추가하세요.

   	4. 연주를 시작 / 재개하고 싶은 코드 위치에
	   TOUR_PLAY() 매크로 함수 호출 구문을 추가하세요.
*/

#pragma once

/*
	----------------------------
	TOUR 설정 부분 시작
	----------------------------
*/

/* 초기 BPM값. 범위는 [50, 305] (이 값을 직접 바꿀 땐 T 명령어의 -50 규칙을 쓰지 않은 원래 BPM값을 사용해야 됨) */
#define TOUR_DEFAULT_BPM 60

/* 초기 옥타브값. 범위는 [0, 255] */
#define TOUR_DEFAULT_OCTAVE 5

/* '5옥타브 라 음(880Hz)'을 내는 pulse의 너비 (성능상의 문제로 4옥타브 대신 5옥타브 사용, 이 값은 바꿀 일이 거의 없음) */
#define TOUR_WIDTH_BASIC 	4197UL		/* 주의: 이 상수값 끝에는 반드시 UL 또는 L을 붙여 둬야 함! */

/* 1BPM(1분 안에 4분음표 1개 포함)에서 64분음표 하나의 길이만큼(3.75초) 음을 내기 위해 몇 번 루프를 돌아야 하는지 (이 값은 거의 항상 바꿔야 함) */
#define TOUR_TICKS_BASIC	1300000UL	/* 주의: 이 상수값 끝에는 반드시 UL 또는 L을 붙여 둬야 함! */

/*
	----------------------------
	TOUR 설정 부분 끝. 이제 맨 아래에 있는 매크로 함수를 보고 그거만 쓰면 됨.
	----------------------------
*/

/* 64분음표 하나의 길이에 해당하는 tick값 */
extern unsigned long TOUR_current_ticks_per_64th_note;

/* TOUR 프레임을 굴리기 위한 tick값 */
extern unsigned long TOUR_tick;

/* 현재 설정된 옥타브값 */
extern unsigned char TOUR_current_octave;

/* 현재 연주중인 음이 얼마나 남았는지 - 1인 경우 다음 프레임에 연주 종료 */
extern unsigned char TOUR_remained_duration;

/* 현재 연주중인 음의 pulse 너비 */
extern unsigned int TOUR_current_width;

/* Uwaaang 연주의 시작음 pulse 너비 */
extern unsigned int TOUR_uwaaang_width_start;

/* Uwaaang 연주의 끝음 pulse 너비 */
extern unsigned int TOUR_uwaaang_width_end;

/* Uwaaang 연주의 전체 길이 - 0인 경우 Uwaaang 연주를 하고 있지 않음을 의미, Uwaaang 연주를 수행하는 마지막 프레임에서 이 값을 0으로 초기화해 줌 */
extern unsigned int TOUR_uwaaang_duration;

/* 현재 연주중인 악보 배열의 시작 위치(또는 M 명령어로 마크한 위치) */
extern unsigned char *TOUR_current_score;

/* 악보 배열 내의 이번에 연주할 위치 */
extern unsigned char *TOUR_current_data;

/* Piezo 출력을 위한 trigger 변수 */
extern unsigned char TOUR_enabled;


/* 주어진 bpm에서 64분음표 하나의 길이에 해당하는 tick값을 계산하기 위한 매크로 함수 */
#define TOUR_GET_TICKS_PER_64TH_NOTE_FROM_BPM(bpm)	( TOUR_TICKS_BASIC / ( bpm ) )


/* 4옥타브 음들에 대한 pulse의 너비 */
#define TOUR_WIDTH_REST		0UL
#define TOUR_WIDTH_C		(TOUR_WIDTH_BASIC * 100 / 119)
#define TOUR_WIDTH_C_SHARP	(TOUR_WIDTH_BASIC * 100 / 126)
#define TOUR_WIDTH_D_FLAT	(TOUR_WIDTH_BASIC * 100 / 126)
#define TOUR_WIDTH_D		(TOUR_WIDTH_BASIC * 100 / 133)
#define TOUR_WIDTH_D_SHARP	(TOUR_WIDTH_BASIC * 100 / 141)
#define TOUR_WIDTH_E_FLAT	(TOUR_WIDTH_BASIC * 100 / 141)
#define TOUR_WIDTH_E		(TOUR_WIDTH_BASIC * 100 / 150)
#define TOUR_WIDTH_F		(TOUR_WIDTH_BASIC * 100 / 159)
#define TOUR_WIDTH_F_SHARP	(TOUR_WIDTH_BASIC * 100 / 168)
#define TOUR_WIDTH_G_FLAT	(TOUR_WIDTH_BASIC * 100 / 168)
#define TOUR_WIDTH_G		(TOUR_WIDTH_BASIC * 100 / 178)
#define TOUR_WIDTH_G_SHARP	(TOUR_WIDTH_BASIC * 100 / 189)
#define TOUR_WIDTH_A_FLAT	(TOUR_WIDTH_BASIC * 100 / 189)
#define TOUR_WIDTH_A		(TOUR_WIDTH_BASIC * 100 / 200)
#define TOUR_WIDTH_A_SHARP	(TOUR_WIDTH_BASIC * 100 / 212)
#define TOUR_WIDTH_B_FLAT	(TOUR_WIDTH_BASIC * 100 / 212)
#define TOUR_WIDTH_B		(TOUR_WIDTH_BASIC * 100 / 224)

/* 주어진 옥타브의 특정 음에 대한 너비 상수를 계산하기 위한 매크로 함수 */
#define TOUR_WIDTH(name, oct)	( oct > 4 ? TOUR_WIDTH_##name >> ( oct - 4 ) : TOUR_WIDTH_##name << ( 4 - oct ) )


/*
	----------------------------
	TOUR 라이브러리 함수 정의 시작. 이 아래에 있는 네 함수만 잘 보고 사용하면 됨.
	----------------------------
*/

/*
	TOUR_INITIALIZE()

	포트 설정 및 Timer1 초기화를 수행합니다.
	다른 초기화 코드들과 함께 메인 while문이 시작되기 전 부분에 이 함수를 호출하는 구문을 추가하세요.
*/
#define TOUR_INITIALIZE() ( DDRB |= 0b00100000, TCCR1A = 0b01000000, TCCR1B = 0b00001010 )

/*
	TOUR_PLAY()

	주어진 악보 배열의 내용을 바탕으로 다음 TOUR 프레임부터 Piezo 연주를 시작합니다.
	연주를 시작하거나 재개하고 싶은 부분에 이 함수를 호출하는 구문을 추가하세요.
	물론 초기화 코드 바로 아래에 적어 두어도 됩니다.
*/
#define TOUR_PLAY(score) ( TOUR_current_score = ( score ), TOUR_current_data = ( score ), TOUR_remained_duration = 1, TOUR_enabled = 1 )

/*
	TOUR_STOP()

	Piezo 연주를 중단합니다.
	연주를 다시 시작하려면 TOUR_PLAY()를 또 호출하세요.
*/
#define TOUR_STOP() ( OCR1A = 0, TOUR_enabled = 0 )

/*
	TOUR_UPDATE()

	Piezo 연주를 위한 TOUR 프레임 코드가 들어 있는 함수입니다.
	메인 while문 안에 이 함수를 호출하는 구문을 딱 한 번만 추가하세요.
*/
#define TOUR_UPDATE()																			\
		do																						\
		{																						\
			if ( TOUR_enabled )																	\
			{																					\
				if ( TOUR_tick >= TOUR_current_ticks_per_64th_note )							\
				{																				\
					--TOUR_remained_duration;													\
																								\
					if ( TOUR_remained_duration == 0 )											\
					{																			\
						while ( TOUR_remained_duration == 0 )									\
						{																		\
							switch ( *TOUR_current_data )										\
							{																	\
							case 'C':															\
								TOUR_current_width = TOUR_WIDTH(C, TOUR_current_octave);		\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'D':															\
								TOUR_current_width = TOUR_WIDTH(D, TOUR_current_octave);		\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'E':															\
								TOUR_current_width = TOUR_WIDTH(E, TOUR_current_octave);		\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'F':															\
								TOUR_current_width = TOUR_WIDTH(F, TOUR_current_octave);		\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'G':															\
								TOUR_current_width = TOUR_WIDTH(G, TOUR_current_octave);		\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'A':															\
								TOUR_current_width = TOUR_WIDTH(A, TOUR_current_octave);		\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'B':															\
								TOUR_current_width = TOUR_WIDTH(B, TOUR_current_octave);		\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'H':															\
								TOUR_current_width = TOUR_WIDTH(C_SHARP, TOUR_current_octave);	\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'I':															\
								TOUR_current_width = TOUR_WIDTH(D_SHARP, TOUR_current_octave);	\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'J':															\
								TOUR_current_width = TOUR_WIDTH(F_SHARP, TOUR_current_octave);	\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'K':															\
								TOUR_current_width = TOUR_WIDTH(G_SHARP, TOUR_current_octave);	\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'L':															\
								TOUR_current_width = TOUR_WIDTH(A_SHARP, TOUR_current_octave);	\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'T':															\
								++TOUR_current_data;											\
								TOUR_current_ticks_per_64th_note = TOUR_GET_TICKS_PER_64TH_NOTE_FROM_BPM((int)*TOUR_current_data + 50);	\
								++TOUR_current_data;											\
								break;															\
							case 'O':															\
								++TOUR_current_data;											\
								TOUR_current_octave = *TOUR_current_data;						\
								++TOUR_current_data;											\
								break;															\
							case 'U':															\
								++TOUR_current_data;											\
								TOUR_uwaaang_duration = *TOUR_current_data;						\
								++TOUR_current_data;											\
								switch ( *TOUR_current_data )												\
								{																			\
								case 'C':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(C, TOUR_uwaaang_duration);		\
									break;																	\
								case 'D':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(D, TOUR_uwaaang_duration);		\
									break;																	\
								case 'E':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(E, TOUR_uwaaang_duration);		\
									break;																	\
								case 'F':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(F, TOUR_uwaaang_duration);		\
									break;																	\
								case 'G':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(G, TOUR_uwaaang_duration);		\
									break;																	\
								case 'A':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(A, TOUR_uwaaang_duration);		\
									break;																	\
								case 'B':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(B, TOUR_uwaaang_duration);		\
									break;																	\
								case 'H':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(C_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								case 'I':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(D_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								case 'J':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(F_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								case 'K':																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(G_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								default:																	\
									TOUR_uwaaang_width_start = TOUR_WIDTH(A_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								}																			\
																								\
								++TOUR_current_data;											\
								TOUR_uwaaang_duration = *TOUR_current_data;						\
								++TOUR_current_data;											\
								switch ( *TOUR_current_data )												\
								{																			\
								case 'C':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(C, TOUR_uwaaang_duration);			\
									break;																	\
								case 'D':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(D, TOUR_uwaaang_duration);			\
									break;																	\
								case 'E':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(E, TOUR_uwaaang_duration);			\
									break;																	\
								case 'F':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(F, TOUR_uwaaang_duration);			\
									break;																	\
								case 'G':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(G, TOUR_uwaaang_duration);			\
									break;																	\
								case 'A':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(A, TOUR_uwaaang_duration);			\
									break;																	\
								case 'B':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(B, TOUR_uwaaang_duration);			\
									break;																	\
								case 'H':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(C_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								case 'I':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(D_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								case 'J':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(F_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								case 'K':																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(G_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								default:																	\
									TOUR_uwaaang_width_end = TOUR_WIDTH(A_SHARP, TOUR_uwaaang_duration);	\
									break;																	\
								}																			\
								++TOUR_current_data;											\
								TOUR_uwaaang_duration = *TOUR_current_data;						\
								++TOUR_current_data;											\
																								\
								TOUR_current_width = TOUR_uwaaang_width_start;					\
								TOUR_remained_duration = TOUR_uwaaang_duration;					\
								break;															\
							case 'R':															\
								TOUR_current_width = TOUR_WIDTH_REST;							\
								++TOUR_current_data;											\
								TOUR_remained_duration = *TOUR_current_data;					\
								++TOUR_current_data;											\
								break;															\
							case 'M':															\
								++TOUR_current_data;											\
								TOUR_current_score = TOUR_current_data;							\
								break;															\
							case '$':															\
								TOUR_current_width = TOUR_WIDTH_REST;							\
								TOUR_current_data = TOUR_current_score;							\
								TOUR_remained_duration = 1;										\
								TOUR_enabled = 0;												\
								break;															\
							default:															\
								TOUR_current_data = TOUR_current_score;							\
								break;															\
							}																	\
						}																		\
																								\
						OCR1A = TOUR_current_width;												\
					}																			\
					else if ( TOUR_uwaaang_duration != 0 )										\
					{																			\
						TOUR_current_width = TOUR_uwaaang_width_end - ( (long)TOUR_uwaaang_width_end - TOUR_uwaaang_width_start ) * TOUR_remained_duration / TOUR_uwaaang_duration;	\
						OCR1A = TOUR_current_width;												\
																								\
						if ( TOUR_remained_duration == 1 )										\
							TOUR_uwaaang_duration = 0;											\
					}																			\
																								\
					TOUR_tick = 0;																\
				}																				\
				else																			\
					++TOUR_tick;																\
			}																					\
		}																						\
		while ( 0 )

