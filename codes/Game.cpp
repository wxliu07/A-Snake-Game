#include "Game.h"

#define WIDTH 640		// ������
#define HIGHT 400		// ����߶�
#define SNAKE_MAX 400	// ��������
#define WALL_MAX 10		// ǽ�������

#define	color_SnakeHead	RGB(254, 67, 101)		// ��ͷ��ɫ
#define	color_SnakeBody	RGB(252, 157, 154)		// ������ɫ

// UIͼƬ
IMAGE image_begin, imag_quit_game, imag_fail_game, image_game, image_game2, imag_secret;

// �߽ṹ��, ��num(����), speed(�ٶ�), dir(����), size����(��󳤶�)
struct Snake
{
	int num;					// �ߵĳ���
	int speed;					// �ߵ��ٶ�
	int dir;					// �ߵķ���
	int socre;					// �ߵĵ÷�
	POINT size[SNAKE_MAX];		// �ߵ���󳤶�
}snake;

// ʳ��ṹ�� ��x, y, r,(λ����Ϣ) flag(���ڱ�־), color(��ɫ)
struct Food
{
	int x;					// ʳ��x����
	int y;					// ʳ��y����
	int r;					// ʳ��뾶��С
	bool flag;				// true û�гԵ� false �Ե���
	unsigned long color;	// ��ɫ
}food;

// ǽ�ṹ�� ���Խ������� ��ɫ
struct Wall
{
	int x1, y1, x2, y2;		// ǽ�Խ�����������
	unsigned long color;	// RGB��������ֵΪunsigned long����
};

// ö�ٷ���
enum Dir
{
	UP, DOWN, LEFT, RIGHT
};

Wall	wall[10];

/********************************************���ڲ˵�����*******************************************************/

// �˵�����
void menuInit()
{
	initgraph(WIDTH, HIGHT);
	loadimage(&image_begin, "��ʼ����.png", WIDTH, HIGHT);
	putimage(0, 0, &image_begin);
	setbkmode(TRANSPARENT);
}

// С����
void secret()
{
	loadimage(&imag_secret, "С����.png", WIDTH, HIGHT);
	putimage(0, 0, &imag_secret);

}

// �˳���Ϸ(���)
void gameExit()
{
	initgraph(WIDTH, HIGHT);
	loadimage(&imag_quit_game, "�˳���Ϸ.png", WIDTH, HIGHT);
	putimage(0, 0, &imag_quit_game);
}

// ��Ϸ����(������)
void gameOver()
{
	initgraph(WIDTH, HIGHT);
	loadimage(&imag_fail_game, "��Ϸʧ��.png", WIDTH, HIGHT);
	putimage(0, 0, &imag_fail_game);
}

// ��Ϸ�����Ƿ���ʾ����
bool isShowGrid()
{
	return true;
}

// ���Ƶ�ͼ����
void lineDraw()
{
	if (isShowGrid())							//���Ƶ�ͼ����
	{
		setlinestyle(PS_SOLID, 1, NULL, 0);
		setlinecolor(BLACK);
		// ����
		for (int i = 1; i < HIGHT / 10; i++)
			line(0, 10 * i, 640, 10 * i);
		for (int j = 0; j < WIDTH; j++)
			line(10 * j, 0, 10 * j, 400);
	}
}

// ��ȡ����
int getScore()
{
	return snake.socre;
}

// ��ȡ����
int getLen()
{
	return snake.num;
}

// ��ʾ����
void showScore(int score)
{
	//�����ı���ɫ, ģʽ
	settextstyle(26, 0, _T("Consolas"));//����Ŀ�+����ĸ�+����ķ��
	setbkmode(TRANSPARENT);
	settextcolor(RGB(66, 76, 78));
	char s[80];
	sprintf(s, "%d", score);
	outtextxy(115, 32, s);
}

// ��ʾ����
void showLenth(int len)
{
	//�����ı���ɫ, ģʽ
	settextstyle(26, 0, _T("Consolas"));//����Ŀ�+����ĸ�+����ķ��
	setbkmode(TRANSPARENT);
	settextcolor(RGB(66, 76, 78));
	char s[80];
	sprintf(s, "%d", len);
	outtextxy(115, 50, s);
}

// ��ʾʱ��
void showTime(double snakeTime)
{
	//�����ı���ɫ, ģʽ
	settextstyle(26, 0, _T("Consolas"));//����Ŀ�+����ĸ�+����ķ��
	setbkmode(TRANSPARENT);
	settextcolor(RGB(66, 76, 78));
	char s[80];
	sprintf(s, "%lf", snakeTime);
	outtextxy(115, 68, s);
}


/********************************************������Ϸģʽ*******************************************************/

