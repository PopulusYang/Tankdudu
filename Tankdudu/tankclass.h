#pragma once
//最大生命值
#define MAXHEALTH 100
//障碍物的超级血量
#define SUPER_OBSTACLE 9999
//最长游戏时间
#define MAXTIME 60
#define PI 3.14159265359
#include"tankhead.h"
#include <random>
//666
#include<array>
#include <time.h>
#include<mutex>
extern std::mutex lock;
extern std::mutex lock2;
extern bool isgaming;
class bullet;
extern std::vector<bullet> allbullet;

class Function
{
public:
	// transparentimage 函数：
		// 根据 png 的 alpha 信息实现半透明贴图（基于直接操作显示缓冲区）
		//x,y图片左上角坐标
	static void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
	{
		DWORD* dst = GetImageBuffer(dstimg);
		DWORD* src = GetImageBuffer(srcimg);
		int src_width = srcimg->getwidth();
		int src_height = srcimg->getheight();
		int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
		int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());
		int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;
		int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;
		if (x < 0)
		{
			src += -x;
			iwidth -= -x;
			x = 0;
		}
		if (y < 0)
		{
			src += src_width * -y;
			iheight -= -y;
			y = 0;
		}
		dst += dst_width * y + x;
		for (int iy = 0; iy < iheight; iy++)
		{
			for (int ix = 0; ix < iwidth; ix++)
			{
				int sa = ((src[ix] & 0xff000000) >> 24);
				int sr = ((src[ix] & 0xff0000) >> 16);
				int sg = ((src[ix] & 0xff00) >> 8);
				int sb = src[ix] & 0xff;
				int dr = ((dst[ix] & 0xff0000) >> 16);
				int dg = ((dst[ix] & 0xff00) >> 8);
				int db = dst[ix] & 0xff;
				dst[ix] = ((sr + dr * (255 - sa) / 255) << 16)
					| ((sg + dg * (255 - sa) / 255) << 8)
					| (sb + db * (255 - sa) / 255);
			}
			dst += dst_width;
			src += src_width;
		}
	}
};
class Vec//方向向量
{
public:

	double x;
	double y;
	int angle;
	Vec(double x, double y) :x(x), y(y)
	{
		// 使用atan2计算反正切值，结果为弧度  
		double radians = atan2(y, x);
		// 将弧度转换为度  
		angle = (int)(radians * 180.0 / std::acos(-1.0));

	}
	Vec() :x(0.0), y(1.0), angle(0) {}
	void roundchange(int userKey)
	{
		switch (userKey)
		{
		case 2://沿着向量方向增加
			angle += 6;
			y = std::sin(PI / 180 * angle);
			x = std::cos(PI / 180 * angle);
			break;
		case 1:
			angle -= 6;
			y = std::sin(PI / 180 * angle);
			x = std::cos(PI / 180 * angle);
			break;

		}
		if (angle >= 360)
		{
			angle -= 360;
		}
		if (angle < 0)
		{
			angle += 360;
		}
	}
};
class ColliderBox;
extern std::vector<ColliderBox> allbox;
class Entity;
//碰撞箱
class ColliderBox
{
public:
	//坐标以及长宽
	ColliderBox() :mx(0), my(0), height(0), width(0) {};
	ColliderBox(double x, double y, int h, int w) :mx(x), my(y), height(h), width(w) {
		allbox.push_back(*this);
	};
	friend bool ColliderDectect(const ColliderBox& box1, const ColliderBox& box2);
	static inline void drawColliderbox(ColliderBox obj);
	virtual ~ColliderBox() {};
protected:
	double mx;
	double my;
	int width;
	int height;
};

//调试模式，绘制碰撞箱
void ColliderBox::drawColliderbox(ColliderBox obj)
{
	setfillcolor(WHITE);
	rectangle((int)(obj.mx) + 21, (int)(obj.my) + 20, (int)(obj.mx + obj.width) - 25, (int)(obj.my + obj.height) - 15);
}

