//�ļ�����tankhead.h
//���ߣ�������
//���ܣ��ṩ������ڣ��������ͷ�ļ�


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

void singlegame();//������Ϸ�������

void couplelegame();//˫����Ϸ�������

void operate();//����ָ�Ͻ����������

void checkachieve();//�ɾ�ϵͳ

bool unlockachieve(obstacle* wall, int score1, int score2);//����Ƿ���ɳɾ�

//�ɾ͵Ĵ���
void Acfileread();
void Acfilewrite();

//���ý������
void setting();
//���õĴ���
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
		settextstyle(18, 0, "��������");
		char t[4] = { '\0' };
		t[0] = (time / 100) + '0';
		t[1] = ((time % 100) / 10) + '0';
		t[2] = (time % 10) + '0';
		outtextxy(x - 50, y, "ʱ�䣺");
		outtextxy(x, y, _T(t));
	}
};

