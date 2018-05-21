#ifndef _SNAKE_H
#define _SNAKE_H

//����̰����ͷ���ĳ�ʼλ��
#define SNAKE_X 20
#define SNAKE_Y 20

//����ǽ�ĳ�ʼλ�ü�����
//#define SQUARE_X 0
//#define SQUARE_Y 0
//#define SQUARE_L 30
//#define SQUARE_W 26

//����̰�����ƶ��ķ���
#define SNAKE_UP 1
#define SNAKE_DOWN 2
#define SNAKE_RIGHT 3
#define SNAKE_LEFT 4

//̰���ߵľ����ж�
void Pos(int x, int y);                                                           //���λ��
void Create_Map();                                           //�����ͼ
void Init_Snake();                                                //��ʼ��̰����
void Create_Food();                                            //����ʳ��
void Snake_Move();                                            //̰���ߵ��ƶ�
void Pause();                                                       //̰������ͣ
int Biteself();                                                    //̰����ҧ���Լ�
void HitWall();                                                    //̰����ײǽ

 //��Ϸ�ĺ�ۿ���
void Game_Start();
void Game_Charge();
void Game_Over();

#endif