class Entity :public  ColliderBox//继承了碰撞箱的属性
{
protected:
	int mhealth;//健康值/血量
	//位置
	double speed;
	bool IsAlive;
	Vec vec;//方向
	ColliderBox mybox;

public:
	//默认构造函数
	Entity() :mhealth(MAXHEALTH), speed(0), IsAlive(true) {};
	//x坐标，y坐标，宽度，高度，速度，生命值
	Entity(int x, int y, int w, int h, int s, int health, Vec vec) :mhealth(health), ColliderBox(x, y, h, w), mybox(x, y, h, w), speed(s), IsAlive(true), vec(vec) {};
	Entity(int x, int y, int w, int h, int s, int health) :mhealth(health), ColliderBox(x, y, h, w), mybox(x, y, h, w), speed(s), IsAlive(true) {};
	virtual ~Entity() {}
	virtual void Dead() = 0;//死亡
	virtual void Move(int) = 0;//移动
};

class obstacle : public Entity//障碍物
{
private:
	int kind;
	//下面这行为输入图片
	//三种不同的障碍物（3为不可击穿的）
	IMAGE img1, img2, img3;
public:
	//原点指图片左上角
	//原点的x坐标，原点的y坐标，宽度，高度,速度，血量,障碍物类型
	obstacle(int x, int y, int w, int h, int s, int blood, int kind) :Entity(x, y, w, h, 0, s), kind(kind)
	{
		//此处的载入图片需要改成三种障碍物
		loadimage(&img1, "sorce/wall1.png", w, h);
		loadimage(&img2, "sorce/wall2.png", w, h);
		loadimage(&img3, "sorce/wall3.png", w, h);
		//此处被注释掉的代码应该在调用函数时确定kind类型时候使用
		/*std::random_device rd;  // 获取随机数种子
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(1, 2);
		kind = distrib(gen);*/
	}
	obstacle() :Entity(0, 0, 0, 0, 0, 0), kind(0) {}
	void Dead() override
	{
		if (IsAlive)
		{
			//不再成为实体，修改图像或者删除图像
		}
		if (!IsAlive)
		{

		}
	}
	void Move(int) override {}
	void display()
	{
		IMAGE temp;
		switch (kind)
		{
		case 1:
			temp = img1;
			break;
		case 2:
			temp = img2;
			break;
		case 3:
			temp = img3;
		}
		putimage((int)mx, (int)my, &temp);
	}
};
class bullet : public Entity//子弹类
{
private://private function
	bool alive() const
	{
		size_t size = allbox.size();
		bool flag = 1;
		for (int i = 0; i < size; i++)
		{
			if (ColliderDectect(this->mybox, allbox[i]))
			{
				flag = 0;
				break;
			}
		}
		if (!flag) {
			return false;
		}
		return true;
	}
private:
	int kind;
public:
	//初始x坐标，初始y坐标，宽度，高度，速度,种类,
	bullet(double x, double y, int kind, Vec vec) :Entity(x, y, 3, 3, 10.0, 1, vec), kind(kind) {}
	void Move(int) override {}
	void Dead() override {}
	static void bullMove(int isgaming)
	{
		while (isgaming)
		{
			for (bullet& p : allbullet)
			{
				p.mx += p.vec.x * p.speed;
				p.my += p.vec.y * p.speed;
				p.speed -= 0.08;
			}
			checkDead();
			Sleep(15);
		}
	}
	static void checkDead()
	{
		int i = 0;
		for (bullet& p : allbullet)
		{
			if (p.speed < 4.0)
			{
				allbullet.erase(allbullet.begin() + i);
				i--;
			}
			i++;
		}
	}
	static void display()
	{
		setfillcolor(0x242424);
		for (const auto& bullet : allbullet)
		{
			fillcircle((int)(bullet.mx), (int)(bullet.my), 3);
		}
	}
};

class Tank : public Entity//坦克类
{
	friend bullet;
protected:
	int pierce;//穿甲弹数量
	int explosive;//高爆弹数量
	int clip;//弹夹中的子弹数量
	int movepng = 1;//显示哪张图片
	IMAGE img1, img2, img3;

