#include"tankclass.h"
#include"tankhead.h"

bool isgaming = 1;
unsigned char map[ROWS][COLS];

static void checkdead(obstacle* wall_rock, obstacle* wall_wire_mesh, Player& player, Enemy& enemy)
{
	while (isgaming)
	{
		for (int i = 0; i < 4; i++)
		{
			wall_rock[i].deblood();
			wall_wire_mesh[i].deblood();
			wall_rock[i].Dead();
			wall_wire_mesh[i].Dead();
		}
		player.deblood();
		enemy.deblood();
		player.Dead();
		enemy.Dead();
	}
}


//单人游戏进入这个函数，避免main函数过长（C语言课设因为这个问题我要死了）
void singlegame()
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

	Player player('W','S','A','D','R','J');
	Enemy enemy;
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
	int time = MAXTIME;
	//多线程
	//动图
	std::thread thread1(&Player::changepng, &player ,std::ref(isgaming));
	//玩家控制
	std::thread thread2(&Player::control,&player,std::ref(isgaming));
	//AI控制
	std::thread thread3(&Enemy::aicontrol, &enemy, std::ref(isgaming));
	//子弹控制
	std::thread thread4(&bullet::bullMove, std::ref(isgaming));
	//玩家冷却
	std::thread thread5(&Player::wait, &player, std::ref(isgaming));
	//AI冷却
	std::thread thread6(&Enemy::wait, &enemy, std::ref(isgaming));
	//计时
	std::thread thread7(&TimeFun::setTime, std::ref(time), std::ref(isgaming));
	//检测扣血
	std::thread thread8(&checkdead, wall_rock, wall_wire_mesh, std::ref(player), std::ref(enemy));
	//能量
	std::thread thread9(&Player::defpower, &player, std::ref(isgaming));
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
		outtextxy(540, 10, _T(s));
		for (int i = 0; i < 4; i++)
		{
			wall_rock[i].display();
			wall_wire_mesh[i].display();
		}
		player.display();
		enemy.display();
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
			if (!player.IsAlive)
				score2++;
			if (!enemy.IsAlive)
				score1++;
			wait = false;
		}
		//全部复活，恢复
		if (player.IsAlive && enemy.IsAlive)
			wait = true;
	}
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
	std::cout << "All threads have been over." << std::endl;
	std::cin.sync();
}