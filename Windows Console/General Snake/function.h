#pragma once
#include <Windows.h>
#include <iostream>
#include "define.h"

extern int map[YMAX + 1][XMAX + 1];

void SetCursor(int x, int y)
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void Dot(int x, int y, bool show, bool isSnake = 1)
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);

	if (show)
	{
		std::cout << BLOCK;
		if (isSnake)
		{
			map[y][x] = 1;
		}
	}
	else
	{
		std::cout << " ";
		if (isSnake)
		{
			map[y][x] = 0;
		}
	}

	return;
}

int Contrl(int current)
{
	//返回0表示无按键
	//保证不会反方向
	if (GetAsyncKeyState(VK_UP) && current != D)
	{
		return U;
	}
	else if (GetAsyncKeyState(VK_DOWN) && current != U)
	{
		return D;
	}
	else if (GetAsyncKeyState(VK_LEFT) && current != R)
	{
		return L;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && current != L)
	{
		return R;
	}
	else
	{
		return current;
	}
}

void ShowOutline()
{
	for (int i = 0; i <= XMAX + 1; i++)
	{
		Dot(i, YMAX + 1, 1, 0);
	}
	for (int j = 0; j <= YMAX + 1; j++)
	{
		Dot(XMAX + 1, j, 1, 0);
	}
	return;
}

void EndGame()
{
	Dot(XMAX + 5, YMAX, 0);
	printf("______输了______");
	system("pause");
	exit(1);
}

void ShowScore(int score)
{
	SetCursor(XMAX, YMAX + 5);
	printf("当前分数:%d", score);
}
