//�ļ�����couple.cpp
//���ߣ������ԣ������, ������
//���ܣ�Ϊ˫����Ϸ�ṩ���
#include"tankclass.h"
#include"tankhead.h"
typedef struct Setting
{
	bool sound = true;
	int background = 3;
	int gametime = 2;
} Setting;

extern Setting set;
//˫����Ϸ�����������������main����������C���Կ�����Ϊ���������Ҫ���ˣ�
void couplelegame()
{
	bool jug = true;
	ExMessage msg;
	isgaming = true;
	int score1 = 0;
	int score2 = 0;
	bool wait = true;
	cleardevice();
	IMAGE background;
	switch (set.background)
	{
	case 1:
		loadimage(&background, "sorce/bk1.jpg", 640, 480, 1);
		break;
	case 2:
		loadimage(&background, "sorce/bk2.jpg", 640, 480, 1);
		break;
	case 3:
		loadimage(&background, "sorce/bk3.jpg", 640, 480, 1);
	}
	RECT center = { 0,0,639,479 };
	RECT say1 = { 0,100,639,479 };
	RECT say2 = { 0,0,639,379 };
	RECT settlement = { 0,0,639,96 };
	settextstyle(36, 0, "��������");
	settextcolor(WHITE);
	drawtext("����һ�����ھ��Լ�����ը�ٶԷ���", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("׼����ս��", &say2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("���������ʼ��Ϸ.", &say1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();
	while (jug)
	{
		if (peekmessage(&msg, EX_KEY))
			jug = false;
	}

	Player player1('W', 'S', 'A', 'D', 'R', VK_SPACE);
	Player player2(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_SHIFT, VK_RETURN, 5);
	//׼����ͼ
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
		obstacle(250, 80,147, 75, 0, 50, 1),
		obstacle(250, 330,147, 75, 0, 50, 1),
	};
	int time = MAXTIME / set.gametime;
	//���߳�
	//��ͼ
	std::thread thread1(&Player::changepng, &player1, std::ref(isgaming));
	std::thread thread10(&Player::changepng, &player2, std::ref(isgaming));
	//��ҿ���
	std::thread thread2(&Player::control, &player1, std::ref(isgaming));
	std::thread thread3(&Player::control, &player2, std::ref(isgaming));
	//�ӵ�����
	std::thread thread4(&bullet::bullMove, std::ref(isgaming));
	//�����ȴ
	std::thread thread5(&Player::wait, &player1, std::ref(isgaming));
	std::thread thread6(&Player::wait, &player2, std::ref(isgaming));
	//��ʱ
	std::thread thread7(&TimeFun::setTime, std::ref(time), std::ref(isgaming));
	//����Ѫ
	std::thread thread8(&otherFun::checkdead, wall_rock, wall_wire_mesh, std::ref(player1), std::ref(player2));
	//����
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
		//����ģʽ����ʾ��ײ��
		//ColliderBox::drawColliderbox(player);
		bullet::display();
		HWND hWnd = GetHWnd();
		if (IsWindow(hWnd))
			FlushBatchDraw();
		else
			exit(0);
		//�Ƴɼ�
		if (wait)
		{
			if (!player1.IsAlive)
				score2++;
			if (!player2.IsAlive)
				score1++;
			wait = false;
		}
		//ȫ������ָ�
		if (player1.IsAlive && player2.IsAlive)
			wait = true;
	}
	EndBatchDraw();
	setfillcolor(0x9BB171);
	fillrectangle(160, 120, 480, 360);

	settextcolor(WHITE);
	settextstyle(36, 0, "��������");
	drawtext("��Ϸ���������Ժ�", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	if (time != 0)
		std::cout << "��Ϸδ���" << std::endl;
	time = 0;

	//�ȴ��߳̽���
	thread1.join();
	std::cout << "thread1 have been over." << std::endl;
	thread2.join();
	std::cout << "thread2 have been over." << std::endl;
	thread3.join();
	std::cout << "thread3 have been over." << std::endl;
	thread4.join();
	std::cout << "thread4 have been over." << std::endl;
	thread5.join();
	std::cout << "thread5 have been over." << std::endl;
	thread6.join();
	std::cout << "thread6 have been over." << std::endl;
	thread7.join();
	std::cout << "thread7 have been over." << std::endl;
	thread8.join();
	std::cout << "thread8 have been over." << std::endl;
	thread9.join();
	std::cout << "thread9 have been over." << std::endl;
	thread10.join();
	std::cout << "thread10 have been over." << std::endl;
	thread11.join();
	std::cout << "thread11 have been over." << std::endl;
	allbox.clear();
	IDnum = 0;
	std::cout << "All threads have been over." << std::endl;

	cleardevice();
	std::cin.sync();
	if (score1 > score2)
	{
		drawtext("P1 WIN!!!", &settlement, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		IMAGE img1;
		loadimage(&img1, "sorce/awardsP1.png", 400, 300);
		Function::transparentimage(NULL, 319 - 200, 219 - 150, &img1);
	}
	if (score1 < score2)
	{
		drawtext("P2 WIN!!!", &settlement, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		IMAGE img1;
		loadimage(&img1, "sorce/awardsP2.png", 400, 300);
		Function::transparentimage(NULL, 319 - 200, 219 - 150, &img1);
	}
	if (score1 == score2)
		drawtext("SCORE DRAW", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	button over(260, 390, 120, 40, "����");
	over.create();
	jug = true;
	while (jug)
	{
		if (peekmessage(&msg, EX_MOUSE))
			if (msg.message == WM_LBUTTONDOWN)
				if (over.test(msg))
					jug = false;
	}
	//���������
	std::cin.sync();
}