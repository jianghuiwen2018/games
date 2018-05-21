#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include"snake.h"

//define structure block
typedef struct _tag_block
{
	struct _tag_block *Nextblock;
	int x;
	int y;
}block;

//define global variable
block *head;
block *food;
block *p;
int Score;
int Add;
int drt=SNAKE_RIGHT;
int GameOver_Status;
int sleeptime = 200;

void Pos(int x, int y)
{
	COORD pos;
	HANDLE Houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(Houtput, pos);
}

void Create_Map()
{
	/*
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
	*/
	int i;
	for (i = 0; i<58; i += 2)//打印上下边框
	{
		Pos(i, 0);
		printf("■");
		Pos(i, 26);
		printf("■");
	}
	for (i = 1; i<26; i++)//打印左右边框
	{
		Pos(0, i);
		printf("■");
		Pos(56, i);
		printf("■");
	}
}

void Init_Snake()
{
	/*
	int i;
	block *tail;
	tail = (block*)malloc(sizeof(block));
	tail->Nextblock = NULL;
	tail->x = 24;
	tail->y = 5;

	block *head;
	for (i = 1; i <= 4; i++)
	{
		//头插法，从block的尾部开始
		head = (block*)malloc(sizeof(block));
		head->Nextblock = tail;
		head->x = tail->x + i * 2;
		head->y = tail->y;
		tail = head;
	}

	while (tail !=NULL)
	{
		Pos(tail->x, tail->y);
		printf("■");
		tail = tail->Nextblock;
	}
	*/
	block *tail;
	int i;
	tail = (block*)malloc(sizeof(block));//从蛇尾开始，头插法，以x,y设定开始的位置//
	tail->x = 24;
	tail->y = 5;
	tail->Nextblock = NULL;
	for (i = 1; i <= 4; i++)
	{
		head = (block*)malloc(sizeof(block));
		head->Nextblock = tail;
		head->x = 24 + 2 * i;
		head->y = 5;
		tail = head;
	}
	while (tail != NULL)//从头到为，输出蛇身
	{
		Pos(tail->x, tail->y);
		printf("■");
		tail = tail->Nextblock;
	}
}

void Create_Food()
{
	/*
	block*NewFood;
	srand((unsigned)time(NULL));
	NewFood = (block*)malloc(sizeof(block));
	while ((NewFood->x % 2)!=0)  //保证其为偶数，使得食物能够与蛇头对齐
	{
		NewFood->x = (rand() % (SQUARE_L  - 1)) + SQUARE_X + 1;
	}
	NewFood->y = (rand() % (SQUARE_W - 1)) + SQUARE_Y + 1;

	p = head;
	//这里有一个疑问：while循环
	while (p->Nextblock == NULL)
	{
		if (p->x == NewFood->x && p->y == NewFood->y) //判断蛇身是否与食物重合
		{
			free(NewFood);
			Create_Food();
		}
		p = p->Nextblock;
	}
	Pos(NewFood->x, NewFood->y);
	food = NewFood;
	printf("■");
	*/
	block *NewFood;
	srand((unsigned)time(NULL));
	NewFood = (block*)malloc(sizeof(block));
	while ((NewFood->x % 2) != 0) //保证其为偶数，使得食物能与蛇头对其
	{
		NewFood->x = rand() % 52 + 2;
	}
	NewFood->y = rand() % 24 + 1;
	p = head;
	while (p->Nextblock == NULL)
	{
		if (p->x == NewFood->x && p->y == NewFood->y) //判断蛇身是否与食物重合
		{
			free(NewFood);
			Create_Food();
		}
		p = p->Nextblock;
	}
	Pos(NewFood->x, NewFood->y);
	food = NewFood;
	printf("■");
}

