#include"tankclass.h"
#include"tankhead.h"
extern std::vector<ColliderBox> allbox;
extern std::vector<std::thread> allthread;
extern std::vector<bullet> allbullet;
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
	Player player;
	std::random_device rd;  // 获取随机数种子
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 2);
	int kind = distrib(gen);
	obstacle wall3[3] =
	{
		obstacle(320, 240, 10, 10, 0, SUPER_OBSTACLE, 3),
		obstacle(320, 220, 10, 10, 0, SUPER_OBSTACLE, 3),
		obstacle(320, 260,10, 10, 0, SUPER_OBSTACLE, 3),
			
	};
	std::thread thread1(&Player::changepng, &player ,isgaming);
	std::thread thread2(&Player::control,&player,std::ref(isgaming));
	std::thread thread3(&Player::footprint, &player, isgaming);
	std::thread thread4(&bullet::bullMove, isgaming);
	setbkcolor(WHITE);
	BeginBatchDraw();
	while (1)
	{
		cleardevice();
		player.display();
		o1.display();
		ColliderBox::drawColliderbox(player);
		bullet::display();
		FlushBatchDraw();
	}
	EndBatchDraw();
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	while (!allthread.empty());
}