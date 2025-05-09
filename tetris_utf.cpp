#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

//*********************************
//상수 선언
//*********************************

#define EXT_KEY			0xffffffe0	//확장키 인식값 
#define KEY_LEFT		0x4b
#define KEY_RIGHT		0x4d
#define KEY_UP			0x48
#define KEY_DOWN		0x50

//*********************************
//구조체 선언
//*********************************
struct STAGE{		//각 스테이지마다의 난이도 설정
	int	speed;	//숫자가 낮을수록 속도가 빠르다
	int stick_rate;		//막대가 나오는 확률 0~99 , 99면 막대기만 나옴
	int clear_line; //클리어해야 하는 줄의 개수
};

enum { 
		BLACK,      /*  0 : 까망 */ 
		DARK_BLUE,    /*  1 : 어두운 파랑 */ 
		DARK_GREEN,    /*  2 : 어두운 초록 */ 
		DARK_SKY_BLUE,  /*  3 : 어두운 하늘 */ 
		DARK_RED,    /*  4 : 어두운 빨강 */ 
		DARK_VOILET,  /*  5 : 어두운 보라 */ 
		DARK_YELLOW,  /*  6 : 어두운 노랑 */ 
		GRAY,      /*  7 : 회색 */ 
		DARK_GRAY,    /*  8 : 어두운 회색 */ 
		BLUE,      /*  9 : 파랑 */ 
		GREEN,      /* 10 : 초록 */ 
		SKY_BLUE,    /* 11 : 하늘 */ 
		RED,      /* 12 : 빨강 */ 
		VOILET,      /* 13 : 보라 */ 
		YELLOW,      /* 14 : 노랑 */ 	
		WHITE,      /* 15 : 하양 */ 	
}; 

//*********************************
//전역변수선언
//*********************************
int level; //각 레벨을 나타냄
int ab_x,ab_y;	//화면중 블럭이 나타나는 좌표의 절대위치
int block_shape,block_angle,block_x,block_y;
int next_block_shape; //다음 블럭의 모양
int score; //점수
int lines;
char total_block[21][14];		//화면에 표시되는 블럭들
struct STAGE stage_data[10];
char block[7][4][4][4] = {
	//막대모양
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,

	//네모모양
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,

	//'ㅓ' 모양
	0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,	1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,	0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,

	//'ㄱ'모양
	1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,	1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,	0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,

	//'ㄴ' 모양
	1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,	1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,	1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,

	//'Z' 모양
	1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,	1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,

	//'S' 모양
	0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,	0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0

};
//*********************************
//함수 선언
//*********************************
int gotoxy(int x,int y);	//커서옮기기
void SetColor(int color);	//색표현
int init();					//각종변수 초기화
int show_cur_block(int shape,int angle,int x,int y);	//진행중인 블럭을 화면에 표시한다
int erase_cur_block(int shape,int angle,int x,int y);	//블럭 진행의 잔상을 지우기 위한 함수
int show_total_block();	//쌓여져있는 블럭을 화면에 표시한다.
int show_next_block(int shape);
int make_new_block();	//return값으로 block의 모양번호를 알려줌
int strike_check(int shape,int angle,int x,int y);	//블럭이 화면 맨 아래에 부닥쳤는지 검사 부닥치면 1을리턴 아니면 0리턴
int merge_block(int shape,int angle,int x,int y);	//블럭이 바닥에 닿았을때 진행중인 블럭과 쌓아진 블럭을 합침
int block_start(int shape,int* angle,int* x,int* y);	//블럭이 처음 나올때 위치와 모양을 알려줌
int move_block(int* shape,int* angle,int* x,int* y,int* next_shape);	//게임오버는 1을리턴 바닥에 블럭이 닿으면 2를 리턴
int rotate_block(int shape,int* angle,int* x,int* y);
int show_gameover();
int show_gamestat();
int show_logo();
int input_data();
int check_full_line();	
int show_clear_screen();


