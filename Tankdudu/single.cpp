#include"tankclass.h"
#include"tankhead.h"
extern std::vector<ColliderBox> allbox;
bool isgaming = 1;


//������Ϸ�����������������main����������C���Կ�����Ϊ���������Ҫ���ˣ�
void singlegame()
{
	cleardevice();
	RECT center = { 0,0,639,479 };
	settextstyle(36, 0, "��������");
	settextcolor(WHITE);
	drawtext("�����У����������ʼ���ԡ�", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();
	Player player;
	std::random_device rd;  // ��ȡ���������
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
	std::thread thread2(&Player::control, &player, isgaming);
	std::thread thread3(&Player::footprint, &player, isgaming);
	setbkcolor(WHITE);
	BeginBatchDraw();
	while (1)
	{
		cleardevice();
		player.display();
		for (int i=0; i < 3; i++)
		{
			wall3[i].display();
		}
		ColliderBox::drawColliderbox(player);
		FlushBatchDraw();
	}
	EndBatchDraw();
	thread1.join();
	thread2.join();
	thread3.join();
}