// ��ʳ��, �÷�(ȡ���ڰ뾶��С1, 2, 3, 4), �����������ʳ��
void Eat()
{
	// �Ƿ�ʳ�������ʳ����ζԽ������������ͷ�Խ�������
	if (food.flag && snake.size[0].x >= food.x - food.r && snake.size[0].x <= food.x + food.r && snake.size[0].y >= food.y - food.r && snake.size[0].y <= food.y + food.r)
	{
		food.flag = false;
		// ʳ��뾶��5 �� 20 ֮��
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

	// ���ʳ��û��, �������������ʳ��
	if (!food.flag)
	{
		food.x = rand() % WIDTH;		//��ʼ��x
		food.y = rand() % HIGHT;		//��ʼ��y
		for (int i = 0; i < 10; i++)
		{
			if (food.x == wall[i].x1 && food.y == wall[i].y1 && food.x + 6 == wall[i].x2 + 6 && food.y + 6 == wall[i].y2 + 6)
			{
				food.r = rand() % 16 + 5;
				food.x = rand() % WIDTH;		//��ʼ��x
				food.y = rand() % HIGHT;		//��ʼ��y
			}
		}
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = rand() % 16 + 5;
		food.flag = true;
	}
}

// ��������
void keyControl()
{
	// �ж����޼�������
	if (_kbhit())
	{
		// ��ȡ��������
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
		case 'e':	// ��ͣ
		case 'E':
			while (_getch() != 'e' && _getch() != 'E');
			break;
		}
	}
}

// �ж��Ƿ����
bool isAcceleration()
{
	// �ж��Ƿ���� VK_SPACEΪ�ո��
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		return true;
	}
	return false;
}

// ��ͨģʽ�ƶ��߲��ж��Ƿ����ƶ���, �����򷵻�false,��֮����true
bool snakeMove1()
{
	// ����Ϣ����
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.size[i] = snake.size[i - 1];
	}

	// �ɷ����ж����Ƿ�����
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
	// �ж����Ƿ���ɱ
	for (int i = snake.num - 1; i > 0; i--)
	{
		if (snake.size[0].x == snake.size[i].x && snake.size[0].y == snake.size[i].y)
			return false;
	}
	return true;
}

// �޾�ģʽ�͹ؿ�ģʽ�ƶ��߲��ж��Ƿ����ƶ���, �����򷵻�false,��֮����true
bool snakeMove2()
{
	// ����Ϣ����
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.size[i] = snake.size[i - 1];
	}

	// ʵ���ߴ�ǽ, ����Ϊ��ͷ���ƶ�
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

	// �ж����Ƿ���ɱ
	for (int i = snake.num - 1; i > 0; i--)
	{
		if (snake.size[0].x == snake.size[i].x && snake.size[0].y == snake.size[i].y)
			return false;
	}

	// �ж��Ƿ�ײǽ
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (snake.size[0].x >= wall[i].x1 && snake.size[0].x <= wall[i].x2 && snake.size[0].y >= wall[i].y1 && snake.size[0].y <= wall[i].y2)
			return false;
	}

	return true;
}

// ��ͨģʽ���޾�ģʽ��ʼ��
void gameInit1()
{
	initgraph(WIDTH, HIGHT);
	BeginBatchDraw();
	//С�߳�ʼ��
	snake.num = 3;		// ����Ϊ3
	snake.speed = 10;	// �ٶ�Ϊ10
	snake.socre = 0;	// �÷�Ϊ0
	snake.dir = LEFT;	// ������

	// ��ʼ����λ����Ϣ
	for (int i = snake.num - 1; i >= 0; i--)
	{
		snake.size[i].x = 10 * i + 400;
		snake.size[i].y = 30;
	}

	// ��ʼ��ʳ��
	food.r = rand() % 16 + 5;		// r�İ뾶��5~20��
	food.x = rand() % WIDTH;		//��ʼ��x
	food.y = rand() % HIGHT;		//��ʼ��y

	// ����ʳ������ڽ���
	if (food.x < food.r) 
		food.x += food.r * 2;
	// ����ʳ���ܳ�ȥ
	if (WIDTH - food.x < food.r) 
		food.x -= food.r * 2;
	if (food.y < food.r) 
		food.y += food.r * 2;
	if (HIGHT - food.y < food.r) 
		food.y -= food.r * 2;

	// ��ʼ�������ɫ
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	
	// ��ʼ��ʳ���־
	food.flag = true;
	EndBatchDraw();
}

