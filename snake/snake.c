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
		printf("��");
		//print bottom
		Pos(i, SQUARE_Y + SQUARE_W);
		printf("��");
	}
	for (i = SQUARE_Y; i <= SQUARE_Y + SQUARE_W; i++)
	{
		//print left
		Pos(SQUARE_X, i);
		printf("��");
		//print right
		Pos(SQUARE_X + SQUARE_L, i);
		printf("��");
	}
	*/
	int i;
	for (i = 0; i<58; i += 2)//��ӡ���±߿�
	{
		Pos(i, 0);
		printf("��");
		Pos(i, 26);
		printf("��");
	}
	for (i = 1; i<26; i++)//��ӡ���ұ߿�
	{
		Pos(0, i);
		printf("��");
		Pos(56, i);
		printf("��");
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
		//ͷ�巨����block��β����ʼ
		head = (block*)malloc(sizeof(block));
		head->Nextblock = tail;
		head->x = tail->x + i * 2;
		head->y = tail->y;
		tail = head;
	}

	while (tail !=NULL)
	{
		Pos(tail->x, tail->y);
		printf("��");
		tail = tail->Nextblock;
	}
	*/
	block *tail;
	int i;
	tail = (block*)malloc(sizeof(block));//����β��ʼ��ͷ�巨����x,y�趨��ʼ��λ��//
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
	while (tail != NULL)//��ͷ��Ϊ���������
	{
		Pos(tail->x, tail->y);
		printf("��");
		tail = tail->Nextblock;
	}
}

