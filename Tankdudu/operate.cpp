#include"tankclass.h"
#include"tankhead.h"
//����ָ�Ͻ����������

void operate()
{
	int operate_jug = 1;
	button* b1 = new button(60, 390, 120, 50, "���ز˵�");
	IMAGE img1,img2;
	loadimage(&img1, "sorce/operate.png", 640, 360);
	loadimage(&img2, "sorce/white.png", 640, 480);
	while (operate_jug)
	{
		cleardevice();
		Function::transparentimage(NULL, 0, 0, &img2);
		Function::transparentimage(NULL, 0, 50, &img1);
		b1->create();
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
					if (choose)
						jug = false;
					break;
				default:
					break;
				}
			}
		}
		if (choose)
		{
			operate_jug = 0;
		}
	}
}