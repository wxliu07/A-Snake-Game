#include "Game.h"

#define WIDTH 640		// 界面宽度
#define HIGHT 400		// 界面高度
#define SNAKE_MAX 400	// 蛇最大节数
#define WALL_MAX 10		// 墙最大数量

#define	color_SnakeHead	RGB(254, 67, 101)		// 蛇头颜色
#define	color_SnakeBody	RGB(252, 157, 154)		// 蛇身颜色

// UI图片
IMAGE image_begin, imag_quit_game, imag_fail_game, image_game, image_game2, imag_secret;

// 蛇结构体, 含num(节数), speed(速度), dir(方向), size数组(最大长度)
struct Snake
{
	int num;					// 蛇的长度
	int speed;					// 蛇的速度
	int dir;					// 蛇的方向
	int socre;					// 蛇的得分
	POINT size[SNAKE_MAX];		// 蛇的最大长度
}snake;

// 食物结构体 含x, y, r,(位置信息) flag(存在标志), color(颜色)
struct Food
{
	int x;					// 食物x坐标
	int y;					// 食物y坐标
	int r;					// 食物半径大小
	bool flag;				// true 没有吃掉 false 吃掉了
	unsigned long color;	// 颜色
}food;

// 墙结构体 含对角线坐标 颜色
struct Wall
{
	int x1, y1, x2, y2;		// 墙对角线两点坐标
	unsigned long color;	// RGB函数返回值为unsigned long类型
};

// 枚举方向
enum Dir
{
	UP, DOWN, LEFT, RIGHT
};

Wall	wall[10];

/********************************************关于菜单界面*******************************************************/

// 菜单界面
void menuInit()
{
	initgraph(WIDTH, HIGHT);
	loadimage(&image_begin, "开始界面.png", WIDTH, HIGHT);
	putimage(0, 0, &image_begin);
	setbkmode(TRANSPARENT);
}

// 小秘密
void secret()
{
	loadimage(&imag_secret, "小秘密.png", WIDTH, HIGHT);
	putimage(0, 0, &imag_secret);

}

// 退出游戏(玩家)
void gameExit()
{
	initgraph(WIDTH, HIGHT);
	loadimage(&imag_quit_game, "退出游戏.png", WIDTH, HIGHT);
	putimage(0, 0, &imag_quit_game);
}

// 游戏结束(蛇死亡)
void gameOver()
{
	initgraph(WIDTH, HIGHT);
	loadimage(&imag_fail_game, "游戏失败.png", WIDTH, HIGHT);
	putimage(0, 0, &imag_fail_game);
}

// 游戏界面是否显示网格
bool isShowGrid()
{
	return true;
}

// 绘制地图网格
void lineDraw()
{
	if (isShowGrid())							//绘制地图网格
	{
		setlinestyle(PS_SOLID, 1, NULL, 0);
		setlinecolor(BLACK);
		// 画行
		for (int i = 1; i < HIGHT / 10; i++)
			line(0, 10 * i, 640, 10 * i);
		for (int j = 0; j < WIDTH; j++)
			line(10 * j, 0, 10 * j, 400);
	}
}

// 获取分数
int getScore()
{
	return snake.socre;
}

// 获取长度
int getLen()
{
	return snake.num;
}

// 显示分数
void showScore(int score)
{
	//设置文本颜色, 模式
	settextstyle(26, 0, _T("Consolas"));//字体的宽+字体的高+字体的风格
	setbkmode(TRANSPARENT);
	settextcolor(RGB(66, 76, 78));
	char s[80];
	sprintf(s, "%d", score);
	outtextxy(115, 32, s);
}

// 显示长度
void showLenth(int len)
{
	//设置文本颜色, 模式
	settextstyle(26, 0, _T("Consolas"));//字体的宽+字体的高+字体的风格
	setbkmode(TRANSPARENT);
	settextcolor(RGB(66, 76, 78));
	char s[80];
	sprintf(s, "%d", len);
	outtextxy(115, 50, s);
}

