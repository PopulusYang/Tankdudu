//�ļ�����achievement.cpp
//���ߣ�������
//���ܣ��ṩ�˳ɾ͵���غ���

#include "tankhead.h"
#include"tankclass.h"

typedef struct Achievement
{
	bool a1 = false, a2 = false, a3 = false, a4 = false;
} Achieve;

extern Achieve achieve;

void Acfileread()
{
	std::ifstream file("achievement.bin", std::ios::binary); // ���ļ��Խ��ж�ȡ  

	if (!file)
	{ // ����ļ��Ƿ�ɹ���  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//��ȡ
	file.read(reinterpret_cast<char*>(&achieve.a1), sizeof(bool));
	file.read(reinterpret_cast<char*>(&achieve.a2), sizeof(bool));
	file.read(reinterpret_cast<char*>(&achieve.a3), sizeof(bool));
	file.read(reinterpret_cast<char*>(&achieve.a4), sizeof(bool));
	//�ر��ļ�
	file.close();
}

void Acfilewrite()
{
	std::ofstream file("achievement.bin", std::ios::binary); // ���ļ��Խ���д��

	if (!file)
	{ // ����ļ��Ƿ�ɹ���  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//д���ļ�
	file.write(reinterpret_cast<char*>(&achieve.a1), sizeof(bool));
	file.write(reinterpret_cast<char*>(&achieve.a2), sizeof(bool));
	file.write(reinterpret_cast<char*>(&achieve.a3), sizeof(bool));
	file.write(reinterpret_cast<char*>(&achieve.a4), sizeof(bool));
	//�ر��ļ�
	file.close();
}

void checkachieve()
{
	//ҳ��׼��
	IMAGE background;
	loadimage(&background, "sorce/bk3.jpg", 640, 480, 1);
	cleardevice();
	putimage(0, 0, &background);
	RECT upcenter{ 0,0,639,50 };
	settextstyle(40, 0, "��������");
	settextcolor(RED);
	drawtext("��   ��", &upcenter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setfillcolor(0x9BB171);
	fillrectangle(70, 50, 570, 430);
	button back(260, 350, 120, 50, "����");
	back.create();
	//���سɾ͵�ͼƬ
	IMAGE a1, a2, a3, a4;
	loadimage(&a1, "sorce/achievement/cleaner.png", 80, 80, 1);
	loadimage(&a2, "sorce/achievement/Killer.png", 80, 80, 1);
	loadimage(&a3, "sorce/achievement/loser.png", 80, 80, 1);
	loadimage(&a4, "sorce/achievement/winner.png", 80, 80, 1);


	settextcolor(BLACK);
	//�ɾ�1
	if (achieve.a1)
	{
		putimage(105, 85, &a1);
		settextstyle(20, 0, "΢���ź�");
		outtextxy(190, 90, "������");
		settextstyle(18, 0, "΢���ź�");
		outtextxy(190, 110, "�򻵵�ͼ������");
		outtextxy(190, 130, "��˿��");
		outtextxy(190, 150, "�����氮�ɾ���");
	}
	else
	{
		RECT center{ 105,85,300,165 };
		settextstyle(40, 0, "��������");
		settextcolor(BLACK);
		drawtext("δ����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//�ɾ�2
	if (achieve.a2)
	{
		putimage(330, 85, &a2);
		settextstyle(20, 0, "΢���ź�");
		outtextxy(415, 90, "ɱ��");
		settextstyle(18, 0, "΢���ź�");
		outtextxy(415, 110, "������Ϸ��ɱ��");
		outtextxy(415, 130, "����5��");
		outtextxy(415, 150, "�����������ˣ���");
	}
	else
	{
		RECT center{ 330,85,525,165 };
		settextstyle(40, 0, "��������");
		settextcolor(BLACK);
		drawtext("δ����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//�ɾ�3
	if (achieve.a3)
	{
		putimage(105, 200, &a3);
		settextstyle(20, 0, "΢���ź�");
		outtextxy(190, 205, "ʧ����");
		settextstyle(18, 0, "΢���ź�");
		outtextxy(190, 225, "������Ϸ�б�����");
		outtextxy(190, 245, "ɱ��5��");
		outtextxy(190, 265, "������ã�AI��ɵ��");
	}
	else
	{
		RECT center{ 105,200,300,280 };
		settextstyle(40, 0, "��������");
		settextcolor(BLACK);
		drawtext("δ����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//�ɾ�4
	if (achieve.a4)
	{
		putimage(330, 200, &a4);
		settextstyle(20, 0, "΢���ź�");
		outtextxy(415, 205, "ʤ����");
		settextstyle(18, 0, "΢���ź�");
		outtextxy(415, 225, "�õ�һ��ʤ��");
		outtextxy(415, 265, "��������ˣ���");
	}
	else
	{
		RECT center{ 330,200,525,280 };
		settextstyle(40, 0, "��������");
		settextcolor(BLACK);
		drawtext("δ����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//�����
	rectangle(105, 85, 300, 165);
	rectangle(105, 200, 300, 280);
	rectangle(330, 85, 525, 165);
	rectangle(330, 200, 525, 280);
	//��ť�˳�
	bool jug = true;
	ExMessage msg;
	while (jug)
	{
		if (peekmessage(&msg, EX_MOUSE))
			if (msg.message == WM_LBUTTONDOWN)
				if (back.test(msg))
					jug = false;
	}
	std::cin.sync();
}

//����Ƿ���ɳɾ�
bool unlockachieve(obstacle* wall,int score1, int score2)
{
	bool rejug = false;
	if (!achieve.a1)
	{
		bool jug = true;
		for (int i = 0; i < 4; i++)
		{
			if (wall[i].mhealth != MAXHEALTH)
				jug = false;
		}
		if (jug)
		{
			achieve.a1 = true;
			rejug = true;
		}
	}
	if (!achieve.a2)
	{
		if (score1 >= 5)
		{
			achieve.a2 = true;
			rejug = true;
		}
	}
	if (!achieve.a3)
	{
		if (score2 >= 5)
		{
			achieve.a3 = true;
			rejug = true;
		}
	}
	if (!achieve.a4)
	{
		if (score1 > score2)
		{
			achieve.a4 = true;
			rejug = true;
		}
	}
	return rejug;
}