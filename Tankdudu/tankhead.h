//文件名：tankhead.h
//作者：杨武显
//功能：提供函数入口，打包各种头文件


#pragma once
#pragma warning(disable:6031)
#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<thread>
#include<cmath>
#include<mmsystem.h>
#include<fstream>
#include <random>
#include<mutex>
#include<cstdlib>
#include <time.h>  

#include "tankhead.h"
class obstacle;

#pragma comment(lib,"winmm.lib")

void singlegame();//单人游戏函数入口

void couplelegame();//双人游戏函数入口

void operate();//操作指南进入这个函数

void checkachieve();//成就系统

bool unlockachieve(obstacle* wall, int score1, int score2);//检测是否完成成就

//成就的储存
void Acfileread();
void Acfilewrite();

//设置界面入口
void setting();
//设置的储存
void sefileread();
void sefilewrite();

class TimeFun
{
public:


	static void setTime(int& time, bool& isgaming)
	{
		for (; time > 0; time--)
		{
			Sleep(1000);
		}
		isgaming = false;
	}
	static void showTime(int& time, int x, int y)
	{
		settextstyle(18, 0, "华文隶书");
		char t[4] = { '\0' };
		t[0] = (time / 100) + '0';
		t[1] = ((time % 100) / 10) + '0';
		t[2] = (time % 10) + '0';
		outtextxy(x - 50, y, "时间：");
		outtextxy(x, y, _T(t));
	}
};

