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
typedef struct allscore
{
	int score1;
	int score2;
}allscore;

std::vector<allscore> scores;


void fileread()
{
	std::ifstream file("scores.bin", std::ios::binary); // 打开文件以进行读取  

	if (!file) { // 检查文件是否成功打开  
		std::cerr << "Unable to open file";
		exit(-1);
	}
	//临时储存分数
	int score1 = 0;
	int score2 = 0;

	while (file) 
	{ 
		//读取
		file.read(reinterpret_cast<char*>(&score1), sizeof(score1));
		file.read(reinterpret_cast<char*>(&score2), sizeof(score2));
		//存入内存
		scores.push_back(allscore{ score1, score2 });
		if (file.gcount() == 0)
		{
			if (file.eof())
			{
				// 确实是EOF，文件读取完毕  
				std::cout << "End of file reached." << std::endl;
				scores.pop_back();
				break;
			}
			else
			{
				// 出现了其他错误  
				std::cerr << "Error reading file." << std::endl;
				break;
			}
		}
	}
}


void filewrite()
{
	std::ofstream file("scores.bin", std::ios::binary); // 打开文件以进行读取  

	if (!file) { // 检查文件是否成功打开  
		std::cerr << "Unable to open file";
		exit(-1);
	}

	for (allscore p : scores)
	{
		file.write(reinterpret_cast<char*>(&p.score1), sizeof(p.score1));
		file.write(reinterpret_cast<char*>(&p.score2), sizeof(p.score2));
	}
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

// 移动星星
void MoveStar(int i)
{
	putpixel((int)star[i].x, star[i].y, 0);
	star[i].x += star[i].step;
	if (star[i].x > 640)	InitStar(i);
	putpixel((int)star[i].x, star[i].y, star[i].color);
}



int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2)
{
	// 检测X轴上的碰撞
	bool xOverlap = box1.mx < box2.mx + box2.width && box1.mx + box1.width > box2.mx;
	// 检测Y轴上的碰撞
	bool yOverlap = box1.my < box2.my + box2.height && box1.my + box1.height > box2.my;

	if (xOverlap && yOverlap) {
		// 判断碰撞更多发生在X轴还是Y轴
		double xOverlapAmount = min(box1.mx + box1.width - box2.mx, box2.mx + box2.width - box1.mx);
		double yOverlapAmount = min(box1.my + box1.height - box2.my, box2.my + box2.height - box1.my);

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
		{(int)box1.mx, (int)box1.my}, // 左上角
		{(int)box1.mx + box1.width, (int)box1.my}, // 右上角
		{(int)box1.mx, (int)box1.my + box1.height}, // 左下角
		{(int)box1.mx + box1.width,(int)box1.my + box1.height} // 右下角
	};

	int box2Corners[4][2] = {
		{(int)box2.mx, (int)box2.my}, // 左上角
		{(int)box2.mx + box2.width, (int)box2.my}, // 右上角
		{(int)box2.mx, (int)box2.my + box2.height}, // 左下角
		{(int)box2.mx + box2.width, (int)box2.my + box2.height} // 右下角
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


static void starting()
{
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




int main()
{
	fileread();
	srand((unsigned)time(NULL));
	// 绘图窗口初始化
	//为了方便调试，我把控制台打开，完成后关掉即可
	initgraph(640, 480, EX_SHOWCONSOLE);
	mciSendString("open music/start.wav alias start", NULL, 0, NULL);
	mciSendString("open music/fire.wav alias fire", NULL, 0, NULL);
	mciSendString("open music/blast.wav alias blast", NULL, 0, NULL);
	mciSendString("open music/bang.wav alias bang", NULL, 0, NULL);
	button* b1 = new button(260, 230, 120, 50, "单人游戏");
	button* b2 = new button(260, 310, 120, 50, "双人游戏");
	button* b3 = new button(460, 390, 120, 50, "静 音");
	button* b5 = new button(260, 390, 120, 50, "退出游戏");
	button* b4 = new button(60, 390, 120, 50, "操作指南");
	while (play_game)
	{
		starting();
		b1->create();
		b2->create();
		b3->create();
		b4->create();
		b5->create();
		setfillcolor(0x9BB171);
		fillrectangle(70, 220, 180, 370);
		RECT position{ 70,220,180,240 };
		settextstyle(18, 0, "华文隶书");
		int i = 0;
		drawtext("单人成绩", &position, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		for (allscore p : scores)
		{
			if (i > 5)
				break;
			i++;
			RECT pos{ 70,220 + 20 * i,180,240 + 20 * i };
			char s[6] = { '\0'};
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
			//获取鼠标消息，检测是否按在了按钮上
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
			std::cout << "Button 3 has been pushed" << std::endl;
			volume_jug = (volume_jug + 1) % 2;
			if (volume_jug)
				b3->changetext("静音");
			else
				b3->changetext("解除静音");
			b3->create();
			break;
		case 4:
			operate();
			cleardevice();
			break;
		case 5:
			std::cout << "Button 4 has been pushed" << std::endl;
			play_game = 0;
			cleardevice();
			RECT center = { 0,0,639,479 };
			settextstyle(36, 0, "华文隶书");
			settextcolor(WHITE);
			drawtext("游戏结束,任意键退出。感谢游玩", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	//释放空间
	delete(b1);
	delete(b2);
	delete(b3);
	delete(b4);

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
	filewrite();
	return 0;
}