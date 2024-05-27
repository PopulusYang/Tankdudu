#pragma once
#pragma warning(disable:6031)
#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<thread>
#include<cmath>
#include<mmsystem.h>
#include<fstream>
#pragma comment(lib,"winmm.lib")

void singlegame();//单人游戏函数入口

void couplelegame();//双人游戏函数入口

void operate();//操作指南进入这个函数

class TimeFun
{
public:

	//显示倒计时

//倒计时60秒

//在右上角显示

//计时结束isgaming变为false


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
		char t[3] = { '\0' };
		t[0] = (time / 10) + '0';
		t[1] = (time % 10) + '0';
		outtextxy(x - 50, y, "时间：");
		outtextxy(x, y, _T(t));
	}
};

