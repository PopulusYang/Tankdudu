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
//星星
#define MAXSTAR 200	// 星星总数
//小星星
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

// 移动星星
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

/*
PROMISE of
allbox distrub:
[0]:player
[1]-[n]:OBS
[n+1]:enemies
 (ensure dynamic space for enemy generating)
*/



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

<<<<<<<<< Temporary merge branch 1
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
}*/


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
//near==true
bool isPointNear(int x1, int y1, int x2, int y2, int range) {
	return abs(x1 - x2) <= range && abs(y1 - y2) <= range;
}

bool angleDectect(const ColliderBox& box1, const ColliderBox& box2, int range) {
	// 获取 box1 和 box2 的四个角的坐标
	int box1Corners[4][2] = {
		{box1.mx, box1.my}, // 左上角
		{box1.mx + box1.width, box1.my}, // 右上角
		{box1.mx, box1.my + box1.height}, // 左下角
		{box1.mx + box1.width, box1.my + box1.height} // 右下角
	};

	int box2Corners[4][2] = {
		{box2.mx, box2.my}, // 左上角
		{box2.mx + box2.width, box2.my}, // 右上角
		{box2.mx, box2.my + box2.height}, // 左下角
		{box2.mx + box2.width, box2.my + box2.height} // 右下角
	};

	// 检查 box1 的每个角是否接近 box2 的任意一个角
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

/*
PROMISE of
allbox distrub:
[0]:player
[1]-[n]:OBS
[n+1]:enemies
 (ensure dynamic space for enemy generating)
*/



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
			button* b1 = new button(260, 230, 120, 50, "单人游戏");
			button* b2 = new button(260, 310, 120, 50, "双人游戏");
			button* b3 = new button(460, 390, 120, 50, "静 音");
			button* b4 = new button(260, 390, 120, 50, "退出游戏");
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
				settextstyle(36, 0, "华文隶书");
				settextcolor(WHITE);
				drawtext("游戏结束,任意键退出。感谢游玩", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
		if (!volume_jug)
		{
			button* b1 = new button(260, 230, 120, 50, "单人游戏");
			button* b2 = new button(260, 310, 120, 50, "双人游戏");
			button* b3 = new button(460, 390, 120, 50, "解除静音");
			button* b4 = new button(260, 390, 120, 50, "退出游戏");
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
				settextstyle(36, 0, "华文隶书");
				settextcolor(WHITE);
				drawtext("游戏结束,任意键退出。感谢游玩", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

		}
	}
	//以下是星星的代码
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
	// 按任意键退出
	closegraph();
	return 0;
}




