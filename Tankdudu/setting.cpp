#define _CRT_SECURE_NO_WARNINGS
//文件名：setting.cpp
//作者：杨武显
//功能：设置
#include "tankclass.h"
#include "tankhead.h"

extern int volume_jug;

typedef struct Setting
{
	bool sound = true;
	int background = 1;
	int gametime = MAXTIME;
} Setting;
typedef struct allscore
{
	int score1;
	int score2;
}allscore;
typedef struct Achievement
{
	bool a1 = false, a2 = false, a3 = false, a4 = false;
} Achieve;
extern Achieve achieve;
extern std::vector<allscore> scores;
extern Setting set;
void setting()
{
	//准备页面
	IMAGE background;
	loadimage(&background, "sorce/bk3.jpg", 640, 480, 1);
	RECT upcenter{ 0,0,639,50 };
	ExMessage msg;
	//创建一堆按钮
	char str[10];
	if (set.sound)
		strcpy(str, "静音");
	else
		strcpy(str, "解除静音");
	button s1(140, 90, 120, 50, _T(str));
	button s2(140, 175, 120, 50, "修改背景");
	button s3(350, 90, 120, 50, "游戏时间");
	button s4(350, 175, 120, 50, "清空记录");
	button s5(245, 265, 120, 50, "重置成就");
	button back(245, 350, 120, 50, "返回");
	
	bool issetting = true;
	BeginBatchDraw();
	//设置主循环
	while (issetting)
	{
		//准备页面
		cleardevice();
		putimage(0, 0, &background);
		settextstyle(40, 0, "华文隶书");
		settextcolor(BLACK);
		drawtext("设   置", &upcenter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setfillcolor(0x9BB171);
		fillrectangle(70, 50, 570, 430);

		s1.create();
		s2.create();
		settextstyle(20, 0, "微软雅黑");
		switch (set.background)
		{
		case 1:
			outtextxy(140, 230, "当前背景：灌木丛");
			break;
		case 2:
			outtextxy(140, 230, "当前背景：沙漠");
			break;
		case 3:
			outtextxy(140, 230, "当前背景：草地");
		}
		
		s3.create();
		settextstyle(20, 0, "微软雅黑");
		switch (set.gametime)
		{
		case 1:
			outtextxy(350, 145, "游戏时间：120s");
			break;
		case 2:
			outtextxy(350, 145, "游戏时间：60s");
			break;
		case 3:
			outtextxy(350, 145, "游戏时间：40s");
		}
		s4.create();
		s5.create();
		back.create();
		FlushBatchDraw();
		//检测是否按下
		int choose = 0;
		bool jug = true;
		while (jug)
		{
			HWND hWnd = GetHWnd();
			if (!IsWindow(hWnd))
				exit(0);
			//获取鼠标消息，检测是否按在了按钮上
			if (peekmessage(&msg, EX_MOUSE))
			{
				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					if (s1.test(msg))
						choose = 1;
					if (s2.test(msg))
						choose = 2;
					if (s3.test(msg))
						choose = 3;
					if (s4.test(msg))
						choose = 4;
					if (s5.test(msg))
						choose = 5;
					if (back.test(msg))
						choose = 6;
					if (choose)
						jug = false;
					break;
				default:
					break;
				}
			}
		}

		switch (choose)
		{
		case 1:
			volume_jug = (volume_jug + 1) % 2;
			if (volume_jug)
				s1.changetext("静音");
			else
				s1.changetext("解除静音");
			set.sound = !set.sound;
			break;
		case 2:
			set.background++;
			if (set.background > 3)
				set.background -= 3;
			break;
		case 3:
			set.gametime++;
			if (set.gametime > 3)
				set.gametime -= 3;
			break;
		case 4:
			scores.clear();
			s4.changetext("清除成功");
			break;
		case 5:
			achieve.a1 = false;
			achieve.a2 = false;
			achieve.a3 = false;
			achieve.a4 = false;
			s5.changetext("重置成功");
			break;
		case 6:
			issetting = false;
			break;
		}
	}
	EndBatchDraw();
	std::cin.sync();
}

//文件储存
void sefileread()
{
	std::ifstream file("setting.bin", std::ios::binary); // 打开文件以进行读取  

	if (!file)
	{ // 检查文件是否成功打开  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//读取
	file.read(reinterpret_cast<char*>(&set), sizeof(Setting));
	//关闭文件
	file.close();
}

void sefilewrite()
{
	std::ofstream file("setting.bin", std::ios::binary); // 打开文件以进行读取  

	if (!file)
	{ // 检查文件是否成功打开  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//读取
	file.write(reinterpret_cast<char*>(&set), sizeof(Setting));
	//关闭文件
	file.close();
}