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

void singlegame();//������Ϸ�������

void couplelegame();//˫����Ϸ�������

void operate();//����ָ�Ͻ����������

class TimeFun
{
public:

	//��ʾ����ʱ

//����ʱ60��

//�����Ͻ���ʾ

//��ʱ����isgaming��Ϊfalse


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
		settextstyle(18, 0, "��������");
		char t[3] = { '\0' };
		t[0] = (time / 10) + '0';
		t[1] = (time % 10) + '0';
		outtextxy(x - 50, y, "ʱ�䣺");
		outtextxy(x, y, _T(t));
	}
};

