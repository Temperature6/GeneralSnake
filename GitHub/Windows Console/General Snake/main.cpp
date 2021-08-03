#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include "function.h"
#include "define.h"


struct SNAKE
{
	int x;
	int y;
}snake;
SNAKE food;

SNAKE body[XMAX * YMAX];
int map[YMAX + 1][XMAX + 1] = { 0 }; //地图
int end = 2; //蛇身结束索引
int dirt = U;
int score = 0; //分数
bool eat = false;
int delay_ms = 200;

void SnakeMove(bool gotted=false);
void InitGame();
void GenerateFood();
void BiteSelf();
void HitWall();
bool EatFood(bool judge = 1);
void Run();

int main()
{
	ShowOutline();
	InitGame();
	Run();
}

void Run()
{
	while (true)
	{
		int eat = false;
		dirt = Contrl(dirt);
		BiteSelf();
		HitWall();
		SnakeMove(EatFood());
		Sleep(delay_ms);
	}
}

void SnakeMove(bool gotted)
{
	//将上一个块的坐标赋值给下一个块
	if (!gotted) //如果吃到了食物，就延长蛇身(不再清除蛇尾巴)
	{
		Dot(body[end].x, body[end].y, 0);
	}
	for (int i = end; i >= 1; i--)
	{
		body[i] = body[i - 1];
	}
	if (dirt == U)
	{
		body[0].y--;
		Dot(body[0].x, body[0].y, 1);
	}
	else if (dirt == D)
	{
		body[0].y++;
		Dot(body[0].x, body[0].y, 1);
	}
	else if (dirt == L)
	{
		body[0].x--;
		Dot(body[0].x, body[0].y, 1);
	}
	else if (dirt == R)
	{
		body[0].x++;
		Dot(body[0].x, body[0].y, 1);
	}
}

void InitGame()
{
	int _x = XMAX / 2;
	int _y = YMAX / 2;
	for (int i = 0; i <= end; i++)
	{
		body[i].x = _x;
		body[i].y = _y + i;
		Dot(_x, _y + i, 1);
	}
	GenerateFood();
	ShowScore(score);
}

void GenerateFood()
{
	int _x, _y;
	srand((unsigned)time(NULL));
	while (true)
	{
		_x = rand() % XMAX;
		_y = rand() % YMAX;

		if (map[_y][_x] == 1)
		{
			continue;
		}
		else
		{
			break;
		}
	}
	//_x = 20;
	//_y = 8;
	food.x = _x;
	food.y = _y;
	Dot(_x, _y, 1, 0);
	map[_y][_x] = 2;
}

void BiteSelf()
{
	if (dirt == U && map[body[0].y - 1][body[0].x] == 1)
	{
		//printf("撞到自己上");
		EndGame();
	}
	else if (dirt == D && map[body[0].y + 1][body[0].x] == 1)
	{
		//printf("撞到自己下");
		EndGame();
	}
	else if (dirt == L && map[body[0].y][body[0].x - 1] == 1)
	{
		//printf("撞到自己左");
		EndGame();
	}
	else if (dirt == R && map[body[0].y][body[0].x + 1] == 1)
	{
		//printf("撞到自己右");
		EndGame();
	}
	return;
}

void HitWall()
{
	if (body[0].x <= -1 || body[0].x >= XMAX + 1 || body[0].y <= -1 || body[0].y >= YMAX + 1)
	{
		//printf("撞到墙");
		EndGame();
	}
}

bool EatFood(bool judge)
{
	if (judge && dirt == U && map[body[0].y - 1][body[0].x] == 2)
	{
		EatFood(0); //吃到食物，跳过卡面的判断，执行加分
	}
	else if (judge && dirt == D && map[body[0].y + 1][body[0].x] == 2)
	{
		EatFood(0);
	}
	else if (judge && dirt == L && map[body[0].y][body[0].x - 1] == 2)
	{
		EatFood(0);
	}
	else if (judge && dirt == R && map[body[0].y][body[0].x + 1] == 2)
	{
		EatFood(0);
	}
	else if (judge == 0)
	{
		end++;
		score++;
		Dot(food.x, food.y, 0, 0);
		GenerateFood();
		ShowScore(score);
		return true;
	}
	else
	{
		return false;
	}
}
