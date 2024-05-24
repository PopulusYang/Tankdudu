#include"tankclass.h"
#include"tankhead.h"

class bullet;
std::mutex lock;
std::mutex lock2;
int IDnum = 0;
std::vector<ColliderBox> allbox;
std::vector<bullet> allbullet;
int volume_jug = 1;
int play_game = 1;
//����
#define MAXSTAR 200	// ��������
//С����
struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};

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
/*
int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2) // AABB - AABB collision
{
	int jug = 0;//��Ϊ����ֵ
	// x�᷽����ײ��
	bool collisionX = box1.mx + box1.width >= box2.mx &&
		box2.mx + box2.width >= box1.mx;
	// y�᷽����ײ��
	bool collisionY = box1.my + box1.height >= box2.my &&
		box2.my + box2.height >= box1.my;
	// ֻ��������������ײʱ����ײ
	return collisionX && collisionY;
}

void starting()
{
	// ��ͼ���ڳ�ʼ��
	//Ϊ�˷�����ԣ��Ұѿ���̨�򿪣���ɺ�ص�����

	initgraph(640, 480, EX_SHOWCONSOLE);
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

/*
PROMISE of
allbox distrub:
[0]:player
[1]-[n]:OBS
[n+1]:enemies
 (ensure dynamic space for enemy generating)
*/



//����Ƿ���ײ,ֻ�贫�������������ײ����󣬼��ɼ���Ƿ���ײ
/*bool ColliderDectect(const ColliderBox& box1, const ColliderBox& box2) {
	if (!((box1.mx > box2.mx + box2.width) &&//�ұ�
		(box1.mx + box1.width < box2.mx) &&//���
		(box1.my > box2.my + box2.height) &&//�±�
		(box1.my + box1.height < box2.my))//�ϱ�
		)
	{
		return true;
	};
	return false;
}*/
/*
//if collider, return true
int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	int jug = 0;
	if (box1.mx >= box2.mx && box1.mx <= box2.mx + box2.width && box1.my >= box2.my && box1.my <= box2.my + box2.height)//����
		jug += 1;
	if (box1.mx >= box2.mx && box1.mx <= box2.mx + box2.width && box1.my + box1.height >= box2.my && box1.my + box1.height <= box2.my + box2.height)//����
		jug += 2;
	if (box1.mx + box1.width >= box2.mx && box1.mx + box1.width <= box2.mx + box2.width && box1.my >= box2.my && box1.my <= box2.my + box2.height)//����
		jug += 4;
	if (box1.mx + box1.width >= box2.mx && box1.mx + box1.width <= box2.mx + box2.width && box1.my + box1.height >= box2.my && box1.my + box1.height <= box2.my + box2.height)//����
		jug += 8;
	return jug;
}
*/

<<<<<<<<< Temporary merge branch 1
int dustiColliderDectect(const ColliderBox& box1, const ColliderBox& box2) // AABB - AABB collision
{
	int jug = 0;//��Ϊ����ֵ
	// x�᷽����ײ��
	bool collisionX = box1.mx + box1.width >= box2.mx &&
		box2.mx + box2.width >= box1.mx;
	// y�᷽����ײ��
	bool collisionY = box1.my + box1.height >= box2.my &&
		box2.my + box2.height >= box1.my;
	// ֻ��������������ײʱ����ײ
	return collisionX && collisionY;
}
int dustiiColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	
		// ��ⱾBOX���ұ��Ƿ�������ײ�䷢����ײ
		if (box1.mx + box1.width > box2.mx && box1.mx < box2.mx &&
			(box1.my > box2.my - box2.height && box1.my - box1.height < box2.my)) {
			return 114;
		}
		// ��ⱾBOX������Ƿ�������ײ�䷢����ײ
		 if (box1.mx < box2.mx + box2.width && box1.mx > box2.mx &&
			(box1.my > box2.my -box2.height &&box1.my - box1.height <box2.my)) {
			return 514;
		}
		// ��ⱾBOX���ϱ��Ƿ�������ײ�䷢����ײ
		 if (box1.my < box2.my + box2.height && box1.my + box1.height > box2.my&&
			 (box1.mx < box2.mx + box2.width && box1.mx + box1.width > box2.mx))
		 {
			return 191;
		}
		// ��ⱾBOX���±��Ƿ�������ײ�䷢����ײ
		 if (box1.my -box1.height< box2.my &&
			 (box1.mx < box2.mx + box2.width && box1.mx + box1.width > box2.mx))
		 {
			return 810;
		 }
	
	return 0;
}*/


