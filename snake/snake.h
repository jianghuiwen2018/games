#ifndef _SNAKE_H
#define _SNAKE_H

//定义贪吃蛇头部的初始位置
#define SNAKE_X 20
#define SNAKE_Y 20

//定义墙的初始位置及长度
//#define SQUARE_X 0
//#define SQUARE_Y 0
//#define SQUARE_L 30
//#define SQUARE_W 26

//定义贪吃蛇移动的方向
#define SNAKE_UP 1
#define SNAKE_DOWN 2
#define SNAKE_RIGHT 3
#define SNAKE_LEFT 4

//贪吃蛇的具体行动
void Pos(int x, int y);                                                           //光标位置
void Create_Map();                                           //创造地图
void Init_Snake();                                                //初始化贪吃蛇
void Create_Food();                                            //创造食物
void Snake_Move();                                            //贪吃蛇的移动
void Pause();                                                       //贪吃蛇暂停
int Biteself();                                                    //贪吃蛇咬到自己
void HitWall();                                                    //贪吃蛇撞墙

 //游戏的宏观控制
void Game_Start();
void Game_Charge();
void Game_Over();

#endif