// 显示时间
void showTime(double snakeTime)
{
	//设置文本颜色, 模式
	settextstyle(26, 0, _T("Consolas"));//字体的宽+字体的高+字体的风格
	setbkmode(TRANSPARENT);
	settextcolor(RGB(66, 76, 78));
	char s[80];
	sprintf(s, "%lf", snakeTime);
	outtextxy(115, 68, s);
}


/********************************************关于游戏模式*******************************************************/

// 吃食物, 得分(取决于半径大小1, 2, 3, 4), 重新随机生成食物
void Eat()
{
	// 是否食物存在且食物矩形对角线坐标等于蛇头对角线坐标
	if (food.flag && snake.size[0].x >= food.x - food.r && snake.size[0].x <= food.x + food.r && snake.size[0].y >= food.y - food.r && snake.size[0].y <= food.y + food.r)
	{
		food.flag = false;
		// 食物半径在5 到 20 之间
		switch (food.r / 5)
		{
		case 1:
			snake.num += 1;
			snake.socre += 10;
			break;
		case 2:
			snake.num += 2;
			snake.socre += 20;
			break;
		case 3:
			snake.num += 3;
			snake.socre += 30;
			break;
		case 4:
			snake.num += 4;
			snake.socre += 40;
		default:
			break;
		}
	}

	// 如果食物没了, 则重新随机生成食物
	if (!food.flag)
	{
		food.x = rand() % WIDTH;		//初始化x
		food.y = rand() % HIGHT;		//初始化y
		for (int i = 0; i < 10; i++)
		{
			if (food.x == wall[i].x1 && food.y == wall[i].y1 && food.x + 6 == wall[i].x2 + 6 && food.y + 6 == wall[i].y2 + 6)
			{
				food.r = rand() % 16 + 5;
				food.x = rand() % WIDTH;		//初始化x
				food.y = rand() % HIGHT;		//初始化y
			}
		}
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 16 + 5;
		food.flag = true;
	}
}

// 按键交互
void keyControl()
{
	// 判断有无键盘输入
	if (_kbhit())
	{
		// 获取键盘输入
		switch (_getch())
		{
		case 'W':
		case 'w':
			if (snake.dir != DOWN)
				snake.dir = UP;
			break;
		case 'S':
		case 's':
			if (snake.dir != UP)
				snake.dir = DOWN;
			break;
		case 'A':
		case 'a':
			if (snake.dir != RIGHT)
				snake.dir = LEFT;
			break;
		case 'D':
		case 'd':
			if (snake.dir != LEFT)
				snake.dir = RIGHT;
			break;
		case 'e':	// 暂停
		case 'E':
			while (_getch() != 'e' && _getch() != 'E');
			break;
		}
	}
}

// 判断是否加速
bool isAcceleration()
{
	// 判断是否加速 VK_SPACE为空格键
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		return true;
	}
	return false;
}

// 普通模式移动蛇并判断是否能移动蛇, 不能则返回false,反之返回true
bool snakeMove1()
{
	// 蛇信息更新
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.size[i] = snake.size[i - 1];
	}

	// 由方向判断蛇是否死亡
	switch (snake.dir)
	{
	case UP:
		snake.size[0].y -= snake.speed;
		if (snake.size[0].y <= 0)
			return false;
		break;
	case DOWN:
		snake.size[0].y += snake.speed;
		if (snake.size[0].y >= HIGHT)
			return false;
		break;
	case LEFT:
		snake.size[0].x -= snake.speed;
		if (snake.size[0].x <= 0)
			return false;
		break;
	case RIGHT:
		snake.size[0].x += snake.speed;
		if (snake.size[0].x >= WIDTH)
			return false;
		break;
	}
	// 判断蛇是否自杀
	for (int i = snake.num - 1; i > 0; i--)
	{
		if (snake.size[0].x == snake.size[i].x && snake.size[0].y == snake.size[i].y)
			return false;
	}
	return true;
}

