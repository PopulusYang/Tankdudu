//�ļ�����gamemain.cpp
//���ߣ������ԣ�����ã�������
//���ļ�Ϊ�������ڣ����ֺ�����ʵ���Լ��ļ��Ķ�д��


#include"tankclass.h"
#include"tankhead.h"
#define MAXSTAR 200	// ��������

class bullet;
std::mutex lock;
std::mutex lock2;
int IDnum = 0;
std::vector<ColliderBox> allbox;
std::vector<bullet> allbullet;
int volume_jug = 1;
int play_game = 1;


//����Ч��չʾ����
// https://codebus.cn/yangw/word-art-freeze
// ����ȫ�ֱ���
POINT* g_pDst;		// �㼯(Ŀ��)
POINT* g_pSrc;		// �㼯(Դ)
int g_nWidth;		// ���ֵĿ��
int g_nHeight;		// ���ֵĸ߶�
int g_nCount;		// �㼯�����ĵ������


// ��ȡĿ��㼯
void GetDstPoints()
{
	// ������ʱ��ͼ����
	IMAGE img;
	SetWorkingImage(&img);

	// ����Ŀ���ַ���
	TCHAR s[] = _T("PopulusYang");

	// ����Ŀ���ַ����Ŀ�ߣ���������ʱ��ͼ����ĳߴ�
	setcolor(BLUE);
	setfont(150, 0, _T("Blackadder ITC"));
	g_nWidth = textwidth(s);
	g_nHeight = textheight(s);
	Resize(&img, g_nWidth, g_nHeight);

	// ���Ŀ���ַ����� img ����
	outtextxy(0, 0, s);

	// ���㹹��Ŀ���ַ����ĵ������
	int x, y;
	g_nCount = 0;
	for (x = 0; x < g_nWidth; x++)
		for (y = 0; y < g_nHeight; y++)
			if (getpixel(x, y) == BLUE)
				g_nCount++;

	// ����Ŀ������
	g_pDst = new POINT[g_nCount];
	int i = 0;
	for (x = 0; x < g_nWidth; x++)
		for (y = 0; y < g_nHeight; y++)
			if (getpixel(x, y) == BLUE)
			{
				//������ܷ����Ĵ���
				if (i >= g_nCount)
					break;
				g_pDst[i].x = x + (640 - g_nWidth) / 2;
				g_pDst[i].y = y + (480 - g_nHeight) / 2;
				i++;
			}

	// �ָ�����Ļ�Ļ�ͼ����
	SetWorkingImage(NULL);
}


// ��ȡԴ�㼯
void GetSrcPoints()
{
	// �����������
	srand((unsigned int)time(NULL));

	// ���������Դ����
	g_pSrc = new POINT[g_nCount];
	for (int i = 0; i < g_nCount; i++)
	{
		g_pSrc[i].x = rand() % 640;
		g_pSrc[i].y = rand() % 480;
	}
}


// ȫ��ģ������(������Ļ��һ�к����һ��)
void Blur(DWORD* pMem)
{
	for (int i = 640; i < 640 * 479; i++)
	{
		pMem[i] = RGB(
			(GetRValue(pMem[i]) + GetRValue(pMem[i - 640]) + GetRValue(pMem[i - 1]) + GetRValue(pMem[i + 1]) + GetRValue(pMem[i + 640])) / 5+1,
			(GetGValue(pMem[i]) + GetGValue(pMem[i - 640]) + GetGValue(pMem[i - 1]) + GetGValue(pMem[i + 1]) + GetGValue(pMem[i + 640])) / 5,
			(GetBValue(pMem[i]) + GetBValue(pMem[i - 640]) + GetBValue(pMem[i - 1]) + GetBValue(pMem[i + 1]) + GetBValue(pMem[i + 640])) / 5);
	}
}