int main(int argc, char* argv[])
{
	int i;
	int is_gameover=0;
	char keytemp;
	show_logo();
	while(1)
	{  
		score = 0;
		lines = 0;
		if (is_gameover == 3) {
			show_logo();
		}
		init();
		input_data();
		is_gameover = 0;
		block_shape = make_new_block();
		next_block_shape = make_new_block();
		show_next_block(next_block_shape);
		show_total_block();
		block_start(block_shape,&block_angle,&block_x,&block_y);
		show_gamestat();
		for(i=1;1;i++)
		{
			if(_kbhit())
			{
				keytemp = _getche();
				if(keytemp == EXT_KEY)
				{
					keytemp = _getche();
					switch(keytemp)
					{
					case KEY_UP:		//회전하기
						rotate_block(block_shape, &block_angle, &block_x, &block_y);
						break;
					case KEY_LEFT:		//왼쪽으로 이동
						if(block_x>1)
						{
							erase_cur_block(block_shape,block_angle,block_x,block_y);
							block_x--;
							if(strike_check(block_shape,block_angle,block_x,block_y) == 1)
								block_x++;
							
							show_cur_block(block_shape,block_angle,block_x,block_y);
						}
						break;
					case KEY_RIGHT:		//오른쪽으로 이동
						
						if(block_x<14)
						{
							erase_cur_block(block_shape,block_angle,block_x,block_y);
							block_x++;
							if(strike_check(block_shape,block_angle,block_x,block_y) == 1)
								block_x--;
							show_cur_block(block_shape,block_angle,block_x,block_y);
						}
						break;
					case KEY_DOWN:		//아래로 이동
						is_gameover = move_block(&block_shape,&block_angle,&block_x,&block_y,&next_block_shape);
						if(is_gameover!=1&& is_gameover!=3){
							show_cur_block(block_shape, block_angle, block_x, block_y);
						}
						break;
					}
				}
				else {
					//특수 키 아닌 경우 버퍼 지우기
					while (_kbhit()) (void)_getch();
				}

				if(keytemp == 32 )	//스페이스바를 눌렀을때
				{
					while(is_gameover == 0)
					{
						is_gameover = move_block(&block_shape,&block_angle,&block_x,&block_y,&next_block_shape);
					}
					show_cur_block(block_shape,block_angle,block_x,block_y);
					while (_kbhit()) (void)_getch();

				}
			}
			if(i%stage_data[level].speed == 0)
			{
				is_gameover = move_block(&block_shape,&block_angle,&block_x,&block_y,&next_block_shape);
				if (is_gameover != 1&&is_gameover !=3) {
					show_cur_block(block_shape, block_angle, block_x, block_y);
				}
			}
			
			if(is_gameover == 1)
			{
				show_gameover();
				SetColor(GRAY);
				is_gameover = 0;
				while (_kbhit()) (void)_getch();
				system("cls");
				break;
			}
			if (is_gameover == 3) {
				while (_kbhit()) (void)_getch();
				system("cls");
				break;
			}
			
			gotoxy(77,23);
			Sleep(15);
			gotoxy(77,23);
		}
	}
	return 0;
}

//좌표지정 함수
int gotoxy(int x,int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD pos; 
	pos.Y=y;
	pos.X=x;
	SetConsoleCursorPosition(hConsole, pos); 
	return 0;
}


//색 지정 함수
void SetColor(int color) 

{ 
	static HANDLE std_output_handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std_output_handle, color); 
	
} 

//초기화 하는 함수
int init()
{
	int i,j;

	srand((unsigned)time(NULL));
	
	
	for(i=0;i<20;i++)
	{
		for(j=0;j<14;j++)
		{
			if((j==0) || (j==13))
			{
				total_block[i][j]=1;
			}else{
				total_block[i][j]=0;
			}
		}
	}

	for(j=0;j<14;j++)			//화면의 제일 밑의 줄은 1로 채운다.
		total_block[20][j]=1;
	
	//전역변수 초기화
	level=0; //레벨은 0으로
	lines=0; //라인도 0으로
	ab_x = 5; //블럭이 나타나는 절대 위치 초기화
	ab_y = 1;

	stage_data[0].speed=40; //각 레벨 별 떨어지는 속도 지정
	stage_data[0].stick_rate=20;	
	stage_data[0].clear_line=1; //쉬운 확인을 위해 클리어해야하는 줄의 수 1로 변경
	stage_data[1].speed=38;
	stage_data[1].stick_rate=18;
	stage_data[1].clear_line=1;
	stage_data[2].speed=35;
	stage_data[2].stick_rate=18;
	stage_data[2].clear_line=1;
	stage_data[3].speed=30;
	stage_data[3].stick_rate=17;
	stage_data[3].clear_line=1;
	stage_data[4].speed=25;
	stage_data[4].stick_rate=16;
	stage_data[4].clear_line=1;
	stage_data[5].speed=20;
	stage_data[5].stick_rate=14;
	stage_data[5].clear_line=1;
	stage_data[6].speed=15;
	stage_data[6].stick_rate=14;
	stage_data[6].clear_line=1;
	stage_data[7].speed=10;
	stage_data[7].stick_rate=13;
	stage_data[7].clear_line=1;
	stage_data[8].speed=6;
	stage_data[8].stick_rate=12;
	stage_data[8].clear_line=1;
	stage_data[9].speed=4;
	stage_data[9].stick_rate=11;
	stage_data[9].clear_line=1;
	return 0;
}


