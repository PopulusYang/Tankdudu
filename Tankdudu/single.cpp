//�ļ�����single.cpp
//���ߣ������ԣ������,������
//���ܣ�Ϊ������Ϸ�ṩ���

#include"tankclass.h"
#include"tankhead.h"

extern bool pause;
bool isgaming = 1;
unsigned char map[ROWS][COLS];

typedef struct allscore
{
	int score1;
	int score2;
}allscore;

extern std::vector<allscore> scores;

typedef struct Setting
{
	bool sound = true;
	int background = 3;
	int gametime = 2;
} Setting;

extern Setting set;

//������Ϸ�����������������main����������C���Կ�����Ϊ���������Ҫ���ˣ�
void singlegame()
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
	drawtext("���ڹ涨ʱ���ھ��Լ�����ը�ٶԷ���", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("׼����ս��", &say2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("���������ʼ��Ϸ.", &say1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();
	while (jug)
	{
		if (peekmessage(&msg, EX_KEY))
			jug = false;
	}

	Player player('W', 'S', 'A', 'D', 'R', 'J');
	Enemy enemy;
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
	//��ʼ��ʱ��
	int time = MAXTIME / set.gametime;
	//������Ϸ��Ҫ���߳�
	//��ͼ
	std::thread thread1(&Player::changepng, &player, std::ref(isgaming));
	//��ҿ���
	std::thread thread2(&Player::control, &player, std::ref(isgaming));
	//AI����
	std::thread thread3(&Enemy::aicontrol, &enemy, std::ref(isgaming));
	//�ӵ�����
	std::thread thread4(&bullet::bullMove, std::ref(isgaming));
	//�����ȴ
	std::thread thread5(&Player::wait, &player, std::ref(isgaming));
	//AI��ȴ
	std::thread thread6(&Enemy::wait, &enemy, std::ref(isgaming));
	//��ʱ
	std::thread thread7(&TimeFun::setTime, std::ref(time), std::ref(isgaming));
	//����Ѫ
	std::thread thread8(&otherFun::checkdead, wall_rock, wall_wire_mesh, std::ref(player), std::ref(enemy));
	//����
	std::thread thread9(&Player::defpower, &player, std::ref(isgaming));
	BeginBatchDraw();
	char s[12] = "score ";
	while (isgaming)
	{
		cleardevice();
		putimage(0, 0, &background);

		TimeFun::showTime(time, 540, 0);
		//��ʾ����
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
		player.display();
		enemy.display();
		//����ģʽ����ʾ��ײ��
		//ColliderBox::drawColliderbox(player);
		//ColliderBox::drawColliderbox(enemy);
		bullet::display();
		//������ͣģ��
		if (pause)
		{
			EndBatchDraw();
			Pause(&isgaming);
			BeginBatchDraw();
		}
		HWND hWnd = GetHWnd();
		if (IsWindow(hWnd))
			FlushBatchDraw();
		else
			exit(0);
		//�Ƴɼ�
		if (wait)
		{
			if (!player.IsAlive)
				score2++;
			if (!enemy.IsAlive)
				score1++;
			wait = false;
		}
		//ȫ������ָ�
		if (player.IsAlive && enemy.IsAlive)
			wait = true;
	}
	EndBatchDraw();
	setfillcolor(0x9BB171);
	fillrectangle(160, 120, 480, 360);

	settextcolor(WHITE);
	settextstyle(36, 0, "��������");
	bool notover = false;
	if (time != 0)
		notover = true;
	drawtext("ͳ���У����Ժ�", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//�ȴ��߳̽���
	time = 0;
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
	allbox.clear();
	IDnum = 0;
	std::cout << "All threads have been over." << std::endl;
	//���������
	cleardevice();
	if (!notover)
	{
		std::cin.sync();
		if (scores.size() < 6 && (score1 != 0 || score2 != 0))
		{
			scores.push_back(allscore{ score1,score2 });
		}
		else
		{
			if (score1 != 0 || score2 != 0)
			{
				for (int i = 4; i >= 0; i--)
				{
					scores[i + 1] = scores[i];
				}
				scores[0].score1 = score1;
				scores[0].score2 = score2;
			}
		}
		if (score1 > score2)
		{
			for (int i = 0; i < 20; i++)
			{
				IMAGE img1;
				loadimage(&img1, "sorce/win.png", (int)(12.5 * (i + 1)), (int)(12.5 * (i + 1)));
				Function::transparentimage(NULL, 319 - (25 * (i + 1) / 4), 219 - (25 * (i + 1) / 4), &img1);
				settextcolor(RGB(255, 255, 255));
				drawtext("YOU WIN!!!", &settlement, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(RGB(0, 0, 0));
				Sleep(50);
			}
		}
		if (score1 < score2)
		{
			settextcolor(RGB(255, 255, 255));
			drawtext("YOU LOSE...", &settlement, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			IMAGE img1;
			loadimage(&img1, "sorce/lose.png", 250, 250);
			Function::transparentimage(NULL, 319 - 125, 219 - 125, &img1);
			settextcolor(RGB(0, 0, 0));
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
		//����Ƿ�������³ɾ�
		jug = unlockachieve(wall_wire_mesh, score1, score2);
		if (jug)
		{
			cleardevice();
			settextstyle(36, 0, "��������");
			settextcolor(WHITE);
			drawtext("���������³ɾͣ���ȥ�����ɣ�", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			over.create();
			while (jug)
			{
				if (peekmessage(&msg, EX_MOUSE))
					if (msg.message == WM_LBUTTONDOWN)
						if (over.test(msg))
							jug = false;
			}
		}
	}
	std::cin.sync();
}