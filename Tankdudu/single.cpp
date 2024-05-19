#include"tankclass.h"
#include"tankhead.h"

bool isgaming = 1;




//单人游戏进入这个函数，避免main函数过长（C语言课设因为这个问题我要死了）
void singlegame()
{
	cleardevice();
	RECT center = { 0,0,639,479 };
	settextstyle(36, 0, "华文隶书");
	settextcolor(WHITE);
	drawtext("开发中，按任意键开始测试。", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();
	Player player('W','S','A','D','R','J');
	std::random_device rd;  // 获取随机数种子
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 2);
	int kind = distrib(gen);
	obstacle wall3[3] =
	{
		obstacle(100, 100, 100, 100, 0, SUPER_OBSTACLE, 3),
		obstacle(200, 200, 200, 100, 0, SUPER_OBSTACLE, 3),
		obstacle(320, 260,100, 100, 0, SUPER_OBSTACLE, 3),			
	};


	std::thread thread1(&Player::changepng, &player ,isgaming);
	std::thread thread2(&Player::control,&player,std::ref(isgaming));
	std::thread thread3(&Player::footprint, &player, isgaming);
	std::thread thread4(&bullet::bullMove, isgaming);
	std::thread thread5(&Player::wait, &player, isgaming);
	setbkcolor(WHITE);
	BeginBatchDraw();
	while (isgaming)
	{
		cleardevice();
		player.display();
		for (int i=0; i < 3; i++)
		{
			wall3[i].display();
		}
		ColliderBox::drawColliderbox(player);
		bullet::display();
		HWND hWnd = GetHWnd();
		if (!IsWindow(hWnd))
			exit(0);
	 	FlushBatchDraw();
	}
	EndBatchDraw();
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
}