void showFreeze()
{
	DWORD* pBuf = GetImageBuffer();		// ��ȡ��ʾ������ָ��
	GetDstPoints();						// ��ȡĿ��㼯
	GetSrcPoints();						// ��ȡԴ�㼯

	// ����
	int x, y;
	for (int i = 2; i <= 256; i += 2)
	{
		COLORREF c = RGB(i - 1, i - 1, i - 1);
		Blur(pBuf);						// ȫ��ģ������

		for (int d = 0; d < g_nCount; d++)
		{
			x = g_pSrc[d].x + (g_pDst[d].x - g_pSrc[d].x) * i / 256;
			y = g_pSrc[d].y + (g_pDst[d].y - g_pSrc[d].y) * i / 256;
			pBuf[y * 640 + x] = c;		// ֱ�Ӳ�����ʾ����������
		}

		Sleep(20);						// ��ʱ
	}

	// �����ڴ�
	delete g_pDst;
	delete g_pSrc;
}

bool pause = false;

void Pause(bool *isgaming)
{
	//�����
	setfillcolor(0x9BB171);
	fillrectangle(160, 155, 485, 320);
	//��ʾ����
	RECT top{ 160,160,485,200 };
	settextcolor(BLACK);
	settextstyle(44, 0, "��������");
	drawtext("��  ͣ", &top, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//������ť
	button b1(195, 250, 100, 35, "��  ��");
	button b2(355, 250, 100, 35, "��  ��");
	b1.create();
	b2.create();
	//�ȴ���ť����
	int choose = 0;
	bool jug = true;
	ExMessage msg;
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
				if (b1.test(msg))
					choose = 1;
				if (b2.test(msg))
					choose = 2;
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
		*isgaming = false;
		break;
	case 2:
		break;
	}
	//��ʼ��pause
	pause = false;
}

//�����Ƿ��ɳɾ͵Ľṹ��
typedef struct Achievement
{
	bool a1 = false, a2 = false, a3 = false, a4 = false;
} Achieve;

typedef struct Setting
{
	bool sound = true;
	int background = 3;
	int gametime = 2;
} Setting;

Achieve achieve;
Setting set;

//С����
struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};
typedef struct allscore
{
	int score1;
	int score2;
}allscore;

std::vector<allscore> scores;


