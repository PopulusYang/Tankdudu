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


//创建按钮类
class button
{
private:
	//位置信息和大小信息
	int x;
	int y;
	int w;
	int h;
	LPCSTR str;
private:
	//创建并显示按钮
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
	//检测一次按钮是否被按下
	inline bool test(ExMessage msg) const
	{
		if (msg.x >= x && msg.x <= x + w && msg.y >= y && msg.y <= y + h)
			return true;
		else
			return false;
	}
};


//检测是否碰撞,只需传入两个对象的碰撞箱对象，即可检测是否碰撞
/*bool ColliderDectect(const ColliderBox& box1, const ColliderBox& box2) {
	if (!((box1.mx > box2.mx + box2.width) &&//右边
		(box1.mx + box1.width < box2.mx) &&//左边
		(box1.my > box2.my + box2.height) &&//下边
		(box1.my + box1.height < box2.my))//上边
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
	if (box1.mx >= box2.mx && box1.mx <= box2.mx + box2.width && box1.my >= box2.my && box1.my <= box2.my + box2.height)//左下
		jug += 1;
	if (box1.mx >= box2.mx && box1.mx <= box2.mx + box2.width && box1.my + box1.height >= box2.my && box1.my + box1.height <= box2.my + box2.height)//左上
		jug += 2;
	if (box1.mx + box1.width >= box2.mx && box1.mx + box1.width <= box2.mx + box2.width && box1.my >= box2.my && box1.my <= box2.my + box2.height)//右下
		jug += 4;
	if (box1.mx + box1.width >= box2.mx && box1.mx + box1.width <= box2.mx + box2.width && box1.my + box1.height >= box2.my && box1.my + box1.height <= box2.my + box2.height)//右上
		jug += 8;
	return jug;
}
*/

int dustiColliderDectect(const ColliderBox& box1, const ColliderBox& box2) // AABB - AABB collision
{
	int jug = 0;//此为返回值
	// x轴方向碰撞？
	bool collisionX = box1.mx + box1.width >= box2.mx &&
		box2.mx + box2.width >= box1.mx;
	// y轴方向碰撞？
	bool collisionY = box1.my + box1.height >= box2.my &&
		box2.my + box2.height >= box1.my;
	// 只有两个轴向都有碰撞时才碰撞
	return collisionX && collisionY;
}
int dustiiColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	
		// 检测本BOX的右边是否与别的碰撞箱发生碰撞
		if (box1.mx + box1.width > box2.mx && box1.mx < box2.mx &&
			(box1.my > box2.my - box2.height && box1.my - box1.height < box2.my)) {
			return 114;
		}
		// 检测本BOX的左边是否与别的碰撞箱发生碰撞
		 if (box1.mx < box2.mx + box2.width && box1.mx > box2.mx &&
			(box1.my > box2.my -box2.height &&box1.my - box1.height <box2.my)) {
			return 514;
		}
		// 检测本BOX的上边是否与别的碰撞箱发生碰撞
		 if (box1.my < box2.my + box2.height && box1.my + box1.height > box2.my&&
			 (box1.mx < box2.mx + box2.width && box1.mx + box1.width > box2.mx))
		 {
			return 191;
		}
		// 检测本BOX的下边是否与别的碰撞箱发生碰撞
		 if (box1.my -box1.height< box2.my &&
			 (box1.mx < box2.mx + box2.width && box1.mx + box1.width > box2.mx))
		 {
			return 810;
		 }
	
	return 0;
}
int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	// 检测X轴上的碰撞
	bool xOverlap = box1.mx < box2.mx + box2.width && box1.mx + box1.width > box2.mx;
	// 检测Y轴上的碰撞
	bool yOverlap = box1.my < box2.my + box2.height && box1.my + box1.height > box2.my;

	if (xOverlap && yOverlap) {
		// 判断碰撞更多发生在X轴还是Y轴
		float xOverlapAmount = min(box1.mx + box1.width - box2.mx, box2.mx + box2.width - box1.mx);
		float yOverlapAmount = min(box1.my + box1.height - box2.my, box2.my + box2.height - box1.my);

		if (xOverlapAmount < yOverlapAmount) {
			return 1; // X轴上碰撞
		}
		else {
			return 2; // Y轴上碰撞
		}
	}

	return 0; // 没有碰撞
}
void starting()
{
	// 绘图窗口初始化
	//为了方便调试，我把控制台打开，完成后关掉即可
	
	initgraph(640, 480, EX_SHOWCONSOLE);
	// 获得窗口句柄
	HWND hWnd = GetHWnd();
	// 使用 Windows API 修改窗口名称
	SetWindowText(hWnd, "坦克大战");
	//设置图标
	HICON newIcon = (HICON)LoadImage(NULL, TEXT("sorce/tankico.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)newIcon);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)newIcon);

	IMAGE img;
	loadimage(&img, "sorce/start.jpg", 640, 480, true);
	putimage(0, 0, &img);
	//展示标题
	setbkmode(TRANSPARENT);
	RECT* title = new RECT{ 180,80,455,140 };
	settextcolor(BLACK);
	settextstyle(44, 0, "华文隶书");
	//输出标题
	drawtext("坦克大战biubiu", title, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	delete(title);
	//分割线
	setlinecolor(BLACK);
	line(140, 160, 500, 160);
	//三个按钮
	settextstyle(24, 0, "华文隶书");
}

int main(int argc, char* argv[])
{
	starting();
	button* b1 = new button(260, 230, 120, 50, "单人游戏");
	button* b2 = new button(260, 310, 120, 50, "双人游戏");
	button* b3 = new button(260, 390, 120, 50, "退出游戏");
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
		settextstyle(36, 0, "华文隶书");
		settextcolor(WHITE);
		drawtext("游戏结束,任意键退出。感谢游玩", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		_getch();
	}
	// 按任意键退出
	closegraph();
	return 0;
}




