#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
#include"snake.h"

//蛇身的一个节点结构体
typedef struct _tag_block 
{
	int x;
	int y;
	struct _tag_block *NextBlock;
}Block;

//定义全局变量
int Score = 0;                                                 //总得分
int Add = 10;                                                 //每次吃食物得分。
int drt;                                                           //方向
int sleeptime = 200;                                      //延迟的时间间隔
Block *head;                                                  //蛇头指针
Block*food;                                                   //食物指针
Block *q;                                                        //遍历蛇的时候用到的指针
int GameOver_Status = 0;                             //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。

//定义游戏地图坐标及长度
int SNAKE_X = 2;                                      //贪吃蛇相对地图的左上角横坐标
int SNAKE_Y = 1;									    //贪吃蛇相对地图的左上角纵坐标		
int SQUARE_X = 4;                                  //游戏左上角横坐标
int SQUARE_Y = 4;                                  //游戏左上角纵坐标													
int SQUARE_W = 40;                               //地图宽度
int SQUARE_L = 100;                               //地图长度
//有可能的话可以实现食物颜色的随机变化 

//设置光标位置
void Pos(int x, int y)
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

//创建地图
void Create_Map()
{
	int i;

	for (i = SQUARE_X; i <= SQUARE_L + SQUARE_X; i += 2)
	{
		//print top
		Pos(i, SQUARE_Y);
		printf("■");
		//print bottom
		Pos(i, SQUARE_Y + SQUARE_W);
		printf("■");
	}
	for (i = SQUARE_Y; i <= SQUARE_Y + SQUARE_W; i++)
	{
		//print left
		Pos(SQUARE_X, i);
		printf("■");
		//print right
		Pos(SQUARE_X + SQUARE_L, i);
		printf("■");
	}
}

//初始化蛇身         
void Init_Snake()
{
	Block *tail;
	int i;
	tail = (Block*)malloc(sizeof(Block));
	tail->x = SNAKE_X+SQUARE_X;
	tail->y = SNAKE_Y+SQUARE_Y;
	tail->NextBlock = NULL;
	for (i = 1; i <= 4; i++)
	{
		head = (Block*)malloc(sizeof(Block));
		head->NextBlock = tail;
		head->x = tail->x + 2 * i;
		head->y = tail->y;
		tail = head;
	}
	while (tail != NULL)
	{
		Pos(tail->x, tail->y);
		printf("■");
		tail = tail->NextBlock;
	}
}

//判断是否咬到了自己
int BiteSelf()
{
	Block *SnakeSelf;
	SnakeSelf = head->NextBlock;
	while (SnakeSelf != NULL)
	{
		if (SnakeSelf->x == head->x && SnakeSelf->y == head->y)
		{
			return 1;
		}
		SnakeSelf = SnakeSelf->NextBlock;
	}
	return 0;
}

//制造食物
void Create_Food()
{
	Block *NewFood;
	srand((unsigned)time(NULL));
	NewFood = (Block*)malloc(sizeof(Block));
	while ((NewFood->x % 2) != 0) //保证其为偶数，使得食物能与蛇头对其
	{
		NewFood->x =SQUARE_X+ rand() %(SQUARE_L-3) + 2;
	}
	NewFood->y =SQUARE_Y+ rand() % (SQUARE_W-1) + 1;
	q = head;
	while (q->NextBlock == NULL)
	{
		if (q->x == NewFood->x && q->y == NewFood->y) //判断蛇身是否与食物重合
		{
			free(NewFood);
			Create_Food();
		}
		q = q->NextBlock;
	}
	Pos(NewFood->x, NewFood->y);
	printf("■");
	food = NewFood;
}

//蛇头是否撞墙
void HitWall()
{
	if (head->x == SQUARE_X || head->x == SQUARE_X+SQUARE_L || head->y == SQUARE_Y || head->y == SQUARE_Y+SQUARE_W)
	{
		GameOver_Status = 1;
		Game_Over();
	}
}

