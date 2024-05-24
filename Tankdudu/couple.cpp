#include"tankclass.h"
#include"tankhead.h"
//双人游戏进入这个函数，避免main函数过长（C语言课设因为这个问题我要死了）
void couplelegame()
{
	cleardevice();
	IMAGE background;
	loadimage(&background, "sorce/bk3.jpg", 640, 480, 1);
	RECT center = { 0,0,639,479 };
	settextstyle(36, 0, "华文隶书");
	settextcolor(WHITE);
	drawtext("开发中，按任意键开始测试。", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();
	Player player1('W', 'S', 'A', 'D', 'R', ' ');
	Player player2('UP', 'DOWN', 'LEFT', 'R', 'I', '0');
	/*std::random_device rd;  // 获取随机数种子
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 2);
	int kind = distrib(gen);*/
	obstacle wall_rock[4] =
	{
		obstacle(80, 70, 150, 100, 0, SUPER_OBSTACLE, 3),
		obstacle(80, 260, 150, 100, 0, SUPER_OBSTACLE, 3),
		obstacle(360, 260,150, 100, 0, SUPER_OBSTACLE, 3),
		obstacle(360, 70,150, 100, 0, SUPER_OBSTACLE, 3),
	};
	obstacle wall_wire_mesh[4] =
	{
		obstacle(185, 170, 45, 90, 0, 50, 2),
		obstacle(360, 170, 45, 90, 0, 50, 2),
		obstacle(225, 105,147, 75, 0, 50, 1),
		obstacle(225, 260,147, 75, 0,50, 1),
	};
	std::thread thread1(&Player::changepng, &player1, isgaming);
	std::thread thread2(&Player::control, &player1, std::ref(isgaming));
	std::thread thread4(&bullet::bullMove, isgaming);
	std::thread thread5(&Player::wait, &player1, isgaming);
	std::thread thread3(&Player::changepng, &player2, isgaming);
	std::thread thread6(&Player::control, &player2, std::ref(isgaming));
	std::thread thread7(&Player::wait, &player2, isgaming);
	setbkcolor(WHITE);
	BeginBatchDraw();
	while (isgaming)
	{
		cleardevice();
		putimage(0, 0, &background);
		for (int i = 0; i < 4; i++)
		{
			wall_rock[i].deblood();
			wall_wire_mesh[i].deblood();
			wall_rock[i].Dead();
			wall_wire_mesh[i].Dead();
		}
		player1.display();
		player2.display();
		for (int i = 0; i < 4; i++)
		{
			wall_rock[i].display();
			wall_wire_mesh[i].display();
		}
		ColliderBox::drawColliderbox(player1);
		bullet::display();
		HWND hWnd = GetHWnd();
		if (IsWindow(hWnd))
			FlushBatchDraw();
		else
			exit(0);
	}
	EndBatchDraw();
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();

}