	void RotateImage(IMAGE* pTo, IMAGE* pFrom, double rad)
	{
		IMAGE* pWorking = GetWorkingImage();
		SetWorkingImage(pFrom);
		int iWidth = getwidth();
		int iHeight = getheight();												// 获取原图长宽

		while (rad > 2 * PI)													// 化简弧度
			rad -= 2 * PI;

		double pad = rad;														// 处理弧度
		if (pad > PI / 2 && pad <= PI)
		{
			pad -= PI / 2;
			pad = PI / 2 - pad;
		}
		else if (pad > PI && pad <= PI / 2 * 3)
		{
			pad -= PI;
		}
		else if (pad > PI / 2 * 3 && pad <= PI * 2)
		{
			pad -= PI / 2 * 3;
			pad = PI / 2 - pad;
		}

		int	tWidth = int(iWidth * cos(pad) + iHeight * sin(pad));
		int	tHeight = int(iHeight * cos(pad) + iWidth * sin(pad));				// 计算新图大小

		int iMinX = -(iWidth / 2), iMinY = -(iHeight / 2);
		int iMaxX = iMinX + iWidth, iMaxY = iMinY + iHeight;					// 计算原图最小（大）坐标

		int tMinX = -(tWidth / 2), tMinY = -(tHeight / 2);
		int tMaxX = tMinX + tWidth, tMaxY = tMinY + tHeight;					// 计算新图最小（大）坐标

		setorigin(-iMinX, -iMinY);												// 设置图片中心为原点

		SetWorkingImage(NULL);
		pTo->Resize(tWidth, tHeight);											// 初始化新图

		DWORD* dst = GetImageBuffer(pTo);
		DWORD* src = GetImageBuffer(pFrom);										// 获取新图、原图的缓冲区

		SetWorkingImage(pTo);
		for (int y1 = 0; y1 < tHeight; y1++)
		{
			for (int x1 = 0; x1 < tWidth; x1++)
				dst[x1] = 0x00000000;
			dst += tWidth;
		}
		SetWorkingImage(pWorking);
		for (int y1 = 0; y1 < tHeight; y1++)									// 初始化新图
			dst -= tWidth;

		for (int y1 = tMinY; y1 < tMaxY; y1++)
		{
			for (int x1 = tMinX; x1 < tMaxX; x1++)
			{
				int x = int(x1 * cos(rad) - y1 * sin(rad));
				int y = int(x1 * sin(rad) + y1 * cos(rad));						// 计算变换后坐标

				int sxy = (iHeight - (y - iMinY) - 1) * iWidth + (x - iMinX);
				int dxy = (tHeight - (y1 - tMinY) - 1) * tWidth + (x1 - tMinX);	// 计算坐标在缓冲区的位置

				if (x >= iMinX && x < iMaxX && y >= iMinY && y < iMaxY)			// 越界特判
					dst[dxy] = src[sxy];
			}
		}

		SetWorkingImage(pFrom);
		setorigin(0, 0);
		SetWorkingImage(pWorking);												// 还原原图坐标
	}
public:
	//初始x坐标，初始y坐标，宽度，高度，速度
	Tank(int x, int y, int s) :Entity(x, y, 97, 80, s, MAXHEALTH), pierce(0), explosive(0), clip(3)
	{
		loadimage(&img1, "sorce/tank1.png", 97, 80);
		loadimage(&img2, "sorce/tank2.png", 97, 80);
		loadimage(&img3, "sorce/tank3.png", 97, 80);
	}
	/*********************************************
	杨武显的设计思路：
	Move函数可以接受参数，不同的参数完成不同的动作
	**********************************************/
	void Move(int input) override
	{
		//检测键盘，完成动作
		switch (input)
		{
		case 1:
			vec.roundchange(1);
			break;
		case 2:
			vec.roundchange(2);
			break;
		case 3:
			mx += vec.x * speed;
			my += vec.y * speed;
			break;
		case 4:
			mx -= vec.x * speed;
			my -= vec.y * speed;
		}
	}
	