void Create_Food()
{
	/*
	block*NewFood;
	srand((unsigned)time(NULL));
	NewFood = (block*)malloc(sizeof(block));
	while ((NewFood->x % 2)!=0)  //��֤��Ϊż����ʹ��ʳ���ܹ�����ͷ����
	{
		NewFood->x = (rand() % (SQUARE_L  - 1)) + SQUARE_X + 1;
	}
	NewFood->y = (rand() % (SQUARE_W - 1)) + SQUARE_Y + 1;

	p = head;
	//������һ�����ʣ�whileѭ��
	while (p->Nextblock == NULL)
	{
		if (p->x == NewFood->x && p->y == NewFood->y) //�ж������Ƿ���ʳ���غ�
		{
			free(NewFood);
			Create_Food();
		}
		p = p->Nextblock;
	}
	Pos(NewFood->x, NewFood->y);
	food = NewFood;
	printf("��");
	*/
	block *NewFood;
	srand((unsigned)time(NULL));
	NewFood = (block*)malloc(sizeof(block));
	while ((NewFood->x % 2) != 0) //��֤��Ϊż����ʹ��ʳ��������ͷ����
	{
		NewFood->x = rand() % 52 + 2;
	}
	NewFood->y = rand() % 24 + 1;
	p = head;
	while (p->Nextblock == NULL)
	{
		if (p->x == NewFood->x && p->y == NewFood->y) //�ж������Ƿ���ʳ���غ�
		{
			free(NewFood);
			Create_Food();
		}
		p = p->Nextblock;
	}
	Pos(NewFood->x, NewFood->y);
	food = NewFood;
	printf("��");
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

		//����Ե�ʳ��
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//ͷ�巨���Ե�ʳ���ʱ��ֻ�½�һ���ڵ㣬���������snake��������
			NextHead->Nextblock = head;
			head = NextHead;
			//p���������������������е����ýڵ�              
			//����ڵ㲻Ϊ�յĻ����ʹ�ӡ���ýڵ�
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//�Ե�ʳ��ʱ���ӷֲ��ҷ����µ�ʳ��
			Score = Score + Add;
			Create_Food();
		}
		//û�гԵ�ʳ��
		//�������нڵ�                                                                                \
		��ͷ�ڵ�ת�Ƶ��½ڵ�NextHead��                                                  \
        ʣ��ڵ�����ת�Ƶ�����Զ�Ӧ����һ���ڵ�                                    \
		��֮ǰ���һ���ڵ��λ�ô�ӡ�ո� ��                                          \
		�ͷ����һ���ڵ�                                                                             \
		���δ�ӡ���нڵ�
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;

			while (p->Nextblock->Nextblock != NULL)                                 //��Ϊ��������һ���ڵ㣬��pָ��������µ���ͷ�ڵ㣬����Ҫ��һ��Nextblock
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//ԭβ�ڵ��ӡ�� ��
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

		//����Ե�ʳ��
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//ͷ�巨���Ե�ʳ���ʱ��ֻ�½�һ���ڵ㣬���������snake��������
			NextHead->Nextblock = head;
			head = NextHead;
			//p���������������������е����ýڵ�              
			//����ڵ㲻Ϊ�յĻ����ʹ�ӡ���ýڵ�
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//�Ե�ʳ��ʱ���ӷֲ��ҷ����µ�ʳ��
			Score = Score + Add;
			Create_Food();
		}
		//û�гԵ�ʳ��
		//�������нڵ�                                                                                \
		��ͷ�ڵ�ת�Ƶ��½ڵ�NextHead��                                                  \
        ʣ��ڵ�����ת�Ƶ�����Զ�Ӧ����һ���ڵ�                                    \
		��֮ǰ���һ���ڵ��λ�ô�ӡ�ո� ��                                          \
		�ͷ����һ���ڵ�                                                                             \
		���δ�ӡ���нڵ�
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;
		   
			while (p->Nextblock->Nextblock != NULL)                                 //��Ϊ��������һ���ڵ㣬��pָ��������µ���ͷ�ڵ㣬����Ҫ��һ��Nextblock
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//ԭβ�ڵ��ӡ�� ��
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

		//����Ե�ʳ��
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//ͷ�巨���Ե�ʳ���ʱ��ֻ�½�һ���ڵ㣬���������snake��������
			NextHead->Nextblock = head;
			head = NextHead;
			//p���������������������е����ýڵ�              
			//����ڵ㲻Ϊ�յĻ����ʹ�ӡ���ýڵ�
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//�Ե�ʳ��ʱ���ӷֲ��ҷ����µ�ʳ��
			Score = Score + Add;
			Create_Food();
		}
		//û�гԵ�ʳ��
		//�������нڵ�                                                                                \
				��ͷ�ڵ�ת�Ƶ��½ڵ�NextHead��                                                  \
        ʣ��ڵ�����ת�Ƶ�����Զ�Ӧ����һ���ڵ�                                    \
		��֮ǰ���һ���ڵ��λ�ô�ӡ�ո� ��                                          \
		�ͷ����һ���ڵ�                                                                             \
		���δ�ӡ���нڵ�
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;

			while (p->Nextblock->Nextblock != NULL)                                 //��Ϊ��������һ���ڵ㣬��pָ��������µ���ͷ�ڵ㣬����Ҫ��һ��Nextblock
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//ԭβ�ڵ��ӡ�� ��
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

		//����Ե�ʳ��
		if (NextHead->x == food->x&&NextHead->y == food->y)
		{
			//ͷ�巨���Ե�ʳ���ʱ��ֻ�½�һ���ڵ㣬���������snake��������
			NextHead->Nextblock = head;
			head = NextHead;
			//p���������������������е����ýڵ�              
			//����ڵ㲻Ϊ�յĻ����ʹ�ӡ���ýڵ�
			p = head;
			while (p != NULL)
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//�Ե�ʳ��ʱ���ӷֲ��ҷ����µ�ʳ��
			Score = Score + Add;
			Create_Food();
		}
		//û�гԵ�ʳ��
		//�������нڵ�                                                                                \
				��ͷ�ڵ�ת�Ƶ��½ڵ�NextHead��                                                  \
        ʣ��ڵ�����ת�Ƶ�����Զ�Ӧ����һ���ڵ�                                    \
		��֮ǰ���һ���ڵ��λ�ô�ӡ�ո� ��                                          \
		�ͷ����һ���ڵ�                                                                             \
		���δ�ӡ���нڵ�
		else
		{
			NextHead->Nextblock = head;
			head = NextHead;
			p = head;

			while (p->Nextblock->Nextblock != NULL)                                 //��Ϊ��������һ���ڵ㣬��pָ��������µ���ͷ�ڵ㣬����Ҫ��һ��Nextblock
			{
				Pos(p->x, p->y);
				printf("��");
				p = p->Nextblock;
			}
			//ԭβ�ڵ��ӡ�� ��
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
		printf("�Բ�����ײ��ǽ�ˡ���Ϸ����.");
	}
	else if (GameOver_Status == 2)
	{
		printf("�Բ�����ҧ���Լ��ˡ���Ϸ����.");
	}
	else if (GameOver_Status == 3)
	{
		printf("���Ѿ���������Ϸ��");
	}
	Pos(24, 13);
	printf("���ĵ÷���%d\n", Score);
	exit(0);
}