void Snake_Move()
{
	block*NextHead;
	HitWall();
	NextHead = (block*)malloc(sizeof(block));

	if (drt == SNAKE_UP)
	{
		NextHead->x = head->x;
		NextHead->y = head->y - 1;

		//如果吃到食物
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//头插法，吃到食物的时候只新建一个节点，并将其插入snake的链表中
			NextHead->Nextblock = head;
			head = NextHead;
			//p的作用是用来遍历链表中的所用节点              
			//如果节点不为空的话，就打印出该节点
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//吃到食物时，加分并且分配新的食物
			Score = Score + Add;
			Create_Food();
		}
		//没有吃到食物
		//遍历所有节点                                                                                \
		蛇头节点转移到新节点NextHead，                                                  \
        剩余节点依次转移到其各自对应的上一个节点                                    \
		在之前最后一个节点的位置打印空格“ ”                                          \
		释放最后一个节点                                                                             \
		依次打印所有节点
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;

			while (p->Nextblock->Nextblock != NULL)                                 //因为多增加了一个节点，而p指向的是最新的舌头节点，所以要多一个Nextblock
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//原尾节点打印“ ”
			Pos(p->Nextblock->x, p->Nextblock->y);
			printf(" ");
			free(p->Nextblock);
			p->Nextblock = NULL;
		}
	}

	if (drt == SNAKE_DOWN)
	{
		NextHead->x = head->x;
		NextHead->y = head->y + 1;

		//如果吃到食物
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//头插法，吃到食物的时候只新建一个节点，并将其插入snake的链表中
			NextHead->Nextblock = head;
			head = NextHead;
			//p的作用是用来遍历链表中的所用节点              
			//如果节点不为空的话，就打印出该节点
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//吃到食物时，加分并且分配新的食物
			Score = Score + Add;
			Create_Food();
		}
		//没有吃到食物
		//遍历所有节点                                                                                \
		蛇头节点转移到新节点NextHead，                                                  \
        剩余节点依次转移到其各自对应的上一个节点                                    \
		在之前最后一个节点的位置打印空格“ ”                                          \
		释放最后一个节点                                                                             \
		依次打印所有节点
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;
		   
			while (p->Nextblock->Nextblock != NULL)                                 //因为多增加了一个节点，而p指向的是最新的舌头节点，所以要多一个Nextblock
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//原尾节点打印“ ”
			Pos(p->Nextblock->x, p->Nextblock->y);
			printf(" ");
			free(p->Nextblock);
			p->Nextblock = NULL;
		}
	}

	if (drt == SNAKE_LEFT)
	{
		NextHead->x = head->x-1;
		NextHead->y = head->y;

		//如果吃到食物
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//头插法，吃到食物的时候只新建一个节点，并将其插入snake的链表中
			NextHead->Nextblock = head;
			head = NextHead;
			//p的作用是用来遍历链表中的所用节点              
			//如果节点不为空的话，就打印出该节点
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//吃到食物时，加分并且分配新的食物
			Score = Score + Add;
			Create_Food();
		}
		//没有吃到食物
		//遍历所有节点                                                                                \
				蛇头节点转移到新节点NextHead，                                                  \
        剩余节点依次转移到其各自对应的上一个节点                                    \
		在之前最后一个节点的位置打印空格“ ”                                          \
		释放最后一个节点                                                                             \
		依次打印所有节点
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;

			while (p->Nextblock->Nextblock != NULL)                                 //因为多增加了一个节点，而p指向的是最新的舌头节点，所以要多一个Nextblock
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//原尾节点打印“ ”
			Pos(p->Nextblock->x, p->Nextblock->y);
			printf(" ");
			free(p->Nextblock);
			p->Nextblock = NULL;
		}
	}

	if (drt == SNAKE_RIGHT)
	{
		NextHead->x = head->x+1;
		NextHead->y = head->y;

		//如果吃到食物
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//头插法，吃到食物的时候只新建一个节点，并将其插入snake的链表中
			NextHead->Nextblock = head;
			head = NextHead;
			//p的作用是用来遍历链表中的所用节点              
			//如果节点不为空的话，就打印出该节点
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//吃到食物时，加分并且分配新的食物
			Score = Score + Add;
			Create_Food();
		}
		//没有吃到食物
		//遍历所有节点                                                                                \
				蛇头节点转移到新节点NextHead，                                                  \
        剩余节点依次转移到其各自对应的上一个节点                                    \
		在之前最后一个节点的位置打印空格“ ”                                          \
		释放最后一个节点                                                                             \
		依次打印所有节点
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;

			while (p->Nextblock->Nextblock != NULL)                                 //因为多增加了一个节点，而p指向的是最新的舌头节点，所以要多一个Nextblock
			{
				Pos(p->x, p->y);
				printf("■");
				p = p->Nextblock;
			}
			//原尾节点打印“ ”
			Pos(p->Nextblock->x, p->Nextblock->y);
			printf(" ");
			free(p->Nextblock);
			p->Nextblock = NULL;
		}
	}
	 
	if (Biteself()==1)
	{
		GameOver_Status == 2;
		Game_Over();
	}
}

void Pause()
{
	while (1)
	{
		Sleep(300);
		if (GetKeyState(VK_SPACE))
		{
			break;
		}
	}
}

int Biteself()
{
	block *SnakeSelf;
	SnakeSelf = head->Nextblock;
	while (SnakeSelf != NULL)
	{
		if (SnakeSelf->x == head->x && SnakeSelf->y == head->y)
		{
			return 1;
		}
		SnakeSelf = SnakeSelf->Nextblock;
	}
}

void HitWall()
{
	if (head->x ==0 || head->x == 56 || head->y == 0 || head->y ==26)
	{
		GameOver_Status = 1;
		Game_Over();
	}
}

void Game_Start()
{
	Create_Map();
	Init_Snake();
	Create_Food();
}
void Game_Charge()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_DOWN) && drt != SNAKE_UP)
		{
			drt = SNAKE_DOWN;
		}
		else if (GetAsyncKeyState(VK_UP) && drt != SNAKE_DOWN)
		{
			drt = SNAKE_UP;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && drt != SNAKE_LEFT)
		{
			drt = SNAKE_RIGHT;
		}
		else if (GetAsyncKeyState(VK_LEFT) && drt != SNAKE_RIGHT)
		{
			drt = SNAKE_LEFT;
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
		else if (GetAsyncKeyState(VK_F1))                                   //press F1 to speed up so sleeptime should be decreased
		{
			if (sleeptime >= 50)
			{
				sleeptime = sleeptime - 30;
				Add = Add + 2;
				if (sleeptime == 320)
				{
					Add = 2;
				}
			}
		}
		else if (GetAsyncKeyState(VK_F2))                                //press F1 to speed up so sleeptime should be increased
		{
			if (sleeptime < 350)
			{
				sleeptime += 30;
				if (sleeptime == 350)
				{
					Add = 1;
				}
			}
		}

		Sleep(sleeptime);
		Snake_Move();
	}
}
void Game_Over()
{
	system("cls");
	Pos(24, 12);
	if (GameOver_Status == 1)
	{
		printf("对不起，您撞到墙了。游戏结束.");
	}
	else if (GameOver_Status == 2)
	{
		printf("对不起，您咬到自己了。游戏结束.");
	}
	else if (GameOver_Status == 3)
	{
		printf("您已经结束了游戏。");
	}
	Pos(24, 13);
	printf("您的得分是%d\n", Score);
	exit(0);
}