//蛇前进,上SNAKE_UP,下SNAKE_DOWN,左SNAKE_LEFT,右SNAKE_RIGHT
void Snake_Move()
{
	Block * NextHead;
	HitWall();

	NextHead = (Block*)malloc(sizeof(Block));
	if (drt == SNAKE_UP)
	{
		NextHead->x = head->x;
		NextHead->y = head->y - 1;
		if (NextHead->x == food->x && NextHead->y == food->y) //如果下一个有食物
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Score = Score + Add;
			Create_Food();
		}
		else //如果没有食物//
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q->NextBlock->NextBlock != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Pos(q->NextBlock->x, q->NextBlock->y);
			printf(" ");
			free(q->NextBlock);
			q->NextBlock = NULL;
		}
	}
	if (drt == SNAKE_DOWN)
	{
		NextHead->x = head->x;
		NextHead->y = head->y + 1;
		if (NextHead->x == food->x && NextHead->y == food->y) //有食物
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Score = Score + Add;
			Create_Food();
		}
		else //没有食物
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q->NextBlock->NextBlock != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Pos(q->NextBlock->x, q->NextBlock->y);
			printf(" ");
			free(q->NextBlock);
			q->NextBlock = NULL;
		}
	}
	if (drt == SNAKE_LEFT)
	{
		NextHead->x = head->x - 2;
		NextHead->y = head->y;
		if (NextHead->x == food->x && NextHead->y == food->y)//有食物
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Score = Score + Add;
			Create_Food();
		}
		else //没有食物
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q->NextBlock->NextBlock != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Pos(q->NextBlock->x, q->NextBlock->y);
			printf(" ");
			free(q->NextBlock);
			q->NextBlock = NULL;
		}
	}
	if (drt == SNAKE_RIGHT)
	{
		NextHead->x = head->x + 2;
		NextHead->y = head->y;
		if (NextHead->x == food->x && NextHead->y == food->y)//有食物
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Score = Score + Add;
			Create_Food();
		}
		else //没有食物
		{
			NextHead->NextBlock = head;
			head = NextHead;
			q = head;
			while (q->NextBlock->NextBlock != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->NextBlock;
			}
			Pos(q->NextBlock->x, q->NextBlock->y);
			printf(" ");
			free(q->NextBlock);
			q->NextBlock = NULL;
		}
	}
	if (BiteSelf() == 1) //判断是否会咬到自己
	{
		GameOver_Status = 2;
		Game_Over();
	}
}

//暂停
void Pause()
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}

	}
}

//游戏控制 
void Game_Charge()
{

	int i;

	for (i = SQUARE_X+SQUARE_L+2; i <= SQUARE_L + SQUARE_X+40; i += 2)
	{
		//print top
		Pos(i, SQUARE_Y);
		printf("■");
		//print bottom
		Pos(i, SQUARE_Y + SQUARE_W);
		printf("■");
	}
	for (i = SQUARE_Y; i <= SQUARE_Y + SQUARE_W; i++)
	{
		//print left
		Pos(SQUARE_X+SQUARE_L+2, i);
		printf("■");
		//print right
		Pos(SQUARE_X + SQUARE_L+40, i);
		printf("■");
	}
	Pos(SQUARE_X + SQUARE_L + 6, SQUARE_Y + 5);
	printf("1.不能穿墙，不能咬到自己");
	Pos(SQUARE_X + SQUARE_L + 6, SQUARE_Y + 7);
	printf("2.用↑.↓.←.→分别控制蛇的移动.");
	Pos(SQUARE_X + SQUARE_L + 6, SQUARE_Y + 9);
	printf("3.F1 为加速，F2 为减速\n");
	Pos(SQUARE_X + SQUARE_L + 6, SQUARE_Y + 11);
	printf("4.ESC ：退出游戏.space：暂停游戏.");
	Pos(SQUARE_X + SQUARE_L + 6, SQUARE_Y + 13);
	drt = SNAKE_RIGHT;
	while (1)
	{
		Pos(SQUARE_X+SQUARE_L+6, SQUARE_Y+(SQUARE_W)/2+2);
		printf("√当前得分：%d ", Score);
		Pos(SQUARE_X + SQUARE_L + 6, SQUARE_Y + (SQUARE_W) / 2 + 4);
		printf("√每个食物得分：%d分", Add);
		if (GetAsyncKeyState(VK_UP) && drt != SNAKE_DOWN)
		{
			drt = SNAKE_UP;
		}
		else if (GetAsyncKeyState(VK_DOWN) && drt != SNAKE_UP)
		{
			drt = SNAKE_DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && drt != SNAKE_RIGHT)
		{
			drt = SNAKE_LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && drt != SNAKE_LEFT)
		{
			drt = SNAKE_RIGHT;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			Pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			GameOver_Status = 3;
			Game_Over();
		}
		else if (GetAsyncKeyState(VK_F1))
		{
			if (sleeptime >= 50)
			{
				sleeptime = sleeptime - 30;
				Add = Add + 2;
				if (sleeptime == 320)
				{
					Add = 2;//防止减到1之后再加回来有错
				}
			}
		}
		else if (GetAsyncKeyState(VK_F2))
		{
			if (sleeptime<350)
			{
				sleeptime = sleeptime + 30;
				Add = Add - 2;
				if (sleeptime == 350)
				{
					Add = 1; //保证最低分为1
				}
			}
		}
		Sleep(sleeptime);
		Snake_Move();
	}
}

void Welcome()
{
	int i;

	for (i = SQUARE_X; i <= SQUARE_L + SQUARE_X; i += 2)
	{
		//print top
		Pos(i, SQUARE_Y);
		printf("■");
		//print bottom
		Pos(i, SQUARE_Y + SQUARE_W);
		printf("■");
	}
	for (i = SQUARE_Y; i <= SQUARE_Y + SQUARE_W; i++)
	{
		//print left
		Pos(SQUARE_X, i);
		printf("■");
		//print right
		Pos(SQUARE_X + SQUARE_L, i);
		printf("■");
	}

}