//현재 떨어지고 있는 블럭을 화면에 표시
int show_cur_block(int shape,int angle,int x,int y)
{
	int i,j;
	
	switch(shape)
	{
	case 0:
		SetColor(RED);
		break;
	case 1:
		SetColor(BLUE);
		break;
	case 2:
		SetColor(SKY_BLUE);
		break;
	case 3:
		SetColor(WHITE);
		break;
	case 4:
		SetColor(YELLOW);
		break;
	case 5:
		SetColor(VOILET);
		break;
	case 6:
		SetColor(GREEN);
		break;
	}

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if( (j+y) <0)
				continue;

			if(block[shape][angle][j][i] == 1)
			{
				gotoxy((i+x)*2+ab_x,j+y+ab_y);
				printf("■");

			}
		}
	}
	SetColor(BLACK);
	gotoxy(77,23);
	return 0;
}


//블록이 이동하거나 회전할때 블럭을 지우는 함수
int erase_cur_block(int shape,int angle,int x,int y)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(block[shape][angle][j][i] == 1)
			{
				gotoxy((i+x)*2+ab_x,j+y+ab_y);
				printf("  ");
				//break;
				
			}
		}
	}
	return 0;	
}


//이미 바닥에 떨어져서 자리잡은 블럭들을 표시
int show_total_block()
{
	int i,j;
	SetColor(DARK_GRAY);
	for(i=0;i<21;i++)
	{
		for(j=0;j<14;j++)
		{
			if(j==0 || j==13 || i==20)		//레벨에 따라 외벽 색이 변함
			{
				SetColor((level %6) +1);
				
			}else{
				SetColor(DARK_GRAY);
			}
			gotoxy( (j*2)+ab_x, i+ab_y );
			if(total_block[i][j] == 1)
			{	
				printf("■");
			}else{
				printf("  ");
			}
			
		}
	}
	SetColor(BLACK);
	gotoxy(77,23);
	return 0;
}

//새로운 블럭을 만드는 함수
int make_new_block()
{
	int shape;
	int i;
	i=rand()%100;
	if(i<=stage_data[level].stick_rate)		//막대기 나올확률 계산
		return 0;							//막대기 모양으로 리턴

	shape = (rand()%6)+1;		//shape에는 1~6의 값이 들어감
	show_next_block(shape);
	return shape;
}


//충돌여부 확인
int strike_check(int shape,int angle,int x,int y)
{
	int i,j;
	int block_dat;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(  ((x+j) == 0)  || ((x+j) == 13) )
				block_dat = 1;
			else
				block_dat = total_block[y+i][x+j];
			
			
			if((block_dat == 1) && (block[shape][angle][i][j] == 1))																							//좌측벽의 좌표를 빼기위함
			{
				return 1;
			}
		}
	}
	return 0;
}

//떨어지고 있는 블럭이 다른 블럭이나 바닥에 부딪혔을때 고정 시키는 함수
int merge_block(int shape,int angle,int x,int y)
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			total_block[y+i][x+j] |=  block[shape][angle][i][j];
		}
	}
	int is_clear  = check_full_line();
	if (is_clear==3) {
		return 3;
	}
	show_total_block();
	
	return 0;
}

//블록이 생성되었을 때 이를 배치시키는 함수
int block_start(int shape,int* angle,int* x,int* y)
{
	
	*x = 5;
	*y = -3;
	*angle = 0;
	return 0;	
}


//게임이 끝났을때 게임 오버창을 띄우는 함수
int show_gameover()
{
	SetColor(RED);
	gotoxy(15,8);
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(15,9);
	printf("┃**************************┃");
	gotoxy(15,10);
	printf("┃*        GAME OVER       *┃");
	gotoxy(15,11);
	printf("┃**************************┃");
	gotoxy(15,12);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	fflush(stdin);
	Sleep(1000);
	
	(void)_getche();
	system("cls");

	return 0;
}


