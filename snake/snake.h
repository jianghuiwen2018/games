#ifndef _SNAKE_H
#define _SNAKE_H

//贪吃蛇移动的方向
#define SNAKE_UP  1
#define SNAKE_DOWN  2
#define SNAKE_LEFT  3 
#define SNAKE_RIGHT  4 

//声明全部函数//
void Pos();
void Create_Map();
void Init_Snake();
int BiteSelf();
void Create_Food();
void HitWall();
void Snake_Move();
void Pause();
void SetColor();
void Set_Size_Coord();
void Game_Charge();
void Welcome();
void Game_Over();
void Game_Start();
//void Game_Replay();
void Game_Diy();
void Game_Score();
#endif // !_SNAKE_H
