// Ϊ�˱���ͬһ��ͷ�ļ���������include�����
#pragma once

// ���VS�жԺ����ڴ����߽�,�ڴ�Խ�������ļ��
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>					// ����ʱ��
#include <time.h>
#include <conio.h>					// ��������
#include <stdlib.h>					// Sleep
#include <iostream>					// �������
#include <graphics.h>				// ͼ�ν���
#include <mmstream.h>				// ��������
#pragma comment(lib, "winmm.lib")	// ��ý���豸�ӿ�
using namespace std; 

/********************************************���ڲ˵�����*******************************************************/

void menuInit();				// �˵�����

void secret();					// С���ܽ���

void gameExit();				// �˳���Ϸ

void gameOver();				// ��Ϸ����

bool isShowGrid();				//��Ϸ�����Ƿ���ʾ����

void showLenth(int len);		// ��ʾ����

void showScore(int score);		// ��ʾ����

void showTime(double snakeTime);// ��ʾʱ��

/********************************************������Ϸģʽ*******************************************************/

void Eat();				// ��ʳ��, �÷�(ȡ���ڰ뾶��С1, 2, 3, 4), �����������ʳ��

void keyControl();		// ��������

bool isAcceleration();	// �ж��Ƿ����
	
bool snakeMove1();		// ��ͨģʽ�ж��Ƿ����ƶ���

bool snakeMove2();		// �޾�ģʽ�͹ؿ�ģʽ�ж��Ƿ����ƶ���

void gameInit1();		// ��ͨģʽ���޾�ģʽ��ʼ��
	
void gameInit2();		// �ؿ�1��ʼ��

void wallDraw();		// ��ǽ

void lineDraw();		// ��������

void gameDraw1();		// ��ͨģʽ

void gameDraw2();		// �޾�ģʽ

void gameDraw3();		// �ؿ�ģʽ

int getScore();			// ��ȡ����

int getLen();			// ��ȡ����
/**************************************************************************************************************/