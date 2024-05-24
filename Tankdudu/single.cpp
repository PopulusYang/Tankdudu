#include"tankclass.h"
#include"tankhead.h"

bool isgaming = 1;
unsigned char map[ROWS][COLS];

//单人游戏进入这个函数，避免main函数过长（C语言课设因为这个问题我要死了）
void singlegame()
{
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
	
	
	std::thread thread1(&Player::changepng, &player ,isgaming);
	std::thread thread2(&Player::control,&player,std::ref(isgaming));
	//std::thread thread3(&Enemy::aicontrol, &enemy, isgaming);
	std::thread thread4(&bullet::bullMove, isgaming);
	std::thread thread5(&Player::wait, &player, isgaming);
	//setbkcolor(WHITE);
	std::thread thread6(&Enemy::wait, &enemy, isgaming);
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
		for (int i=0; i < 4; i++)
		{
			wall_rock[i].display();
			wall_wire_mesh[i].display();
		}
		
		player.deblood();
		enemy.deblood();
		player.Dead();
		enemy.Dead();
		player.display();
		enemy.display();
		
		ColliderBox::drawColliderbox(player);
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
	//thread3.join();
	thread4.join();
	thread5.join();
	thread6.join();
}