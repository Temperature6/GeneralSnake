#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

Adafruit_SSD1306 screen(128, 64, &Wire, 4);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define U 1
#define D 2
#define L 3 
#define R 4

typedef struct SNAKE
{
	int x;
	int y;
	struct SNAKE *next;
}snake;

int vx;
int vy;
int score = 0, add = 1;
int status, sleeptime = 0;
snake *head, *food;
snake *q;
int endGamestatus = 0;

void Pos(int x,int y);
void initSnake();
int biteSelf();
void createFood();
int testfood(snake *food_1);
void cantCrossWall();
void snakeMove();
void pause();
void runGame();
void endGame();
void gameStart();

void setup()
{
	lcd.init();
	lcd.backlight();
	screen.begin(SSD1306_SWITCHCAPVCC, 0x3c);
	screen.clearDisplay();
}

void loop()
{
	gameStart();
	runGame();
	endGame();
}

void Pos(int x,int y)
{
	screen.setCursor(x, y);
}

void initSnake()
{
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，以x,y设定开始的位置//
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i <= 4; i++)//初始长度为4
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + i;
		head->y = 5;
		tail = head;
	}
	while (tail != NULL)//从头到为，输出蛇身
	{
		screen.drawPixel(tail->x, tail->y, WHITE);
		screen.display();
		tail = tail->next;
	}
}

int biteSelf()
{
	snake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			return 1;
		}
		self = self->next;
	}
	return 0;
}

void createFood()
{
	int result;
	snake *food_1;
	food_1 = (snake*)malloc(sizeof(snake));
	do {
		food_1->x = random(128);
		food_1->y = random(64);
		result = testfood(food_1);
	} while (result==1);
	food = food_1;
	screen.drawPixel(food_1->x, food_1->y, WHITE);
	screen.display();
}

int testfood(snake *food_1) {
	
	q = head;
	while (q->next != NULL) {
		if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
		{
			return 1;
			
		}
		q = q->next;
	}return 0;
	}

void cantCrossWall()
{
	if (head->x == 0 || head->x == 128 || head->y == 0 || head->y == 64)
	{
		endGamestatus = 1;
		endGame();
	}
}

void snakeMove()
{
	snake * nexthead;
	cantCrossWall();

	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U)
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个有食物//
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			score = score + add;
			createFood();
		}
		else                                               //如果没有食物//
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			screen.drawPixel(q->next->x, q->next->y, BLACK);
			screen.display();
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == D)
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		if (nexthead->x == food->x && nexthead->y == food->y)  //有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			score = score + add;
			createFood();
		}
		else                               //没有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			screen.drawPixel(q->next->x, q->next->y, BLACK);
			screen.display();
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == L)
	{
		nexthead->x = head->x - 1;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			score = score + add;
			createFood();
		}
		else                                //没有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			screen.drawPixel(q->next->x, q->next->y, BLACK);
			screen.display();
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == R)
	{
		nexthead->x = head->x + 1;
		nexthead->y = head->y;
		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			score = score + add;
			createFood();
		}
		else                                         //没有食物
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				screen.drawPixel(q->x, q->y, WHITE);
				screen.display();
				q = q->next;
			}
			screen.drawPixel(q->next->x, q->next->y, BLACK);
			screen.display();
			free(q->next);
			q->next = NULL;
		}
	}
	if (biteSelf() == 1)       //判断是否会咬到自己
	{
		endGamestatus = 2;
		endGame();
	}
}

void runGame()  
{
	status = R;
	while (1)
	{
		lcd.setCursor(0, 0);
		lcd.print("Score:");
		lcd.print(score);
		
		vx = analogRead(A0);
		vy = analogRead(A1);

		if (vy > 874 && status != L)
		{
			status = R;
		}
		else if (vy < 150 && status != R)
		{
			status = L;
		}
		else if (vx < 150 && status != U)
		{
			status = D;
		}
		else if (vx > 874 && status != D)
		{
			status = U;
		}
		delay(sleeptime);
		snakeMove();
	}
}

void endGame()
{
	screen.clearDisplay();
	screen.fillScreen(WHITE);
	screen.display();
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

void gameStart()
{
	initSnake();
	createFood();
}
