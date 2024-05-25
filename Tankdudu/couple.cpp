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
	Player player1('W', 'S', 'A', 'D', 'R', VK_SPACE);
	Player player2(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 'I', VK_RETURN, 5);
	/*std::random_device rd;  // 获取随机数种子
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 2);
	int kind = distrib(gen);*/
	obstacle wall_rock[4] =
	{
		obstacle(100, 80, 150, 70, 0, SUPER_OBSTACLE, 3),
		obstacle(100, 340, 150, 70, 0, SUPER_OBSTACLE, 3),
		obstacle(400, 340,150, 70, 0, SUPER_OBSTACLE, 3),
		obstacle(400, 80,150, 70, 0, SUPER_OBSTACLE, 3),
	};
	obstacle wall_wire_mesh[4] =
	{
		obstacle(180, 175, 75, 147, 0, 50, 2),
		obstacle(390, 175, 75, 147, 0, 50, 2),
		obstacle(250, 100,147, 75, 0, 50, 1),
		obstacle(250, 310,147, 75, 0, 50, 1),
	};

	std::thread thread1(&Player::changepng, &player1, std::ref(isgaming));
	std::thread thread2(&Player::control, &player1, std::ref(isgaming));
	std::thread thread4(&bullet::bullMove, std::ref(isgaming));
	std::thread thread5(&Player::wait, &player1, std::ref(isgaming));
	std::thread thread3(&Player::changepng, &player2, std::ref(isgaming));
	std::thread thread6(&Player::control, &player2, std::ref(isgaming));
	std::thread thread7(&Player::wait, &player2, std::ref(isgaming));
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
	cleardevice();
	EndBatchDraw();
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();

}