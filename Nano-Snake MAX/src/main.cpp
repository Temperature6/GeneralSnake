#include <Arduino.h>
#include <SPI.h>
#include <time.h>
#include <define.h>
#include <MaxMatrix.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int CLK = 7; // CLK pin of MAX7219 module
int CS = 6;  // CS pin of MAX7219 module
int DIN = 5; // DIN pin of MAX7219 module
MaxMatrix m(DIN, CS, CLK, 1);

byte end_pic[]={8,8,B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000,B00000000};

struct SNAKE
{
	int8_t x;
	int8_t y;
}snake;
SNAKE food;

SNAKE body[XMAX * YMAX];
int8_t game_map[YMAX + 1][XMAX + 1]; //地图
int8_t end = 0; //蛇身结束索引
int8_t dirt = R;
int score = 0; //分数
bool eat = false;
int delay_ms = 700;
int vx, vy;

void SnakeMove(bool gotted=false);
void InitGame();
void GenerateFood();
void BiteSelf();
void HitWall();
bool EatFood(bool judge = 1);
void Run();
void SetCursor(int8_t x, int8_t y);
void Dot(int8_t x, int8_t y, bool show, bool isSnake = 1);
int8_t Contrl(int8_t current);
void ShowOutline();
void EndGame();
void ShowScore(int score);

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  m.init();
  m.setIntensity(1);
  lcd.init();
  lcd.backlight();
}

void loop() {
	InitGame();
	Run();
}

void Run()
{
	while (true)
	{
		dirt = Contrl(dirt);
		BiteSelf();
		HitWall();
		SnakeMove(EatFood());
		delay(delay_ms);
	}
}

void SnakeMove(bool gotted)
{
	//将上一个块的坐标赋值给下一个块
	if (!gotted) //如果吃到了食物，就延长蛇身(不再清除蛇尾巴)
	{
		Dot(body[end].x, body[end].y, 0);
	}
	for (int8_t i = end; i >= 1; i--)
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
	int8_t _x = XMAX / 2;
	int8_t _y = YMAX / 2;
	for (int8_t i = 0; i <= end; i++)
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
	int8_t _x, _y;
	while (true)
	{
    _x = random(XMAX);
    _y = random(YMAX);

		if (game_map[_y][_x] == 1)
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
	game_map[_y][_x] = 2;
}

void BiteSelf()
{
	if (dirt == U && game_map[body[0].y - 1][body[0].x] == 1)
	{
		//printf("撞到自己上");
		EndGame();
    return;
	}
	else if (dirt == D && game_map[body[0].y + 1][body[0].x] == 1)
	{
		//printf("撞到自己下");
		EndGame();
    return;
	}
	else if (dirt == L && game_map[body[0].y][body[0].x - 1] == 1)
	{
		//printf("撞到自己左");
		EndGame();
    return;
	}
	else if (dirt == R && game_map[body[0].y][body[0].x + 1] == 1)
	{
		//printf("撞到自己右");
		EndGame();
    return;
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
	if (judge && dirt == U && game_map[body[0].y - 1][body[0].x] == 2)
	{
		EatFood(0);
		return true;
	}
	else if (judge && dirt == D && game_map[body[0].y + 1][body[0].x] == 2)
	{
		EatFood(0);
		return true;
	}
	else if (judge && dirt == L && game_map[body[0].y][body[0].x - 1] == 2)
	{
		EatFood(0);
		return true;
	}
	else if (judge && dirt == R && game_map[body[0].y][body[0].x + 1] == 2)
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

void Dot(int8_t x, int8_t y, bool show, bool isSnake)
{
	//screen.setCursor(x, y);

	if (show)
	{
		m.setDot(x, y, 1);
		if (isSnake)
		{
			game_map[y][x] = 1;
		}
	}
	else
	{
		m.setDot(x, y, 0);
		if (isSnake)
		{
			game_map[y][x] = 0;
		}
	}
	return;
}

int8_t Contrl(int8_t current)
{
  vx = analogRead(A0);
  vy = analogRead(A1);

  if (vy > 874 && dirt != L)
  {
    return R;
  }
  else if (vy < 150 && dirt != R)
  {
    return L;
  }
  else if (vx < 150 && dirt != U)
  {
    return D;
  }
  else if (vx > 874 && dirt != D)
  {
    return U;
  }
  return dirt;
}

void ShowOutline()
{
	for (int8_t i = 0; i <= XMAX + 1; i++)
	{
		Dot(i, YMAX + 1, 1, 0);
	}
	for (int8_t j = 0; j <= YMAX + 1; j++)
	{
		Dot(XMAX + 1, j, 1, 0);
	}
	return;
}

void EndGame()
{
	m.writeSprite(0, 0, end_pic);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Game Over!");
	lcd.setCursor(0, 1);
	lcd.print("Your score:");
	lcd.print(score);
	while(true)
	{

	}
}

void ShowScore(int score)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Score:");
	lcd.print(score);
}