// �ؿ�ģʽ��ʼ��
void gameInit2()
{
	initgraph(WIDTH, HIGHT);
	BeginBatchDraw();
	//С�߳�ʼ��
	snake.num = 3;		// ����Ϊ3
	snake.speed = 10;	// �ٶ�Ϊ10
	snake.socre = 0;	// �÷�Ϊ0
	snake.dir = LEFT;	// ������

	// ��ʼ����λ����Ϣ
	for (int i = snake.num - 1; i >= 0; i--)
	{

		snake.size[i].x = 10 * i + 400;
		snake.size[i].y = 30;
	}

	// ��ʼ��ǽ
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
	
	// ��ʼ��ʳ��
	food.r = rand() % 16 + 5;
	food.x = rand() % WIDTH;		//��ʼ��x
	food.y = rand() % HIGHT;		//��ʼ��y

	for (int i = 0; i < 10; i++)
	{
		if (food.x == wall[i].x1 && food.y == wall[i].y1 && food.x + 6 == wall[i].x2 + 6 && food.y + 6 == wall[i].y2 + 6)
		{
			food.r = rand() % 16 + 5;
			food.x = rand() % WIDTH;		//��ʼ��x
			food.y = rand() % HIGHT;		//��ʼ��y
		}
	}

	// ����ʳ������ڽ���
	if (food.x < food.r)
		food.x += food.r * 2;
	// ����ʳ���ܵ���ͼ��
	if (WIDTH - food.x < food.r)
		food.x -= food.r * 2;
	if (food.y < food.r)
		food.y += food.r * 2;
	if (HIGHT - food.y < food.r)
		food.y -= food.r * 2;

	// ��ʼ�������ɫ
	food.color = RGB(rand() % 256, rand() % 256, rand() % 256);

	// ��ʼ��ʳ���־
	food.flag = true;
	EndBatchDraw();
}

// ��ǽ
void wallDraw()
{
	BeginBatchDraw();
	// ���������ɫ
	setfillcolor(RGB(95, 158, 160));
	// ������ǽ
	for (int i = 0; i < WALL_MAX; i++)
	{
		solidrectangle(wall[i].x1, wall[i].y1, wall[i].x2, wall[i].y2);
	}
	EndBatchDraw();
}

// ��ͨģʽ
void gameDraw1()
{
	BeginBatchDraw();
	// ����ͼƬ
	loadimage(&image_game, "��Ϸ����.png", WIDTH, HIGHT);
	putimage(0, 0, &image_game);

	// ����
	setfillcolor(color_SnakeHead);
	solidrectangle(snake.size[0].x, snake.size[0].y, snake.size[0].x + 10, snake.size[0].y + 10);

	setfillcolor(color_SnakeBody);
	for (int i = 1; i < snake.num; i++)
	{
		solidrectangle(snake.size[i].x, snake.size[i].y, snake.size[i].x + 10, snake.size[i].y + 10);
	}

	// ��ʳ��
	setfillcolor(food.color);
	if (food.flag)
		solidcircle(food.x, food.y, food.r);

	
	EndBatchDraw();		//������������, �ں���֮ǰ��ʱ��������浽��Ļ��	
}

// �޾�ģʽ
void gameDraw2()
{
	// ����ͼƬ
	loadimage(&image_game, "��Ϸ����.png", WIDTH, HIGHT);
	putimage(0, 0, &image_game);

	// ����
	setfillcolor(color_SnakeHead);
	solidrectangle(snake.size[0].x, snake.size[0].y, snake.size[0].x + 10, snake.size[0].y + 10);

	setfillcolor(color_SnakeBody);
	for (int i = 1; i < snake.num; i++)
	{
		solidrectangle(snake.size[i].x, snake.size[i].y, snake.size[i].x + 10, snake.size[i].y + 10);
	}

	// ��ʳ��
	setfillcolor(food.color);
	if (food.flag)
		solidcircle(food.x, food.y, food.r);

	EndBatchDraw();					//������������
}

// �ؿ�ģʽ
void gameDraw3()
{
	// ����ͼƬ
	loadimage(&image_game2, "��Ϸ����2.png", WIDTH, HIGHT);
	putimage(0, 0, &image_game2);

	// ��ǽ
	wallDraw();

	// ����
	setfillcolor(color_SnakeHead);
	solidrectangle(snake.size[0].x, snake.size[0].y, snake.size[0].x + 10, snake.size[0].y + 10);

	setfillcolor(color_SnakeBody);
	for (int i = 1; i < snake.num; i++)
	{
		solidrectangle(snake.size[i].x, snake.size[i].y, snake.size[i].x + 10, snake.size[i].y + 10);
	}

	// ��ʳ��
	setfillcolor(food.color);

	if (food.flag)
		solidcircle(food.x, food.y, food.r);

	EndBatchDraw();					//������������
}

/*************************************************************************************************************/