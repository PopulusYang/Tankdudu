#include"tankclass.h"
#include"tankhead.h"
class bullet;
std::mutex lock;
std::mutex lock2;
int IDnum = 0;
std::vector<ColliderBox> allbox;
std::vector<bullet> allbullet;

/*
PROMISE of
allbox distrub:
[0]:player
[1]-[n]:OBS
[n+1]:enemies
 (ensure dynamic space for enemy generating)
*/


//������ť��
class button
{
private:
	//λ����Ϣ�ʹ�С��Ϣ
	int x;
	int y;
	int w;
	int h;
	LPCSTR str;
private:
	//��������ʾ��ť
	void create(int x, int y, int w, int h, LPCSTR str)
	{
		setbkmode(TRANSPARENT);
		setfillcolor(0x9BB171);
		fillroundrect(x, y, x + w, y + h, 10, 10);
		RECT r = { x,y,x + w,y + h };
		drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
public:
	button(int x, int y, int w, int h, LPCSTR str) :x(x), y(y), w(w), h(h), str(str)
	{
		create(x, y, w, h, str);
		std::cout << "A button has been created" << std::endl;
	}
	~button()
	{
		std::cout << "The button has been deleted." << std::endl;
	}
	//���һ�ΰ�ť�Ƿ񱻰���
	inline bool test(ExMessage msg) const
	{
		if (msg.x >= x && msg.x <= x + w && msg.y >= y && msg.y <= y + h)
			return true;
		else
			return false;
	}
};


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
}
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

int main(int argc, char* argv[])
{
	starting();
	button* b1 = new button(260, 230, 120, 50, "������Ϸ");
	button* b2 = new button(260, 310, 120, 50, "˫����Ϸ");
	button* b3 = new button(260, 390, 120, 50, "�˳���Ϸ");
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
	switch (choose)
	{
	case 1:
		std::cout << "Button 1 has been pushed" << std::endl;
		singlegame();
		break;
	case 2:
		std::cout << "Button 2 has been pushed" << std::endl;
		break;
	case 3:
		std::cout << "Button 3 has been pushed" << std::endl;
		cleardevice();
		RECT center = { 0,0,639,479 };
		settextstyle(36, 0, "��������");
		settextcolor(WHITE);
		drawtext("��Ϸ����,������˳�����л����", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		_getch();
	}
	// ��������˳�
	closegraph();
	return 0;
}




