#include"tankclass.h"
#include"tankhead.h"
//双人游戏进入这个函数，避免main函数过长（C语言课设因为这个问题我要死了）
void couplelegame()
{
	int score1 = 0;
	int score2 = 0;
	bool wait = true;
	cleardevice();
	IMAGE background;
	loadimage(&background, "sorce/bk3.jpg", 640, 480, 1);

	RECT center = { 0,0,639,479 };
	settextstyle(36, 0, "华文隶书");
	settextcolor(WHITE);
	drawtext("开发中，按任意键开始测试。", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();

	Player player1('W', 'S', 'A', 'D', 'R', 'J');
	Player player2(VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,VK_SHIFT,VK_RETURN,5);

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
	int time = MAXTIME;
	//多线程
	//动图
	std::thread thread1(&Player::changepng, &player1, std::ref(isgaming));
	std::thread thread10(&Player::changepng, &player2, std::ref(isgaming));
	//玩家控制
	std::thread thread2(&Player::control, &player1, std::ref(isgaming));
	std::thread thread3(&Player::control, &player2, std::ref(isgaming));
	//子弹控制
	std::thread thread4(&bullet::bullMove, std::ref(isgaming));
	//玩家冷却
	std::thread thread5(&Player::wait, &player1, std::ref(isgaming));
	std::thread thread6(&Player::wait, &player2, std::ref(isgaming));
	//计时
	std::thread thread7(&TimeFun::setTime, std::ref(time), std::ref(isgaming));
	//检测扣血
	std::thread thread8(&otherFun::checkdead, wall_rock, wall_wire_mesh, std::ref(player1), std::ref(player2));
	//能量
	std::thread thread9(&Player::defpower, &player1, std::ref(isgaming));
	std::thread thread11(&Player::defpower, &player2, std::ref(isgaming));
	BeginBatchDraw();
	char s[12] = "score ";
	while (isgaming)
	{
		cleardevice();
		putimage(0, 0, &background);

		TimeFun::showTime(time, 540, 0);

		s[6] = score1 / 10 + '0';
		s[7] = score1 % 10 + '0';
		s[8] = ':';
		s[9] = score2 / 10 + '0';
		s[10] = score2 % 10 + '0';
		outtextxy(490, 15, _T(s));
		for (int i = 0; i < 4; i++)
		{
			wall_rock[i].display();
			wall_wire_mesh[i].display();
		}
		player1.display();
		player2.display();
		//调试模式：显示碰撞箱
		//ColliderBox::drawColliderbox(player);
		bullet::display();
		HWND hWnd = GetHWnd();
		if (IsWindow(hWnd))
			FlushBatchDraw();
		else
			exit(0);
		//计成绩
		if (wait)
		{
			if (!player1.IsAlive)
				score2++;
			if (!player2.IsAlive)
				score1++;
			wait = false;
		}
		//全部复活，恢复
		if (player1.IsAlive && player2.IsAlive)
			wait = true;
	}
	if (time != 0)
		std::cout << "游戏未结束" << std::endl;
	time = 0;
	cleardevice();
	EndBatchDraw();
	//等待线程结束
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
	thread6.join();
	thread7.join();
	thread8.join();
	thread9.join();
	thread10.join();
	thread11.join();
	std::cout << "All threads have been over." << std::endl;
	std::cin.sync();
}