int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	// ���X���ϵ���ײ
	bool xOverlap = box1.mx < box2.mx + box2.width && box1.mx + box1.width > box2.mx;
	// ���Y���ϵ���ײ
	bool yOverlap = box1.my < box2.my + box2.height && box1.my + box1.height > box2.my;

	if (xOverlap && yOverlap) {
		// �ж���ײ���෢����X�ỹ��Y��
		float xOverlapAmount = min(box1.mx + box1.width - box2.mx, box2.mx + box2.width - box1.mx);
		float yOverlapAmount = min(box1.my + box1.height - box2.my, box2.my + box2.height - box1.my);

		if (xOverlapAmount < yOverlapAmount) {
			return 1; // X������ײ
		}
		else {
			return 2; // Y������ײ
		}
	}

	return 0; // û����ײ
}
//near==true
bool isPointNear(int x1, int y1, int x2, int y2, int range) {
	return abs(x1 - x2) <= range && abs(y1 - y2) <= range;
}

bool angleDectect(const ColliderBox& box1, const ColliderBox& box2, int range) {
	// ��ȡ box1 �� box2 ���ĸ��ǵ�����
	int box1Corners[4][2] = {
		{box1.mx, box1.my}, // ���Ͻ�
		{box1.mx + box1.width, box1.my}, // ���Ͻ�
		{box1.mx, box1.my + box1.height}, // ���½�
		{box1.mx + box1.width, box1.my + box1.height} // ���½�
	};

	int box2Corners[4][2] = {
		{box2.mx, box2.my}, // ���Ͻ�
		{box2.mx + box2.width, box2.my}, // ���Ͻ�
		{box2.mx, box2.my + box2.height}, // ���½�
		{box2.mx + box2.width, box2.my + box2.height} // ���½�
	};

	// ��� box1 ��ÿ�����Ƿ�ӽ� box2 ������һ����
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (isPointNear(box1Corners[i][0], box1Corners[i][1], box2Corners[j][0], box2Corners[j][1], range)) {
				return true;
			}
		}
	}

	return false;
}


