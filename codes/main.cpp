#include "Game.h"

int main()
{
	// 随机数种子
	srand((unsigned int)time(NULL));

again:
	menuInit();
	MOUSEMSG m;
	while (true)
	{
		// 打开音乐文件并自定义别称
		mciSendString(TEXT("open GameMusic.mp3"), NULL, 0, NULL);

		// 获取鼠标信息
		m = GetMouseMsg();

		// 打开音乐
		if (m.x >= 300 && m.x <= 340 && m.y >= 350 && m.y <= 380 && m.uMsg == WM_LBUTTONDOWN)		//打开音乐
		{
			mciSendString("play GameMusic.mp3", NULL, 0, NULL);
			MOUSEMSG m2;
			m2 = GetMouseMsg();
			if (m2.x >= 300 && m2.x <= 340 && m2.y >= 350 && m.y <= 380 && m.uMsg == WM_LBUTTONDOWN)		//打开音乐
				mciSendString(TEXT("close music1"), NULL, 0, NULL);

		}

		// 小秘密
		if (m.x >= 500 && m.x <= 540 && m.y >= 340 && m.y <= 380 && m.uMsg == WM_LBUTTONDOWN)		//打开音乐
		{
			secret();
			MOUSEMSG m1;
			while (1)
			{
				m1 = GetMouseMsg();
				if (m1.x >= 34 && m1.x <= 63 && m1.y >= 360 && m1.y <= 390 && m1.uMsg == WM_LBUTTONDOWN)
				{
					goto again;
				}
			}
		}

		// 普通模式
		if (m.x >= 250 && m.x <= 370 && m.y >= 170 && m.y <= 200 && m.uMsg == WM_LBUTTONDOWN)		//普通模式
		{
			gameInit1();
			lineDraw();
			while (true)
			{
				BeginBatchDraw();
				gameDraw1();
				if (!snakeMove1()) goto over;
				keyControl();
				Eat();
				int score = getScore();
				showScore(score);

				int len = getLen();
				showLenth(len);
				if (isAcceleration())
					Sleep(30);
				else
					Sleep(90);
				EndBatchDraw();
			}
		}

		// 无尽模式
		else if (m.x >= 460 && m.x <= 570 && m.y >= 170 && m.y <= 200 && m.uMsg == WM_LBUTTONDOWN)	//无尽模式
		{
			gameInit1();
			clock_t start, end;
			start = clock();
			while (true)
			{
				BeginBatchDraw();
				gameDraw2();
				if (!snakeMove2())
					goto over;
				keyControl();
				Eat();
				int score = getScore();
				showScore(score);

				int len = getLen();
				showLenth(len);
				if (isAcceleration())
					Sleep(30);
				else
					Sleep(90);
				end = clock();
				EndBatchDraw();
				//double mytime = ((double)end - (double)start) / CLOCKS_PER_SEC;
				//showTime(mytime);
			}
		}

		// 关卡模式
		else if (m.x >= 250 && m.x <= 370 && m.y >= 250 && m.y <= 300 && m.uMsg == WM_LBUTTONDOWN)	//障碍模式
		{
			gameInit2();
			while (true)
			{
				BeginBatchDraw();
				gameDraw3();
				if (!snakeMove2()) goto over;
				keyControl();
				Eat();
				int score = getScore();
				showScore(score);
				Sleep(60);
				EndBatchDraw();
			}
		}


		// 退出游戏
		else if (m.x >= 460 && m.x <= 570 && m.y >= 250 && m.y <= 290 && m.uMsg == WM_LBUTTONDOWN)	//退出游戏
		{
			gameExit();
			Sleep(3000);
			return 0;
		}

	}

over:
	gameOver();
	Sleep(3000);
	goto again;

	return 0;
}