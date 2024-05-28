#define _CRT_SECURE_NO_WARNINGS
//�ļ�����setting.cpp
//���ߣ�������
//���ܣ�����
#include "tankclass.h"
#include "tankhead.h"

extern int volume_jug;

typedef struct Setting
{
	bool sound = true;
	int background = 1;
	int gametime = MAXTIME;
} Setting;
typedef struct allscore
{
	int score1;
	int score2;
}allscore;
typedef struct Achievement
{
	bool a1 = false, a2 = false, a3 = false, a4 = false;
} Achieve;
extern Achieve achieve;
extern std::vector<allscore> scores;
extern Setting set;
void setting()
{
	//׼��ҳ��
	IMAGE background;
	loadimage(&background, "sorce/bk3.jpg", 640, 480, 1);
	RECT upcenter{ 0,0,639,50 };
	ExMessage msg;
	//����һ�Ѱ�ť
	char str[10];
	if (set.sound)
		strcpy(str, "����");
	else
		strcpy(str, "�������");
	button s1(140, 90, 120, 50, _T(str));
	button s2(140, 175, 120, 50, "�޸ı���");
	button s3(350, 90, 120, 50, "��Ϸʱ��");
	button s4(350, 175, 120, 50, "��ռ�¼");
	button s5(245, 265, 120, 50, "���óɾ�");
	button back(245, 350, 120, 50, "����");
	
	bool issetting = true;
	BeginBatchDraw();
	//������ѭ��
	while (issetting)
	{
		//׼��ҳ��
		cleardevice();
		putimage(0, 0, &background);
		settextstyle(40, 0, "��������");
		settextcolor(BLACK);
		drawtext("��   ��", &upcenter, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setfillcolor(0x9BB171);
		fillrectangle(70, 50, 570, 430);

		s1.create();
		s2.create();
		settextstyle(20, 0, "΢���ź�");
		switch (set.background)
		{
		case 1:
			outtextxy(140, 230, "��ǰ��������ľ��");
			break;
		case 2:
			outtextxy(140, 230, "��ǰ������ɳĮ");
			break;
		case 3:
			outtextxy(140, 230, "��ǰ�������ݵ�");
		}
		
		s3.create();
		settextstyle(20, 0, "΢���ź�");
		switch (set.gametime)
		{
		case 1:
			outtextxy(350, 145, "��Ϸʱ�䣺120s");
			break;
		case 2:
			outtextxy(350, 145, "��Ϸʱ�䣺60s");
			break;
		case 3:
			outtextxy(350, 145, "��Ϸʱ�䣺40s");
		}
		s4.create();
		s5.create();
		back.create();
		FlushBatchDraw();
		//����Ƿ���
		int choose = 0;
		bool jug = true;
		while (jug)
		{
			HWND hWnd = GetHWnd();
			if (!IsWindow(hWnd))
				exit(0);
			//��ȡ�����Ϣ������Ƿ����˰�ť��
			if (peekmessage(&msg, EX_MOUSE))
			{
				switch (msg.message)
				{
				case WM_LBUTTONDOWN:
					if (s1.test(msg))
						choose = 1;
					if (s2.test(msg))
						choose = 2;
					if (s3.test(msg))
						choose = 3;
					if (s4.test(msg))
						choose = 4;
					if (s5.test(msg))
						choose = 5;
					if (back.test(msg))
						choose = 6;
					if (choose)
						jug = false;
					break;
				default:
					break;
				}
			}
		}

		switch (choose)
		{
		case 1:
			volume_jug = (volume_jug + 1) % 2;
			if (volume_jug)
				s1.changetext("����");
			else
				s1.changetext("�������");
			set.sound = !set.sound;
			break;
		case 2:
			set.background++;
			if (set.background > 3)
				set.background -= 3;
			break;
		case 3:
			set.gametime++;
			if (set.gametime > 3)
				set.gametime -= 3;
			break;
		case 4:
			scores.clear();
			s4.changetext("����ɹ�");
			break;
		case 5:
			achieve.a1 = false;
			achieve.a2 = false;
			achieve.a3 = false;
			achieve.a4 = false;
			s5.changetext("���óɹ�");
			break;
		case 6:
			issetting = false;
			break;
		}
	}
	EndBatchDraw();
	std::cin.sync();
}

//�ļ�����
void sefileread()
{
	std::ifstream file("setting.bin", std::ios::binary); // ���ļ��Խ��ж�ȡ  

	if (!file)
	{ // ����ļ��Ƿ�ɹ���  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//��ȡ
	file.read(reinterpret_cast<char*>(&set), sizeof(Setting));
	//�ر��ļ�
	file.close();
}

void sefilewrite()
{
	std::ofstream file("setting.bin", std::ios::binary); // ���ļ��Խ��ж�ȡ  

	if (!file)
	{ // ����ļ��Ƿ�ɹ���  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//��ȡ
	file.write(reinterpret_cast<char*>(&set), sizeof(Setting));
	//�ر��ļ�
	file.close();
}