void starting()
{
	// ��ͼ���ڳ�ʼ��
	//Ϊ�˷�����ԣ��Ұѿ���̨�򿪣���ɺ�ص�����

	initgraph(640, 480, EX_SHOWCONSOLE);
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

/*
PROMISE of
allbox distrub:
[0]:player
[1]-[n]:OBS
[n+1]:enemies
 (ensure dynamic space for enemy generating)
*/



//����Ƿ���ײ,ֻ�贫�������������ײ����󣬼��ɼ���Ƿ���ײ
/*bool ColliderDectect(const ColliderBox& box1, const ColliderBox& box2) {
	if (!((box1.mx > box2.mx + box2.width) &&//�ұ�
		(box1.mx + box1.width < box2.mx) &&//���
		(box1.my > box2.my + box2.height) &&//�±�
		(box1.my + box1.height < box2.my))//�ϱ�
		)
	{
		return true;
	};
	return false;
}*/
/*
//if collider, return true
int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	int jug = 0;
	if (box1.mx >= box2.mx && box1.mx <= box2.mx + box2.width && box1.my >= box2.my && box1.my <= box2.my + box2.height)//����
		jug += 1;
	if (box1.mx >= box2.mx && box1.mx <= box2.mx + box2.width && box1.my + box1.height >= box2.my && box1.my + box1.height <= box2.my + box2.height)//����
		jug += 2;
	if (box1.mx + box1.width >= box2.mx && box1.mx + box1.width <= box2.mx + box2.width && box1.my >= box2.my && box1.my <= box2.my + box2.height)//����
		jug += 4;
	if (box1.mx + box1.width >= box2.mx && box1.mx + box1.width <= box2.mx + box2.width && box1.my + box1.height >= box2.my && box1.my + box1.height <= box2.my + box2.height)//����
		jug += 8;
	return jug;
}
*/



int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));
	starting();
	mciSendString("open music/start.wav alias start", NULL, 0, NULL);
	mciSendString("open music/fire.wav alias fire", NULL, 0, NULL);
	mciSendString("open music/blast.wav alias blast", NULL, 0, NULL);
	mciSendString("open music/bang.wav alias bang", NULL, 0, NULL);
	mciSendString("play music/start.wav" , 0, 0, 0);
	while (play_game)
	{
		if (volume_jug)
		{
			button* b1 = new button(260, 230, 120, 50, "������Ϸ");
			button* b2 = new button(260, 310, 120, 50, "˫����Ϸ");
			button* b3 = new button(460, 390, 120, 50, "�� ��");
			button* b4 = new button(260, 390, 120, 50, "�˳���Ϸ");
			int choose = 0;
			ExMessage msg;
			bool jug = true;
			while (jug)
			{
				HWND hWnd = GetHWnd();
				if (!IsWindow(hWnd))
					exit(0);
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
						if (choose)
							jug = false;
						break;
					default:
						break;
					}
				}
			}
			delete(b1);
			delete(b2);
			delete(b3);
			delete(b4);
			switch (choose)
			{
			case 1:
				std::cout << "Button 1 has been pushed" << std::endl;
				singlegame();
				break;
			case 2:
				std::cout << "Button 2 has been pushed" << std::endl;
				couplelegame();
				break;
			case 3:
				std::cout << "Button 3 has been pushed" << std::endl;
				volume_jug = (volume_jug + 1) % 2;
				break;
			case 4:
				std::cout << "Button 4 has been pushed" << std::endl;
				play_game = 0;
				cleardevice();
				RECT center = { 0,0,639,479 };
				settextstyle(36, 0, "��������");
				settextcolor(WHITE);
				drawtext("��Ϸ����,������˳�����л����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
		if (!volume_jug)
		{
			button* b1 = new button(260, 230, 120, 50, "������Ϸ");
			button* b2 = new button(260, 310, 120, 50, "˫����Ϸ");
			button* b3 = new button(460, 390, 120, 50, "�������");
			button* b4 = new button(260, 390, 120, 50, "�˳���Ϸ");
			int choose = 0;
			ExMessage msg;
			bool jug = true;
			while (jug)
			{
				HWND hWnd = GetHWnd();
				if (!IsWindow(hWnd))
					exit(0);
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
						if (choose)
							jug = false;
						break;
					default:
						break;
					}
				}
			}
			delete(b1);
			delete(b2);
			delete(b3);
			delete(b4);
			switch (choose)
			{
			case 1:
				std::cout << "Button 1 has been pushed" << std::endl;
				singlegame();
				break;
			case 2:
				std::cout << "Button 2 has been pushed" << std::endl;
				couplelegame();
				break;
			case 3:
				std::cout << "Button 3 has been pushed" << std::endl;
				volume_jug = (volume_jug + 1) % 2;
				break;
			case 4:
				std::cout << "Button 4 has been pushed" << std::endl;
				play_game = 0;
				cleardevice();
				RECT center = { 0,0,639,479 };
				settextstyle(36, 0, "��������");
				settextcolor(WHITE);
				drawtext("��Ϸ����,������˳�����л����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

		}
	}
	//���������ǵĴ���
	for (int i = 0; i < MAXSTAR; i++)
	{
		InitStar(i);
		star[i].x = rand() % 640;
	}
	while (!_kbhit())
	{
		for (int i = 0; i < MAXSTAR; i++)
			MoveStar(i);
		Sleep(20);
	}
	// ��������˳�
	closegraph();
	return 0;
}