void Game_Diy()
{
	char ch;
	system("cls");
	Welcome();
	Pos(SQUARE_X+(SQUARE_L/2)-16,SQUARE_Y+4);
	printf("1.欢迎来到贪食蛇游戏！");
	Pos(SQUARE_X + (SQUARE_L / 2)-16, SQUARE_Y + 5);
	printf("2.方向键控制蛇的移动,F1加速,F2减速");
	Pos(SQUARE_X + (SQUARE_L / 2)-16, SQUARE_Y + 6);
	printf("3.加速将能得到更高的分数。\n");
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 8);
	printf("请选择地图大小（S）");
	ch =_getch();
	if (ch == 'S')
	{
		Set_Size_Coord();
	}
	else
	{
		printf("请按照提示输入\n");
	}
	SetColor();
	system("cls");
}

void Game_Over()//结束游戏
{
	int i;
	int hscore;
	char ctn;
	system("cls");
	Game_Score();
	FILE *hs;
	hs = fopen("scorefile", "r");
	hscore = _getw(hs);
	fclose(hs);
	for (i = SQUARE_X; i <= SQUARE_L + SQUARE_X; i += 2)
	{
		//print top
		Pos(i, SQUARE_Y);
		printf("■");
		//print bottom
		Pos(i, SQUARE_Y + SQUARE_W);
		printf("■");
	}
	for (i = SQUARE_Y; i <= SQUARE_Y + SQUARE_W; i++)
	{
		//print left
		Pos(SQUARE_X, i);
		printf("■");
		//print right
		Pos(SQUARE_X + SQUARE_L, i);
		printf("■");
	}
	Pos(SQUARE_X+(SQUARE_L/2)-10,SQUARE_Y+SQUARE_W/2);
	if (GameOver_Status == 1)
	{
		printf("Holy shit!you hit the wall!");
	}
	else if (GameOver_Status == 2)
	{
		printf("Idiot,you bite yourself !");
	}
	else if (GameOver_Status == 3)
	{
		printf("you quit this game!");
	}
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + SQUARE_W / 2+1);
	printf("SCORE:%d and the highest score is %d\n", Score,hscore);
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + SQUARE_W / 2 + 2);
	printf("Good Bye!\n");
	Sleep(3000);
	exit(1);
	/*
	printf("would you want to play again? yes(Y) or no (N)");
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + SQUARE_W / 2 + 3);
	scanf_s("%c", &ctn);
	if (ctn == 'Y')
	{
	Game_Replay();
	}
	else if(ctn=="N")
	{
		Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + SQUARE_W / 2 + 4);
		printf("Good bye");
		Sleep(500);
		exit(1);
	}
	ctn = NULL;
	*/
	//这里应实现游戏的重玩功能，但可能因为变量的缓存问题出现错误        \
	  需要抽时间完成
}

void SetColor()
{
	int i = 0;
	char ch;
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 13);
	printf("请选择背景及颜色（C）");
	if (_getch()== 'C')
	{
		Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 15);
		printf("请选择皮肤：\n");
		Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 16);
		printf("1.清爽夏天\n");
		Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 17);
		printf("2.黑白极简\n");
		Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 18);
		scanf_s("%d", &i);
		if (i == 1)
		{
			system("color 72");
		}
		else if (i == 2)
		{
			system("color 70");
		}
	}
	else
	{
		system("color 70");
	}
	
}

void Set_Size_Coord()
{
	int i;
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 9);
	printf("请按照提示输入游戏场景大小：\n");
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 10);
	printf("1.大               2.中                 3.小\n");
	Pos(SQUARE_X + (SQUARE_L / 2) - 16, SQUARE_Y + 11);
	printf("请输入对应的选项：");
	scanf_s("%d", &i);
	if (i == 1)
	{
		SQUARE_L = 140;
		SQUARE_W = 50;
	}
	else if (i == 2)
	{
		SQUARE_L = 100;
		SQUARE_W = 40;
	}
	else if(i==3)
	{
		SQUARE_L = 50;
		SQUARE_W = 30;
	}
	else
	{
		SQUARE_L = 100;
		SQUARE_W = 50;
	}
}

void Game_Start()//游戏初始化
{
	Welcome();
	Create_Map();
	Init_Snake();
	Create_Food();
}

//将最大分数写入文件scorefile
void Game_Score()
{
	int s = 0;
	FILE *scoref;

	//读取scorefile文件中的内容
	scoref = fopen("scorefile", "r");
	int sh = _getw(scoref);
	fclose(scoref);
	printf("%d", sh);
	if(Score >sh)  //如果文件中的内容小于分数，则将分数写入文件
	{
		scoref = fopen("scorefile", "w");
		_putw(Score, scoref);
		fclose(scoref);
	}
}

/*
void Game_Replay()
{

	system("cls");
	Game_Diy();
	Create_Map();
	Game_Start();
	Game_Charge();
	Game_Over();
}
*/