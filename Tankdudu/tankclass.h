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
#include<cstdlib>
extern int IDnum;
extern std::mutex lock;
extern std::mutex lock2;
extern bool isgaming;
class bullet;
extern std::vector<bullet> allbullet;

inline bool KeyDown(int vKey)
{
	return ((GetAsyncKeyState(vKey) & 0x8000) ? 1 : 0);
}

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
	Vec() :x(1.0), y(0.0), angle(0) {}
	void roundchange(int userKey)
	{
		switch (userKey)
		{
		case 2://沿着向量方向增加
			angle += 4;
			y = std::sin(PI / 180 * angle);
			x = std::cos(PI / 180 * angle);
			break;
		case 1:
			angle -= 4;
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

	ColliderBox() :mx(0), my(0), height(0), width(0), p(NULL), ID( IDnum) {}
	ColliderBox(int x, int y, int h, int w) :mx(x+12), my(y+30), height(h-12), width(w-22),ID(IDnum)
	{	
		allbox.push_back(*this);
		p = &allbox[allbox.size()-1];
		IDnum++;
	}
	/*
	ColliderBox fuck(double ix, double iy) {
	 	return ColliderBox((int)ix, (int)iy, width,height);
	}
	*/
	friend int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2);
	static inline void drawColliderbox(ColliderBox& obj);
	virtual ~ColliderBox() {};

	inline ColliderBox* getp() { return p; };
	inline int getID() { return ID; };

	//override the data to public
	double mx;
	double my;
	int width;
	int height;
	int ID;
	ColliderBox* p;
};

//调试模式，绘制碰撞箱
void ColliderBox::drawColliderbox(ColliderBox &obj)
{
	setfillcolor(WHITE);
	//+21 +20 -25 -15
	rectangle((int)(obj.mx), (int)(obj.my), (int)(obj.mx + obj.width), (int)(obj.my + obj.height));
}