// 无尽模式和关卡模式移动蛇并判断是否能移动蛇, 不能则返回false,反之返回true
bool snakeMove2()
{
	// 蛇信息更新
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.size[i] = snake.size[i - 1];
	}

	// 实现蛇穿墙, 本质为蛇头的移动
	switch (snake.dir)
	{
	case UP:
		snake.size[0].y -= snake.speed;
		if (snake.size[0].y <= 0)
			snake.size[0].y = HIGHT;
		break;
	case DOWN:
		snake.size[0].y += snake.speed;
		if (snake.size[0].y >= HIGHT)
			snake.size[0].y = 0;
		break;
	case LEFT:
		snake.size[0].x -= snake.speed;
		if (snake.size[0].x <= 0)
			snake.size[0].x = WIDTH;
		break;
	case RIGHT:
		snake.size[0].x += snake.speed;
		if (snake.size[0].x >= WIDTH)
			snake.size[0].x = 0;
		break;
	}

	// 判断蛇是否自杀
	for (int i = snake.num - 1; i > 0; i--)
	{
		if (snake.size[0].x == snake.size[i].x && snake.size[0].y == snake.size[i].y)
			return false;
	}

	// 判断是否撞墙
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (snake.size[0].x >= wall[i].x1 && snake.size[0].x <= wall[i].x2 && snake.size[0].y >= wall[i].y1 && snake.size[0].y <= wall[i].y2)
			return false;
	}

	return true;
}

// 普通模式和无尽模式初始化
void gameInit1()
{
	initgraph(WIDTH, HIGHT);
	BeginBatchDraw();
	//小蛇初始化
	snake.num = 3;		// 节数为3
	snake.speed = 10;	// 速度为10
	snake.socre = 0;	// 得分为0
	snake.dir = LEFT;	// 方向朝左

	// 初始化蛇位置信息
	for (int i = snake.num - 1; i >= 0; i--)
	{
		snake.size[i].x = 10 * i + 400;
		snake.size[i].y = 30;
	}

	// 初始化食物
	food.r = rand() % 16 + 5;		// r的半径在5~20内
	food.x = rand() % WIDTH;		//初始化x
	food.y = rand() % HIGHT;		//初始化y

	// 避免食物出现在角落
	if (food.x < food.r) 
		food.x += food.r * 2;
	// 避免食物跑出去
	if (WIDTH - food.x < food.r) 
		food.x -= food.r * 2;
	if (food.y < food.r) 
		food.y += food.r * 2;
	if (HIGHT - food.y < food.r) 
		food.y -= food.r * 2;

	// 初始化随机颜色
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	
	// 初始化食物标志
	food.flag = true;
	EndBatchDraw();
}

// 关卡模式初始化
void gameInit2()
{
	initgraph(WIDTH, HIGHT);
	BeginBatchDraw();
	//小蛇初始化
	snake.num = 3;		// 节数为3
	snake.speed = 10;	// 速度为10
	snake.socre = 0;	// 得分为0
	snake.dir = LEFT;	// 方向朝左

	// 初始化蛇位置信息
	for (int i = snake.num - 1; i >= 0; i--)
	{

		snake.size[i].x = 10 * i + 400;
		snake.size[i].y = 30;
	}

	// 初始化墙
	wall[0] = { 60,0,80,100 };
	wall[1] = { 80,80,200,100 };
	wall[2] = { 180,100,200,200 };
	wall[3] = { 200,180,320,200 };
	wall[4] = { 0,180,80,200 };
	wall[5] = { 60,280,400,300 };
	wall[6] = { 280,80,480,100 };
	wall[7] = { 460,100,480,200 };
	wall[8] = { 540,0,560,200 };
	wall[9] = { 540,200,560,400 };
	
	// 初始化食物
	food.r = rand() % 16 + 5;
	food.x = rand() % WIDTH;		//初始化x
	food.y = rand() % HIGHT;		//初始化y

	for (int i = 0; i < 10; i++)
	{
		if (food.x == wall[i].x1 && food.y == wall[i].y1 && food.x + 6 == wall[i].x2 + 6 && food.y + 6 == wall[i].y2 + 6)
		{
			food.r = rand() % 16 + 5;
			food.x = rand() % WIDTH;		//初始化x
			food.y = rand() % HIGHT;		//初始化y
		}
	}

	// 避免食物出现在角落
	if (food.x < food.r)
		food.x += food.r * 2;
	// 避免食物跑到地图外
	if (WIDTH - food.x < food.r)
		food.x -= food.r * 2;
	if (food.y < food.r)
		food.y += food.r * 2;
	if (HIGHT - food.y < food.r)
		food.y -= food.r * 2;

	// 初始化随机颜色
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);

	// 初始化食物标志
	food.flag = true;
	EndBatchDraw();
}

