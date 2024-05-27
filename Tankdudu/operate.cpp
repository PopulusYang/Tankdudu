//文件名：operate.cpp
//作者：朱佳悦
//功能：展示操作指南

#include"tankclass.h"
#include"tankhead.h"
//操作指南进入这个函数

void operate()
{
	int operate_jug = 1;
	int player = 1;
	button* b1 = new button(60, 390, 120, 50, "返回菜单");
	button* b2 = new button(260, 390, 120, 50, "单人操作");
	button* b3 = new button(460, 390, 120, 50, "双人操作");
	button* b4 = new button(460, 390, 120, 50, "其  他");
	button* b5 = new button(260, 390, 120, 50, "双人操作");
	IMAGE img1,img2,img3,img4;
	IMAGE img[3];
	loadimage(&img1, "sorce/operate.png", 550, 320);
	loadimage(&img2, "sorce/white.png", 640, 480);
	loadimage(&img3, "sorce/operate3.png", 480, 360);
	loadimage(&img4, "sorce/other.png", 480, 320);
	loadimage(&img[0], "sorce/tank4.png", 160,194);
	loadimage(&img[1], "sorce/tank5.png",160, 194);
	loadimage(&img[2], "sorce/tank6.png",160, 194);
	while (operate_jug)
	{
		if (player == 1)
		{
			Function::transparentimage(NULL, 0, 0, &img2);
			Function::transparentimage(NULL, 80, 50, &img3);

			b1->create();
			b3->create();
			int jug = 1;
			ExMessage msg;
			int choose = 0;
			while (jug)
			{			
				for (int i = 0; i < 3; i++)
			{
				Function::transparentimage(NULL, 340, 150, &img[i]);
			}
				HWND hWnd = GetHWnd();
				if (!IsWindow(hWnd))
					exit(0);
				if (peekmessage(&msg, EX_MOUSE))
				{
					switch (msg.message)
					{
					case WM_LBUTTONDOWN:
						if (b1->test(msg))
							choose = 1;
						if (b3->test(msg))
							choose = 2;
						if (choose)
							jug = false;
						break;
					default:
						break;
					}
				}
			}
			if (choose == 2)
			{
				player = 2;
			}
			if (choose==1)
			{
				operate_jug = 0;
			}
		}
		if (player == 2)
		{
			Function::transparentimage(NULL, 0, 0, &img2);
			Function::transparentimage(NULL, 50, 70, &img1);
			b1->create();
			b2->create();
			b4->create();
			int jug = 1;
			ExMessage msg;
			int choose = 0;
			while (jug)
			{
				HWND hWnd = GetHWnd();
				if (!IsWindow(hWnd))
					exit(0);
				if (peekmessage(&msg, EX_MOUSE))
				{
					switch (msg.message)
					{
					case WM_LBUTTONDOWN:
						if (b1->test(msg))
							choose = 1;
						if (b2->test(msg))
							choose = 2;
						if (b4->test(msg))
							choose = 3;
						if (choose)
							jug = false;
						break;
					default:
						break;
					}
				}
			}
			if (choose == 2)
			{
				player = 1;
			}
			if (choose == 3)
			{
				player = 3;
			}
			if (choose==1)
			{
				operate_jug = 0;
			}
		}
		if (player == 3)
		{
			Function::transparentimage(NULL, 0, 0, &img2);
			Function::transparentimage(NULL, 100, 60, &img4);
			b1->create();
			b5->create();
			int jug = 1;
			ExMessage msg;
			int choose = 0;
			while (jug)
			{
				HWND hWnd = GetHWnd();
				if (!IsWindow(hWnd))
					exit(0);
				if (peekmessage(&msg, EX_MOUSE))
				{
					switch (msg.message)
					{
					case WM_LBUTTONDOWN:
						if (b1->test(msg))
							choose = 1;
						if (b5->test(msg))
							choose = 2;
						if (choose)
							jug = false;
						break;
					default:
						break;
					}
				}
			}
			if (choose == 2)
			{
				player = 2;
			}
			if (choose == 1)
			{
				operate_jug = 0;
			}
		}
	}
	delete(b1);
	delete(b2);
	delete(b3);
	delete(b4);
	delete(b5);
}