void fileread()
{
	std::ifstream file("scores.bin", std::ios::binary); // ���ļ��Խ��ж�ȡ  

	if (!file)
	{ // ����ļ��Ƿ�ɹ���  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//��ʱ�������
	int score1 = 0;
	int score2 = 0;

	while (file)
	{
		//��ȡ
		file.read(reinterpret_cast<char*>(&score1), sizeof(score1));
		file.read(reinterpret_cast<char*>(&score2), sizeof(score2));
		//�����ڴ�
		scores.push_back(allscore{ score1, score2 });
		if (file.gcount() == 0)
		{
			if (file.eof())
			{
				// ȷʵ��EOF���ļ���ȡ���  
				std::cout << "End of file reached." << std::endl;
				scores.pop_back();
				break;
			}
			else
			{
				// ��������������  
				std::cerr << "Error reading file." << std::endl;
				break;
			}
		}
	}
}


void filewrite()
{
	std::ofstream file("scores.bin", std::ios::binary); // ���ļ��Խ��ж�ȡ  

	if (!file)
	{ // ����ļ��Ƿ�ɹ���  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	if (scores.size() != 0)
	{
		for (allscore p : scores)
		{
			file.write(reinterpret_cast<char*>(&p.score1), sizeof(p.score1));
			file.write(reinterpret_cast<char*>(&p.score2), sizeof(p.score2));
		}
	}
	file.close();
}
STAR star[MAXSTAR];
void InitStar(int i)
{
	star[i].x = 0;
	star[i].y = rand() % 480;
	star[i].step = (rand() % 5000) / 1000.0 + 1;
	star[i].color = (int)(star[i].step * 255 / 6.0 + 0.5);
	star[i].color = RGB(star[i].color, star[i].color, star[i].color);
}

// �ƶ�����
void MoveStar(int i)
{
	putpixel((int)star[i].x, star[i].y, 0);
	star[i].x += star[i].step;
	if (star[i].x > 640)	InitStar(i);
	putpixel((int)star[i].x, star[i].y, star[i].color);
}



int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	// ���X���ϵ���ײ
	bool xOverlap = box1.mx + box1.displaceX < box2.mx + box2.width && box1.mx + box1.width > box2.mx + box2.displaceX;
	// ���Y���ϵ���ײ
	bool yOverlap = box1.my + box1.displaceY < box2.my + box2.height && box1.my + box1.height > box2.my + box2.displaceY;

	if (xOverlap && yOverlap)
	{
		// �ж���ײ���෢����X�ỹ��Y��
		double xOverlapAmount = min(box1.mx + box1.width - box2.mx - box2.displaceX, box2.mx + box2.width - box1.mx - box1.displaceX);
		double yOverlapAmount = min(box1.my + box1.height - box2.my - box2.displaceY, box2.my + box2.height - box1.my - box1.displaceY);

		if (xOverlapAmount < yOverlapAmount)
		{
			return 1; // X������ײ
		}
		else
		{
			return 2; // Y������ײ
		}
	}

	return 0; // û����ײ
}
//near==true
bool isPointNear(int x1, int y1, int x2, int y2, int range)
{
	return abs(x1 - x2) <= range && abs(y1 - y2) <= range;
}

bool angleDectect(const ColliderBox& box1, const ColliderBox& box2, int range)
{
	// ��ȡ box1 �� box2 ���ĸ��ǵ�����
	int box1Corners[4][2] = {
		{(int)(box1.mx + box1.displaceX), (int)(box1.my + box1.displaceY)}, // ���Ͻ�
		{(int)(box1.mx + box1.width), (int)(box1.my + box1.displaceY)}, // ���Ͻ�
		{(int)(box1.mx + box1.displaceX), (int)(box1.my + box1.height)}, // ���½�
		{(int)(box1.mx + box1.width),(int)(box1.my + box1.height)} // ���½�
	};

	int box2Corners[4][2] = {
		{(int)(box2.mx + box2.displaceX), (int)(box2.my + box2.displaceY)}, // ���Ͻ�
		{(int)(box2.mx + box2.width), (int)(box2.my + box2.displaceY)}, // ���Ͻ�
		{(int)(box2.mx + box2.displaceX), (int)(box2.my + box2.height)}, // ���½�
		{(int)(box2.mx + box2.width), (int)(box2.my + box2.height)} // ���½�
	};

	// ��� box1 ��ÿ�����Ƿ�ӽ� box2 ������һ����
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (isPointNear(box1Corners[i][0], box1Corners[i][1], box2Corners[j][0], box2Corners[j][1], range))
			{
				return true;
			}
		}
	}

	return false;
}