class Entity : public ColliderBox//继承了碰撞箱的属性
{
protected:
	int mhealth;//健康值/血量
	//位置
	double speed;
	bool IsAlive;
	ColliderBox mybox;
	Vec vec;
public:
	//默认构造函数
	Entity() :mhealth(MAXHEALTH), speed(0), IsAlive(true) {};
	//x坐标，y坐标，宽度，高度，速度，生命值
	Entity(int x, int y, int w, int h, int s, int health, Vec vec) :mhealth(health), ColliderBox(x, y, h, w),  speed(s), IsAlive(true), vec(vec) //mybox没用
	{
		
	};
	Entity(int x, int y, int w, int h, int s, int health) :mhealth(health), ColliderBox(x, y, h, w),  speed(s), IsAlive(true) {};
	
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
class bullet//子弹类
{
private:
	int kind;
	double mx;
	double my;
	Vec vec;
	double speed;
public:
	//初始x坐标，初始y坐标，宽度，高度，速度,种类,
	bullet(double x, double y, int kind, Vec vec) :mx(x),my(y),vec(vec), kind(kind) ,speed(10.0){}
	inline double getx() { return mx; };
	inline double gety() { return my; }
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
	//检测逻辑优化,不再对子弹套用碰撞箱检测函数，简化检测逻辑，实现子弹的即时生成和销毁而不占用储存空间
	static bool bull_OBSdec(bullet& thisbull)//子弹专属障碍物碰撞检测,Collider==ture
	{	
		int flag = 1;
		for (int i = 1; i < 4; i++)//OBS number define MAX==4
		{	
			if (thisbull.getx() >= allbox[i].mx&& thisbull.getx() < allbox[i].mx + allbox[i].width && thisbull.gety() >= allbox[i].my && thisbull.gety() < allbox[i].my + allbox[i].height) {
				flag = 0;
				break;
			}
		}
		if (flag) {
			return false;
		}
		return true;
	}
	static bool bull_PLAdec(bullet& thisbull) //子弹专属人物碰撞检测,写这个主要是保险，后续可以合并简化，Collider==ture
	{
		int flag = 1;
            #define MAXsize 1 //MAXsize is the number of other player
			for (int i = 4; i <4 ; i++)//Player number define MAXsize==1
			{	//下面的检测逻辑需要依据玩家具体参数调整
				if (thisbull.getx() >= allbox[i].mx && thisbull.getx() < allbox[i].mx + allbox[i].width && thisbull.gety() >= allbox[i].my && thisbull.gety() < allbox[i].my + allbox[i].height) {
					flag = 0;
					break;
				}
			}
		if (flag) {
			return false;
		}
		return true;
	}
	static void checkDead()
	{
		int i = 0;
		for (bullet& p : allbullet)
		{
			if (p.speed < 4.0)
			{
				allbullet.erase(allbullet.begin() + i);//子弹消除操作
				i--;
			}
		 	else if (bull_OBSdec(p))
			{
				/*加入障碍物掉血操作函数*/

				allbullet.erase(allbullet.begin() + i);
				i--;
			}
			else if (bull_PLAdec(p)) {

				/*加入人物掉血操作函数*/

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
	bool canshoot = true;
	int waittime = 0;
	int pierce;//穿甲弹数量
	int explosive;//高爆弹数量
	int clip = 3;//弹夹中的子弹数量
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
		{
			int jug = 1;
			mx += vec.x * speed;
			my += vec.y * speed;
			allbox[0].mx = mx;
			allbox[0].my = my;

		
			for (int i = 0; i < allbox.size(); i++)//改了这
			{
				if (allbox[i].getID() != this->getID())
				{
					if (ColliderDectect(*this, allbox[i]))
						jug = 0;
				}
			}

			if (!jug)
			{

				mx -= vec.x * speed * 2;
				my -= vec.y * speed * 2;
				allbox[0].mx = mx;
				allbox[0].my = my;
			}
			jug = 1;
			break;
		}
		case 4:
		{
			int jug = 1;
			mx -= vec.x * speed;
			my -= vec.y * speed;
			allbox[0].mx = mx;
			allbox[0].my = my;
			ColliderBox* mp = this->getp();
			for (int i = 1; i < allbox.size(); i++)
			{
				if (&allbox[i] != mp)
				{
					if (ColliderDectect(*this, allbox[i]))
						jug = 0;

				}
				if (jug == 0) {

					mx += vec.x * speed * 2;
					my += vec.y * speed * 2;
					allbox[0].mx = mx;
					allbox[0].my = my;
				}
			}
			jug = 1;
			break;
		}
		}
		Sleep(16);
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
		if (canshoot)
		{
			canshoot = false;
			lock2.lock();
			allbullet.push_back(bullet((mx + 48.5 + 37.5 * cos((double)vec.angle / 180.0 * PI)), (my + 40 + 37.5 * sin((double)vec.angle / 180.0 * PI)), kind, vec));//构造子弹对象
			lock2.unlock();
		}
	}
	void wait(bool isgaming)
	{
		while (isgaming)
		{
			if (!canshoot)
			{
				for (waittime = 250; waittime > 0; waittime--)
					Sleep(1);
				canshoot = true;
			}
		}
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
		case 4:
			RotateImage(&temp, &img1, PI / 180.0 * (double)vec.angle);
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
		
		if (mhealth < 100)
		{
			setfillcolor(RED);
			fillrectangle(mx, my - 10, mx + 8 + (int)(74.0 * (double)mhealth / (double)MAXHEALTH), my - 5);
		}
		if (waittime != 0)
		{
			setfillcolor(GREEN);
			fillrectangle(mx + 8, my - 15, mx + 8 + (int)(74.0 * (double)waittime / 250.0), my - 11);
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
	int up, down, left, right, shift, vshoot;
public:
	std::array<Point, 5> fpt;
	Player(int up, int down, int left, int right, int shift, int vshoot)
	:Tank(50, 240, 3), fpt(), up(up), down(down), left(left), right(right), shift(shift), vshoot(vshoot)
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
		while (game)
		{	
			if (KeyDown(left))
				Move(1);
			if (KeyDown(right))
				Move(2);
			if (KeyDown(up))
				Move(3);
			if (KeyDown(down))
				Move(4);
			if (KeyDown(vshoot))
				shoot(bulkind);
			Sleep(5);
		}
	}

	void changepng(bool game)
	{
		while (game)
		{
			if (KeyDown(left) || KeyDown(right) || KeyDown(up) || KeyDown(down))
				movepng++;
			if (movepng == 4)
				movepng = 1;
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