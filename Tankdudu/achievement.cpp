//文件名：achievement.cpp
//作者：杨武显
//功能：提供了成就的相关函数

#include "tankhead.h"
#include"tankclass.h"

typedef struct Achievement
{
	bool a1 = false, a2 = false, a3 = false, a4 = false;
} Achieve;

extern Achieve achieve;

void Acfileread()
{
	std::ifstream file("achievement.bin", std::ios::binary); // 打开文件以进行读取  

	if (!file)
	{ // 检查文件是否成功打开  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//读取
	file.read(reinterpret_cast<char*>(&achieve.a1), sizeof(bool));
	file.read(reinterpret_cast<char*>(&achieve.a2), sizeof(bool));
	file.read(reinterpret_cast<char*>(&achieve.a3), sizeof(bool));
	file.read(reinterpret_cast<char*>(&achieve.a4), sizeof(bool));
	//关闭文件
	file.close();
}

void Acfilewrite()
{
	std::ofstream file("achievement.bin", std::ios::binary); // 打开文件以进行写入

	if (!file)
	{ // 检查文件是否成功打开  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//写入文件
	file.write(reinterpret_cast<char*>(&achieve.a1), sizeof(bool));
	file.write(reinterpret_cast<char*>(&achieve.a2), sizeof(bool));
	file.write(reinterpret_cast<char*>(&achieve.a3), sizeof(bool));
	file.write(reinterpret_cast<char*>(&achieve.a4), sizeof(bool));
	//关闭文件
	file.close();
}

void checkachieve()
{
	//页面准备
	IMAGE background;
	loadimage(&background, "sorce/bk3.jpg", 640, 480, 1);
	cleardevice();
	putimage(0, 0, &background);
	RECT upcenter{ 0,0,639,50 };
	settextstyle(40, 0, "华文隶书");
	settextcolor(RED);
	drawtext("成   就", &upcenter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setfillcolor(0x9BB171);
	fillrectangle(70, 50, 570, 430);
	button back(260, 350, 120, 50, "返回");
	back.create();
	//加载成就的图片
	IMAGE a1, a2, a3, a4;
	loadimage(&a1, "sorce/achievement/cleaner.png", 80, 80, 1);
	loadimage(&a2, "sorce/achievement/Killer.png", 80, 80, 1);
	loadimage(&a3, "sorce/achievement/loser.png", 80, 80, 1);
	loadimage(&a4, "sorce/achievement/winner.png", 80, 80, 1);


	settextcolor(BLACK);
	//成就1
	if (achieve.a1)
	{
		putimage(105, 85, &a1);
		settextstyle(20, 0, "微软雅黑");
		outtextxy(190, 90, "清理者");
		settextstyle(18, 0, "微软雅黑");
		outtextxy(190, 110, "打坏地图上所有");
		outtextxy(190, 130, "铁丝网");
		outtextxy(190, 150, "“你真爱干净”");
	}
	else
	{
		RECT center{ 105,85,300,165 };
		settextstyle(40, 0, "华文隶书");
		settextcolor(BLACK);
		drawtext("未解锁", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//成就2
	if (achieve.a2)
	{
		putimage(330, 85, &a2);
		settextstyle(20, 0, "微软雅黑");
		outtextxy(415, 90, "杀手");
		settextstyle(18, 0, "微软雅黑");
		outtextxy(415, 110, "单人游戏中杀死");
		outtextxy(415, 130, "对手5次");
		outtextxy(415, 150, "“死神，来收人！”");
	}
	else
	{
		RECT center{ 330,85,525,165 };
		settextstyle(40, 0, "华文隶书");
		settextcolor(BLACK);
		drawtext("未解锁", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//成就3
	if (achieve.a3)
	{
		putimage(105, 200, &a3);
		settextstyle(20, 0, "微软雅黑");
		outtextxy(190, 205, "失败者");
		settextstyle(18, 0, "微软雅黑");
		outtextxy(190, 225, "单人游戏中被对手");
		outtextxy(190, 245, "杀死5次");
		outtextxy(190, 265, "“别摆烂，AI很傻”");
	}
	else
	{
		RECT center{ 105,200,300,280 };
		settextstyle(40, 0, "华文隶书");
		settextcolor(BLACK);
		drawtext("未解锁", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//成就4
	if (achieve.a4)
	{
		putimage(330, 200, &a4);
		settextstyle(20, 0, "微软雅黑");
		outtextxy(415, 205, "胜利者");
		settextstyle(18, 0, "微软雅黑");
		outtextxy(415, 225, "得到一次胜利");
		outtextxy(415, 265, "“不过如此！”");
	}
	else
	{
		RECT center{ 330,200,525,280 };
		settextstyle(40, 0, "华文隶书");
		settextcolor(BLACK);
		drawtext("未解锁", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//画框框
	rectangle(105, 85, 300, 165);
	rectangle(105, 200, 300, 280);
	rectangle(330, 85, 525, 165);
	rectangle(330, 200, 525, 280);
	//按钮退出
	bool jug = true;
	ExMessage msg;
	while (jug)
	{
		if (peekmessage(&msg, EX_MOUSE))
			if (msg.message == WM_LBUTTONDOWN)
				if (back.test(msg))
					jug = false;
	}
	std::cin.sync();
}

//检测是否完成成就
bool unlockachieve(obstacle* wall,int score1, int score2)
{
	bool rejug = false;
	if (!achieve.a1)
	{
		bool jug = true;
		for (int i = 0; i < 4; i++)
		{
			if (wall[i].mhealth != MAXHEALTH)
				jug = false;
		}
		if (jug)
		{
			achieve.a1 = true;
			rejug = true;
		}
	}
	if (!achieve.a2)
	{
		if (score1 >= 5)
		{
			achieve.a2 = true;
			rejug = true;
		}
	}
	if (!achieve.a3)
	{
		if (score2 >= 5)
		{
			achieve.a3 = true;
			rejug = true;
		}
	}
	if (!achieve.a4)
	{
		if (score1 > score2)
		{
			achieve.a4 = true;
			rejug = true;
		}
	}
	return rejug;
}