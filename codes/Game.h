// 为了避免同一个头文件被包含（include）多次
#pragma once

// 解决VS中对函数内存做边界,内存越界等问题的检查
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>					// 操作时间
#include <time.h>
#include <conio.h>					// 按键交互
#include <stdlib.h>					// Sleep
#include <iostream>					// 输入输出
#include <graphics.h>				// 图形界面
#include <mmstream.h>				// 播放音乐
#pragma comment(lib, "winmm.lib")	// 多媒体设备接口
using namespace std; 

/********************************************关于菜单界面*******************************************************/

void menuInit();				// 菜单界面

void secret();					// 小秘密界面

void gameExit();				// 退出游戏

void gameOver();				// 游戏结束

bool isShowGrid();				//游戏界面是否显示网格

void showLenth(int len);		// 显示长度

void showScore(int score);		// 显示分数

void showTime(double snakeTime);// 显示时间

/********************************************关于游戏模式*******************************************************/

void Eat();				// 吃食物, 得分(取决于半径大小1, 2, 3, 4), 重新随机生成食物

void keyControl();		// 按键交互

bool isAcceleration();	// 判断是否加速
	
bool snakeMove1();		// 普通模式判断是否能移动蛇

bool snakeMove2();		// 无尽模式和关卡模式判断是否能移动蛇

void gameInit1();		// 普通模式和无尽模式初始化
	
void gameInit2();		// 关卡1初始化

void wallDraw();		// 画墙

void lineDraw();		// 画网格线

void gameDraw1();		// 普通模式

void gameDraw2();		// 无尽模式

void gameDraw3();		// 关卡模式

int getScore();			// 获取分数

int getLen();			// 获取长度
/**************************************************************************************************************/