//블록을 아래로 내려주는 함수, 바닥에 닿거나 다른 블록과 충돌하면 멈추며 고정
int move_block(int* shape,int* angle,int* x,int* y,int* next_shape)
{
	erase_cur_block(*shape,*angle,*x,*y);
	
	(*y)++;	//블럭을 한칸 아래로 내림
	if(strike_check(*shape,*angle,*x,*y) == 1)
	{

		(*y)--;
		if(*y<0)	//게임오버
		{
			SetColor(DARK_GRAY);
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[*shape][*angle][i][j] == 1) {
						int dx = *x + j;
						int dy = *y + i;
						if (dx >= 0 && dx <= 14 && dy >= 0 && dy < 20) {
							gotoxy(dx * 2 + ab_x, dy + ab_y);
							printf("■");
						}
					}
				}
			}
			Sleep(100);

			
			return 1;
		}

		int is_over = merge_block(*shape, *angle, *x, *y);
		
		if (is_over == 3) {
			return 3;
		}
		*shape = *next_shape;
		*next_shape = make_new_block();
		
		block_start(*shape,angle,x,y);	//angle,x,y는 포인터임
		show_next_block(*next_shape);
		return 2;
	}
	erase_cur_block(*shape, *angle, *x, *y);


	return 0;
}

//블록을 회전시킬때 사용
int rotate_block(int shape,int* angle,int* x,int* y)
{
	int new_angle = (*angle + 1) % 4;
	if (strike_check(shape, new_angle, *x, *y) == 0)
	{
		erase_cur_block(shape, *angle, *x, *y);
		*angle = new_angle;
		show_cur_block(shape, *angle, *x, *y);
	}
	return 0;
}

//꽉 차있는 줄을 삭제하고 위의 블록들을 내려줌.
int check_full_line()
{
	int i,j,k;
	for(i=0;i<20;i++)
	{
		for(j=1;j<13;j++)
		{
			if(total_block[i][j] == 0)
				break;
		}
		if(j == 13)	//한줄이 다 채워졌음
		{
			lines++;
			//줄과 레벨 확인
			if (stage_data[level].clear_line <= lines) {
				if (level < 9) {
					lines = 0;
					level++;
					show_gamestat();
				}
				//만약 레벨이 10이 되는 경우 쇼우 클리어를 진행
				else {
					//show_clear_screen이라는 것에서 1의 값을 받으면 3을 반환
					// --> merge_block()에서 이 값에 따른 반환값을 만들도록함
					//메인 함수에서 is_game_over이 3일때 break함
					//이때 화면에서 show_logo()가 잘 보이지 않는다는 것을 확인하여 show_logo() 앞 cls를 통해 지우고 다시 그리도록 함.

					show_total_block();
					lines = 0;
					show_gamestat();
					if (show_clear_screen()) return 3;
				}
			}
			
			show_total_block();
			SetColor(BLUE);
			gotoxy(1*2+ab_x,i+ab_y);
			for(j=1;j<13;j++)
			{
				printf("□");
				Sleep(10);
			}
			gotoxy(1*2+ab_x,i+ab_y);
			for(j=1;j<13;j++)
			{
				printf("  ");
				Sleep(10);
			}

			for(k=i;k>0;k--)
			{
				for(j=1;j<13;j++)
					total_block[k][j] = total_block[k-1][j];
			}
			for(j=1;j<13;j++)
				total_block[0][j] = 0;
			score+= 100+(level*10) + (rand()%10);
			show_gamestat();
		}

	}
	
	return 0;
}

//다음에 나올 블럭이 무엇인지 알려주는 창 띄우는 함수
int show_next_block(int shape)
{
	int i,j;
	SetColor((level+1)%6+1);
	for(i=1;i<7;i++)
	{
		gotoxy(33,i);
		for(j=0;j<8;j++)
		{
			if(i==1 || i==6 || j==0 || j==4)
			{
				printf("■");				
			}else{
				printf("  ");
			}

		}
	}
	show_cur_block(shape,0,15,1);
	return 0;
}


//현재 게임의 진행상태를 보여주는 함수, 앞으로 깨야하는 줄의 수, 점수, 레벨을 보여줌
int show_gamestat()
{
	static int printed_text=0;
	SetColor(GRAY);
	if(printed_text == 0)
	{
		gotoxy(33,7);
		printf("STAGE");

		gotoxy(33,9);
		printf("SCORE");

		gotoxy(33,12);
		printf("LINES");
		

	}
	gotoxy(41,7);
	printf("%10d",level+1);
	gotoxy(41,9);
	printf("%10d",score);
	gotoxy(41,12);
	printf("%10d",stage_data[level].clear_line - lines);
	return 0;
}