// 画墙
void wallDraw()
{
	BeginBatchDraw();
	// 设置填充颜色
	setfillcolor(RGB(95, 158, 160));
	// 遍历画墙
	for (int i = 0; i < WALL_MAX; i++)
	{
		solidrectangle(wall[i].x1, wall[i].y1, wall[i].x2, wall[i].y2);
	}
	EndBatchDraw();
}

// 普通模式
void gameDraw1()
{
	BeginBatchDraw();
	// 加载图片
	loadimage(&image_game, "游戏界面.png", WIDTH, HIGHT);
	putimage(0, 0, &image_game);

	// 画蛇
	setfillcolor(color_SnakeHead);
	solidrectangle(snake.size[0].x, snake.size[0].y, snake.size[0].x + 10, snake.size[0].y + 10);

	setfillcolor(color_SnakeBody);
	for (int i = 1; i < snake.num; i++)
	{
		solidrectangle(snake.size[i].x, snake.size[i].y, snake.size[i].x + 10, snake.size[i].y + 10);
	}

	// 画食物
	setfillcolor(food.color);
	if (food.flag)
		solidcircle(food.x, food.y, food.r);

	
	EndBatchDraw();		//结束批量绘制, 在函数之前暂时不输出画面到屏幕上	
}

// 无尽模式
void gameDraw2()
{
	// 加载图片
	loadimage(&image_game, "游戏界面.png", WIDTH, HIGHT);
	putimage(0, 0, &image_game);

	// 画蛇
	setfillcolor(color_SnakeHead);
	solidrectangle(snake.size[0].x, snake.size[0].y, snake.size[0].x + 10, snake.size[0].y + 10);

	setfillcolor(color_SnakeBody);
	for (int i = 1; i < snake.num; i++)
	{
		solidrectangle(snake.size[i].x, snake.size[i].y, snake.size[i].x + 10, snake.size[i].y + 10);
	}

	// 画食物
	setfillcolor(food.color);
	if (food.flag)
		solidcircle(food.x, food.y, food.r);

	EndBatchDraw();					//结束批量绘制
}

// 关卡模式
void gameDraw3()
{
	// 加载图片
	loadimage(&image_game2, "游戏界面2.png", WIDTH, HIGHT);
	putimage(0, 0, &image_game2);

	// 画墙
	wallDraw();

	// 画蛇
	setfillcolor(color_SnakeHead);
	solidrectangle(snake.size[0].x, snake.size[0].y, snake.size[0].x + 10, snake.size[0].y + 10);

	setfillcolor(color_SnakeBody);
	for (int i = 1; i < snake.num; i++)
	{
		solidrectangle(snake.size[i].x, snake.size[i].y, snake.size[i].x + 10, snake.size[i].y + 10);
	}

	// 画食物
	setfillcolor(food.color);

	if (food.flag)
		solidcircle(food.x, food.y, food.r);

	EndBatchDraw();					//结束批量绘制
}

/*************************************************************************************************************/