static void starting()
{
	// ��ô��ھ��
	HWND hWnd = GetHWnd();
	// ʹ�� Windows API �޸Ĵ�������
	SetWindowText(hWnd, "̹�˴�ս");
	//����ͼ��
	HICON newIcon = (HICON)LoadImage(NULL, TEXT("sorce/tankico.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)newIcon);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)newIcon);

	IMAGE img;
	loadimage(&img, "sorce/start.jpg", 640, 480, true);
	putimage(0, 0, &img);
	//չʾ����
	setbkmode(TRANSPARENT);
	RECT* title = new RECT{ 180,80,455,140 };
	settextcolor(BLACK);
	settextstyle(44, 0, "��������");
	//�������
	drawtext("̹�˴�սbiubiu", title, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	delete(title);
	//�ָ���
	setlinecolor(BLACK);
	line(140, 160, 500, 160);
	//������ť
	settextstyle(24, 0, "��������");
}




int main()
{
	RECT center = { 0,0,639,479 };
	Acfileread();
	sefileread();
	fileread();
	srand((unsigned)time(NULL));
	// ��ͼ���ڳ�ʼ��
	//Ϊ�˷�����ԣ��Ұѿ���̨�򿪣���ɺ�ص�����
	initgraph(640, 480);
	mciSendString("open music/start.wav alias start", NULL, 0, NULL);
	mciSendString("open music/fire.wav alias fire", NULL, 0, NULL);
	mciSendString("open music/blast.wav alias blast", NULL, 0, NULL);
	mciSendString("open music/bang.wav alias bang", NULL, 0, NULL);
	button* b1 = new button(260, 230, 120, 50, "������Ϸ");
	button* b2 = new button(260, 310, 120, 50, "˫����Ϸ");
	button* b3 = new button(460, 390, 120, 50, "�� ��");
	button* b5 = new button(260, 390, 120, 50, "�˳���Ϸ");
	button* b4 = new button(60, 390, 120, 50, "����ָ��");
	button* b6 = new button(460, 310, 120, 50, "�ɾ�");
	//չʾ������
	showFreeze();
	Sleep(1500);
	while (play_game)
	{
		starting();
		b1->create();
		b2->create();
		b3->create();
		b4->create();
		b5->create();
		b6->create();
		setfillcolor(0x9BB171);
		fillrectangle(70, 220, 180, 370);
		RECT position{ 70,220,180,240 };
		settextstyle(18, 0, "��������");
		int i = 0;
		drawtext("���˳ɼ�", &position, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		for (allscore p : scores)
		{
			if (i > 5)
				break;
			i++;
			RECT pos{ 70,220 + 20 * i,180,240 + 20 * i };
			char s[6] = { '\0' };
			s[0] = p.score1 / 10 + '0';
			s[1] = p.score1 % 10 + '0';
			s[2] = ':';
			s[3] = p.score2 / 10 + '0';
			s[4] = p.score2 % 10 + '0';
			drawtext(_T(s), &pos, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		int choose = 0;
		ExMessage msg;
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
					if (b1->test(msg))
						choose = 1;
					if (b2->test(msg))
						choose = 2;
					if (b3->test(msg))
						choose = 3;
					if (b4->test(msg))
						choose = 4;
					if (b5->test(msg))
						choose = 5;
					if (b6->test(msg))
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
			std::cout << "Button 1 has been pushed" << std::endl;
			if (volume_jug)
				mciSendString("play music/start.wav", 0, 0, 0);
			singlegame();
			break;
		case 2:
			std::cout << "Button 2 has been pushed" << std::endl;
			if (volume_jug)
				mciSendString("play music/start.wav", 0, 0, 0);
			couplelegame();
			break;
		case 3:
			setting();
			break;
		case 4:
			std::cout << "Button 4 has been pushed" << std::endl;
			operate();
			cleardevice();
			break;
		case 5:
			std::cout << "Button 5 has been pushed" << std::endl;
			play_game = 0;
			filewrite();
			Acfilewrite();
			sefilewrite();
			cleardevice();
			settextstyle(36, 0, "��������");
			settextcolor(WHITE);
			drawtext("��Ϸ����,������˳�����л����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case 6:
			std::cout << "Button 6 has been pushed" << std::endl;
			checkachieve();
		}
	}
	//�ͷſռ�
	delete(b1);
	delete(b2);
	delete(b3);
	delete(b4);
	delete(b5);
	delete(b6);
	
	//���������ǵĴ���
	for (int i = 0; i < MAXSTAR; i++)
	{
		InitStar(i);
		star[i].x = rand() % 640;
	}
	ExMessage msg;
	while (!peekmessage(&msg, EX_KEY))
	{
		for (int i = 0; i < MAXSTAR; i++)
			MoveStar(i);
		drawtext("��Ϸ����,������˳�����л����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Sleep(20);
	}
	// ��������˳�
	closegraph();
	return 0;
}