//맨 처음 게임 선택창으로 숫자를 선택하도록 함.
int input_data()
{
	int i=0;

	char buf[5];

	//입력값 오류 해결 --> scanf_s()로 받게 되는 경우 1.0도 1로 인식하는 경우 생김, 
	//또한 선행 0을 허용하기 때문에 문제를 해결하기 위해 문자를 입력받는것으로 바꿈
	while (1) {
		SetColor(GRAY);
		gotoxy(10, 7);
		printf("┏━━━━━━━━━<GAME KEY>━━━━━━━━━┓");
		Sleep(10);
		gotoxy(10, 8);
		printf("┃ UP   : Rotate Block        ┃");
		Sleep(10);
		gotoxy(10, 9);
		printf("┃ DOWN : Move One-Step Down  ┃");
		Sleep(10);
		gotoxy(10, 10);
		printf("┃ SPACE: Move Bottom Down    ┃");
		Sleep(10);
		gotoxy(10, 11);
		printf("┃ LEFT : Move Left           ┃");
		Sleep(10);
		gotoxy(10, 12);
		printf("┃ RIGHT: Move Right          ┃");
		Sleep(10);
		gotoxy(10, 13);
		printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
		gotoxy(10, 3);
		printf("Select Start level[1-8]:       \b\b\b\b\b\b\b");
		// 출력 버퍼 비우기
		fflush(stdout);

		fgets(buf, sizeof(buf), stdin);
		buf[strcspn(buf, "\n")] = '\0'; // 개행 제거

		// 문자열이 정수형 숫자 하나로만 이루어졌는지 확인
		if (strlen(buf) == 1 && buf[0] >= '1' && buf[0] <= '8') {
			i = buf[0] - '0'; // 문자 → 정수로 변환
			break;
		}
		else {
			while (getchar() != '\n' && !feof(stdin));
			gotoxy(10, 4);
			printf("올바른 숫자를 입력해주세요.");
			Sleep(1000);
			gotoxy(10, 4);
			printf("                            ");
			system("cls");
		}
	}
	
	
	level = i-1;
	system("cls");
	return 0;
}


//가장 처음 테트리스 로고와 시작창을 보이게 하는 함수
int show_logo()
{
	system("cls");
	int i=0,j;
	SetColor(WHITE);
	gotoxy(13,3);
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	Sleep(100);
	gotoxy(13,4);
	printf("┃ ◆◆◆   ◆◆◆  ◆◆◆    ◆◆     ◆   ◆◆◆ ┃");
	Sleep(100);
	gotoxy(13,5);
	printf("┃  ◆    ◆     ◆     ◆ ◆    ◆   ◆   ┃");
	Sleep(100);
	gotoxy(13,6);
	printf("┃  ◆    ◆◆◆   ◆     ◆◆     ◆    ◆  ┃");
	Sleep(100);
	gotoxy(13,7);
	printf("┃  ◆    ◆     ◆     ◆ ◆    ◆     ◆ ┃");
	Sleep(100);
	gotoxy(13,8);
	printf("┃  ◆    ◆◆◆   ◆     ◆  ◆   ◆   ◆◆◆ ┃");
	Sleep(100);
	gotoxy(13,9);
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(20,20);
	printf("Please Press Any Key~!");

	while(true) {
		if(i%40==0)
		{
			
			
			for(j=0;j<5;j++)
			{
				//여기서 18의 값을 조정해서 첫번째 블럭도 다 지워지도록 조정
				gotoxy(6,14+j);
				printf("                                                             ");
				
				
			}
			show_cur_block(rand()%7,rand()%4,6,14);
			show_cur_block(rand()%7,rand()%4,12,14);
			show_cur_block(rand()%7,rand()%4,19,14);
			show_cur_block(rand()%7,rand()%4,24,14);
		}
		if(_kbhit())
			break;
		Sleep(30);
		i++;
	}
	
	while (_kbhit()) (void)_getche();
	system("cls");

	return 0;
}


int show_clear_screen() {
	SetColor(YELLOW);
	system("cls");
	gotoxy(10, 9);
	printf("             * GAME CLEARED *         ");
	gotoxy(10, 10);
	printf("      축하합니다! 모든 스테이지 완료!   ");
	gotoxy(10, 11);
	printf("          점수: %10d점         ", score);
	gotoxy(10, 14);
	SetColor(GRAY);
	Sleep(1000); // 3초 대기
	system("cls");
	return 1;
}