	void Dead() override
	{
		if (!IsAlive)
		{
			//不再成为实体，修改图像或者删除图像
		}
	}
	void shoot(int kind)//发射
	{
		lock2.lock();
		allbullet.push_back(bullet((mx + 48.5 + 37.5 * cos(vec.angle)), (my + 40 - 37.5 * sin(vec.angle)), kind, vec));//构造子弹对象
		lock2.unlock();
	}
	void display()
	{
		IMAGE temp;
		switch (movepng)
		{
		case 1:
			RotateImage(&temp, &img1, PI / 180.0 * (double)vec.angle);
			break;
		case 2:
			RotateImage(&temp, &img2, PI / 180.0 * (double)vec.angle);
			break;
		case 3:
			RotateImage(&temp, &img3, PI / 180.0 * (double)vec.angle);
			break;
		}
		double temp1, temp2;
		double co, si;
		co = cos(PI / 180.0 * (double)vec.angle);
		si = sin(PI / 180.0 * (double)vec.angle);
		if (co < 0.0)
			co = -co;
		if (si < 0.0)
			si = -si;
		temp1 = (97.0 * co + 80.0 * si) / 2.0;
		temp2 = (97.0 * si + 80.0 * co) / 2.0;
		if (temp1 < 0.0)
			temp1 = -temp1;
		if (temp2 < 0.0)
			temp2 = -temp2;
		temp1 -= 97.0 / 2;
		temp2 -= 80.0 / 2;

		Function::transparentimage(NULL, (int)(mx - temp1), (int)(my - temp2), &temp);
	}
	//检测游戏是否还在进行
	void changepng(bool game)
	{
		while (game)
		{
			while (_kbhit())
			{
				int key = _getch();
				switch (key)
				{
				case 'a':
				case 'A':
				case 'D':
				case 'd':
				case 'w':
				case 'W':
				case 's':
				case 'S':
					movepng++;
				}
				if (movepng == 4)
					movepng = 1;
			}
		}
	}
};


typedef struct point
{
	double x;
	double y;
} Point;





class Player :public Tank
{
private:
	int bulkind = 1;
public:
	std::array<Point, 5> fpt;
	Player() :Tank(50, 240, 5), fpt()
	{
		std::cout << "A player has joined in the game." << std::endl;

	}
	~Player()
	{
		std::cout << "A player has left the game." << std::endl;
	}
	void footprint(bool isgaming)
	{
		while (isgaming)
		{
			for (std::size_t i = 4; i > 0; --i) {
				fpt[i] = fpt[i - 1];
			}
			fpt[0].x = mx;
			fpt[0].y = my;
			Sleep(250);
		}
	}

	void control(bool& game)
	{
		ExMessage msg;
		while (game)
		{	
			if (peekmessage(&msg, EX_KEY, true))//处理键盘信息
			{
				if (msg.message == WM_KEYDOWN && (msg.vkcode == 'a' || msg.vkcode == 'A'))
					Move(1);
				if (msg.message == WM_KEYDOWN && (msg.vkcode == 'd' || msg.vkcode == 'D'))
					Move(2);
				if (msg.message == WM_KEYDOWN && (msg.vkcode == 'w' || msg.vkcode == 'W'))
					Move(3);
				if (msg.message == WM_KEYDOWN && (msg.vkcode == 's' || msg.vkcode == 'S'))
					Move(4);
			}
		}
	}



};


//定义敌人类，写一个敌人AI
class Enemy : public Tank
{
private:
	int view = 50;
	// 计算两点之间的距离  
	inline double distance(const Point& a)
	{
		return std::sqrt(std::pow(a.x - mx, 2) + std::pow(a.y - my, 2));
	}

	// 检查一个点是否在给定半径的圆内  
	inline bool isInsideCircle(double radius, const Point& point)
	{
		return distance(point) <= radius;
	}
	inline bool check(Player& p)
	{
		for (Point& ptr : p.fpt)
		{

		}
	}
public:
	Enemy() :Tank(400, 240, 5)
	{
		std::cout << "An AI has joined the game." << std::endl;
	}
	void aicontrol()
	{
		//寻路
		//攻击
	}
};