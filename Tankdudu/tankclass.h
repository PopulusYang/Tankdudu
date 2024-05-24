#pragma once
//最大生命值
#define MAXHEALTH 100
//障碍物的超级血量
#define SUPER_OBSTACLE 9999
//最长游戏时间
#define MAXTIME 60
#define PI 3.14159265359

#define ROWS 640
#define COLS 480

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
//星星
#define MAXSTAR 200
#include"tankhead.h"
#include <random>
#include<array>
#include<mutex>
#include<cstdlib>
#include <time.h>  
extern int volume_jug;
extern int IDnum;
extern std::mutex lock;
extern std::mutex lock2;
extern bool isgaming;
class bullet;
extern std::vector<bullet> allbullet;
extern unsigned char map[ROWS][COLS];


inline bool KeyDown(int vKey)
{
	return ((GetAsyncKeyState(vKey) & 0x8000) ? 1 : 0);
}


class Function
{
public:
	static void RotateImage(IMAGE* pTo, IMAGE* pFrom, double rad)
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
			angle += 2;
			y = std::sin(PI / 180 * angle);
			x = std::cos(PI / 180 * angle);
			break;
		case 1:
			angle -= 2;
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
//碰撞箱
class ColliderBox
{
public:
	//坐标以及长宽

	ColliderBox() :mx(0), my(0), height(0), width(0), p(NULL), ID(IDnum), mhealth(MAXHEALTH), speed(0), IsAlive(true), tag(0) {}
	ColliderBox(int x, int y, int w, int h, int s, int health, int tag, Vec vec) :mx(x), my(y), height(h), width(w), ID(IDnum), mhealth(health), speed(s), IsAlive(true), vec(vec), tag(tag)
	{
		allbox.push_back(*this);
		p = &allbox[allbox.size() - 1];
		IDnum++;
	}
	ColliderBox(int x, int y, int w, int h, int s, int tag, int health) :mx(x), my(y), height(h), width(w), ID(IDnum), mhealth(health), speed(s), IsAlive(true), tag(tag)
	{
		allbox.push_back(*this);
		p = &allbox[allbox.size() - 1];
		IDnum++;
	}
	virtual ~ColliderBox() {}
	/*
	ColliderBox fuck(double ix, double iy) {
		return ColliderBox((int)ix, (int)iy, width,height);
	}
	*/
	friend int ColliderDectect(const ColliderBox& box1, const ColliderBox& box2);
	static inline void drawColliderbox(ColliderBox& obj);
	inline ColliderBox* getp() { return p; };
	inline int getID() { return ID; };
	//override the data to public
	double mx;
	double my;
	int width;
	int height;
	int ID;
	ColliderBox* p;
	int mhealth;//健康值/血量
	//位置
	double speed;
	bool IsAlive;
	Vec vec;
public:
	virtual void Dead()//死亡
	{}
	virtual void Move(int)//移动
	{}
	unsigned char tag;
};

//调试模式，绘制碰撞箱
void ColliderBox::drawColliderbox(ColliderBox& obj)
{
	setfillcolor(WHITE);
	//+21 +20 -25 -15
	rectangle((int)(obj.mx), (int)(obj.my), (int)(obj.mx + obj.width), (int)(obj.my + obj.height));
}

/*class Entity : public ColliderBox//继承了碰撞箱的属性
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
	Entity(int x, int y, int w, int h, int s, int health, Vec vec) :mhealth(health), ColliderBox(x, y, h, w), speed(s), IsAlive(true), vec(vec) //mybox没用
	{

	};
	Entity(int x, int y, int w, int h, int s, int health) :mhealth(health), ColliderBox(x, y, h, w), speed(s), IsAlive(true) {};

	virtual ~Entity() {}
	virtual void Dead() = 0;//死亡
	virtual void Move(int) = 0;//移动
};*/

class obstacle : public ColliderBox//障碍物
{
private:
	int kind;
	//下面这行为输入图片
	//三种不同的障碍物（3为不可击穿的）
	IMAGE img1, img2, img3;
public:
	//原点指图片左上角
	//原点的x坐标，原点的y坐标，宽度，高度,速度，血量,障碍物类型
	obstacle(int x, int y, int w, int h, int s, int blood, int kind) :ColliderBox(x, y, w, h, s, 1, blood), kind(kind)
	{
		//此处的载入图片需要改成三种障碍物
		loadimage(&img1, "sorce/wall1.png", w, h);
		loadimage(&img2, "sorce/wall2.png", w, h);
		loadimage(&img3, "sorce/wall3.png", w, h);
		if (kind == 2)
		{
			tag = 3;
			allbox.back().tag = 3;
		}
			
		//此处被注释掉的代码应该在调用函数时确定kind类型时候使用
		/*std::random_device rd;  // 获取随机数种子
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(1, 2);
		kind = distrib(gen);*/
	}
	obstacle() :ColliderBox(0, 0, 0, 0, 0, 1, 0), kind(0) {}
	void Dead() override
	{
		if (IsAlive)
		{
			//不再成为实体，修改图像或者删除图像
		}
		if (!IsAlive)
		{
			//int jug = 1;
			this->mx = -100;
			this->my = -100;
			this->height = 0;
			this->width = 0;
			this->mhealth = MAXHEALTH;
			this->IsAlive = true;
			if(volume_jug)
			    PlaySound("music/bang.wav", NULL, SND_FILENAME | SND_ASYNC);
			for (int i = 0; i < allbox.size(); i++)
			{
				if(this->ID == allbox[i].ID)
				{
					allbox[i].mx = this->mx;
					allbox[i].my = this->my;
					allbox[i].height = this->height;
					allbox[i].width = this->width;
					allbox[i].mhealth = this->mhealth;
				}
			}
		}
	}
	void Move(int) override {}
	void deblood()
	{
		mhealth = allbox[this->ID].mhealth;
		if (mhealth < 0)
		{
			IsAlive = false;
			//PlaySound("music/blase.wav", NULL, SND_FILENAME|SND_ASYNC);
			//mciSendString("play music/bang.wav", 0, 0, 0);
		}
	}
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
		Function::transparentimage(NULL, mx, my, &temp);
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
	bullet(double x, double y, int kind, Vec vec) :mx(x), my(y), vec(vec), kind(kind), speed(10.0) {}
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
	//逻辑上更改一下：碰上返回allbox数，没碰上返回0。
	static int bull_OBSdec(bullet& thisbull)//子弹专属障碍物碰撞检测,Collider==ture
	{
		int jug = 0;
		for (int i = 2; i < 9; i++)//OBS number define MAX==4
		{
			if (thisbull.getx() >= allbox[i].mx && thisbull.getx() < allbox[i].mx + allbox[i].width && thisbull.gety() >= allbox[i].my && thisbull.gety() < allbox[i].my + allbox[i].height)
			{
				jug = allbox[i].ID;
				break;
			}
		}
		return jug;
	}
	//子弹检测可以改的和上面那个一样，或者直接合并
	static bool bull_PLAdec(bullet& thisbull) //子弹专属人物碰撞检测,写这个主要是保险，后续可以合并简化，Collider==ture
	{
		int flag = 1;
#define MAXsize 1 //MAXsize is the number of other player
		for (int i = 4; i < 4; i++)//Player number define MAXsize==1
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
				int t = bull_OBSdec(p);
				for (int i = 0; i < allbox.size(); i++)
				{
					if (allbox[i].ID == t)
					{
						allbox[i].mhealth = allbox[i].mhealth - 30;
					}
				}
				allbullet.erase(allbullet.begin() + i);
				i--;
			}
			else if (bull_PLAdec(p)) {

				/*加入人物掉血操作函数*/
				int t = bull_OBSdec(p);
				for (int i = 0; i < allbox.size(); i++)
				{
					if (allbox[i].ID == t)
					{
						allbox[i].mhealth = allbox[i].mhealth - 30;
					}
				}
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


bool isPointNear(int x1, int y1, int x2, int y2, int range);
bool angleDectect(const ColliderBox& box1, const ColliderBox& box2, int range = 5);
class Tank : public ColliderBox//坦克类
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


public:
	//初始x坐标，初始y坐标，宽度，高度，速度
	Tank(int x, int y, int s) :ColliderBox(x + 12, y + 30, 97 - 12, 80 - 22, s, 2, MAXHEALTH), pierce(0), explosive(0), clip(3)
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
		case LEFT:
			vec.roundchange(1);
			break;
		case RIGHT:
			vec.roundchange(2);
			break;
		case UP:
		{
		
			int jug = 1;
			mx += vec.x * speed;
			my += vec.y * speed;
			if (tag == 2)
			{
				allbox[0].mx = mx;
				allbox[0].my = my;
			}
			else
			{
				allbox[1].mx = mx;
				allbox[1].my = my;
			}


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
				for (int stay=1; stay < allbox.size(); stay++) {
					switch (ColliderDectect(*this, allbox[stay])) {
					case 1://左右
						if (!angleDectect(*this, allbox[stay],10)) {
							mx -= vec.x * speed ;//
							my += vec.y * speed * 0.1;
							
						}
						else {
							mx -= vec.x * speed*1.5;//
							my -= vec.y * speed*1.1;
						}
						if (tag == 2)
						{
							allbox[0].mx = mx;
							allbox[0].my = my;
						}
						else
						{
							allbox[1].mx = mx;
							allbox[1].my = my;
						}
						break;
					
					case 2://上下			
						if (!angleDectect(*this, allbox[stay],10)) {
							mx += vec.x * speed * 0.1;
							my -= vec.y * speed;//
							
						}
						else {
							mx -= vec.x * speed*1.1;
							my -= vec.y * speed*1.5;//
						}
						if (tag == 2)
						{
							allbox[0].mx = mx;
							allbox[0].my = my;
						}
						else
						{
							allbox[1].mx = mx;
							allbox[1].my = my;
						}
						break;

					}
				}
			}
				jug = 1;
				break;
			
		}
		case DOWN:
		{
			int jug = 1;
			
			mx -= vec.x * speed*1.5;
			my -= vec.y * speed*1.5;
			if (tag == 2)
			{
				allbox[0].mx = mx;
				allbox[0].my = my;
			}
			else
			{
				allbox[1].mx = mx;
				allbox[1].my = my;
			}

			for (int i = 1; i < allbox.size(); i++)
			{
				if (this->ID != allbox[i].ID)
				{
					if (ColliderDectect(*this, allbox[i]))
						jug = 0;
						
				}
			}

			if (!jug) {
				for (int stay = 1; stay < allbox.size(); stay++) {
					switch (ColliderDectect(*this, allbox[stay])) {
					case 1:

						if (!angleDectect(*this, allbox[stay],10)) {
							mx += vec.x * speed;//
							my -= vec.y * speed * 0.1;
							
						}
						else {
							mx += vec.x * speed*1.5;//
							my += vec.y * speed*1.1 ;
						}

						if (tag == 2)
						{
							allbox[0].mx = mx;
							allbox[0].my = my;
						}
						else
						{
							allbox[1].mx = mx;
							allbox[1].my = my;
						}
						break;
					case 2:
						if (!angleDectect(*this, allbox[stay],10)) {
							mx -= vec.x * speed * 0.1;
							my += vec.y * speed;//
					
						}
						else {
							mx += vec.x * speed*1.1;
							my += vec.y * speed*1.5;//
						}
						
						if (tag == 2)
						{
							allbox[0].mx = mx;
							allbox[0].my = my;
						}
						else
						{
							allbox[1].mx = mx;
							allbox[1].my = my;
						}
						break;
					}
				}
				jug = 1;
				break;
			}
		}
		Sleep(16);
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
		if (canshoot)
		{
			canshoot = false;
			lock2.lock();
			allbullet.push_back(bullet((mx + 48.5 + 37.5 * cos((double)vec.angle / 180.0 * PI)), (my + 40 + 37.5 * sin((double)vec.angle / 180.0 * PI)), kind, vec));//构造子弹对象			
			if(volume_jug)
				mciSendString("play music/fire.wav", 0, 0, 0);
			lock2.unlock();
		}
	}
	void wait(bool isgaming)
	{
		while (isgaming)
		{
			if (!canshoot)
			{
				for (waittime = 125; waittime > 0; waittime--)
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
			Function::RotateImage(&temp, &img1, PI / 180.0 * (double)vec.angle);
			break;
		case 2:
			Function::RotateImage(&temp, &img2, PI / 180.0 * (double)vec.angle);
			break;
		case 3:
			Function::RotateImage(&temp, &img3, PI / 180.0 * (double)vec.angle);
			break;
		case 4:
			Function::RotateImage(&temp, &img1, PI / 180.0 * (double)vec.angle);
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
			fillrectangle((int)mx + 8, (int)(my - 10), (int)(mx + 8 + (int)(74.0 * (double)mhealth / (double)MAXHEALTH)), (int)(my - 5));
		}
		if (waittime != 0)
		{
			setfillcolor(GREEN);
			fillrectangle((int)(mx + 8), (int)(my - 15), (int)(mx + 8 + (int)(74.0 * (double)waittime / 125.0)), (int)(my - 11));
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
		:Tank(0, 400, 1.3), fpt(), up(up), down(down), left(left), right(right), shift(shift), vshoot(vshoot)
	{
		std::cout << "A player has joined in the game." << std::endl;

	}
	~Player()
	{
		std::cout << "A player has left the game." << std::endl;
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
#define VIEW 50;

//定义敌人类，写一个敌人AI
class Enemy : public Tank
{
private:

	// 计算两点之间的距离  
	inline double distance(double x, double y)
	{
		return sqrt((x - mx) * (x - mx) + (y - my) * (y - my));
	}
	enum types { m_road, m_wall };
	enum direct { p_up, p_down, p_left, p_right, p_lup, p_ldown, p_rup, p_rdown };

	class MyPoint//点坐标
	{
	public:
		int row = 0;
		int col = 0;
		double f = 0;
		double g = 0;
		double h = 0;
		bool operator== (MyPoint p)
		{
			if (row == p.row && col == p.col)
				return true;
			else
				return false;
		}
	};

	struct treeNode
	{
		MyPoint pos;
		std::vector<treeNode*> child;//八叉树的子数组
		treeNode* father = NULL;//指向父节点
	};

	treeNode* createNode(MyPoint pos)
	{
		treeNode* pNew = new treeNode{};
		pNew->pos.row = pos.row;
		pNew->pos.col = pos.col;
		return pNew;
	}

	void displaypoint(treeNode* p)//调试用，显示轨迹
	{
		setfillcolor(BLACK);
		while (p)
		{
			fillcircle(p->pos.row, p->pos.col, 1);
			p = p->father;
		}
	}

	treeNode* Astar(int x1, int y1, int x2, int y2)
	{
		//数字描述
		static bool isFind[ROWS][COLS] = { 0 };
		memset(isFind, 0, sizeof(isFind));
		scan();
		//确定起点与终点
		MyPoint begpos;
		begpos.row = x1;
		begpos.col = y1;
		MyPoint endpos;
		endpos.row = x2;
		endpos.col = y2;


		treeNode* pRoot = NULL;
		//起点作为第一个节点
		pRoot = createNode(begpos);
		isFind[begpos.row][begpos.col] = 1;
		treeNode* pCurrent = pRoot;
		treeNode* pChild;

		std::vector<treeNode*> buff;
		std::vector<treeNode*>::iterator it;
		std::vector<treeNode*>::iterator itmin;//找最小用的迭代器
		bool isFindend = false;
		int time = 0;
		while (time < 700)
		{
			for (int i = 0; i < 8; i++)
			{
				pChild = createNode(pCurrent->pos);
				switch (i)
				{
				case p_up:
					pChild->pos.row--;
					pChild->pos.g += 10.0;
					break;
				case p_down:
					pChild->pos.row++;
					pChild->pos.g += 10.0;
					break;
				case p_left:
					pChild->pos.col--;
					pChild->pos.g += 10.0;
					break;
				case p_right:
					pChild->pos.col++;
					pChild->pos.g += 10.0;
					break;
				case p_lup:
					pChild->pos.row--;
					pChild->pos.col--;
					pChild->pos.g += 14.0;
					break;
				case p_ldown:
					pChild->pos.row++;
					pChild->pos.col--;
					pChild->pos.g += 14.0;
					break;
				case p_rup:
					pChild->pos.row--;
					pChild->pos.col++;
					pChild->pos.g += 14.0;
					break;
				case p_rdown:
					pChild->pos.row++;
					pChild->pos.col++;
					pChild->pos.g += 14.0;
				}
				if (canWalk(pChild->pos, map, isFind))
				{
					//计算h
					pChild->pos.h = getH(pChild->pos, endpos);
					//计算f
					pChild->pos.f = pChild->pos.g + pChild->pos.h;
					//存入数组
					buff.push_back(pChild);
					//放入八叉树
					pCurrent->child.push_back(pChild);
					pChild->father = pCurrent;
				}
				else
				{
					delete pChild;
				}
			}
			//找不到终点
			if (buff.empty())
				break;
			//找最小
			itmin = buff.begin();
			for (it = buff.begin(); it != buff.end(); it++)
			{
				if ((*itmin)->pos.f > (*it)->pos.f)
					itmin = it;
			}
			//走
			pCurrent = *itmin;
			time++;
			//displaypoint(pCurrent);
			//标记
			isFind[pCurrent->pos.row][pCurrent->pos.col] = true;
			//清空buff
			buff.clear();
			//判断是否为终点
			if (pCurrent->pos.row>=endpos.row-3&& pCurrent->pos.row <= endpos.row + 3&& pCurrent->pos.col >= endpos.col - 3 && pCurrent->pos.col <= endpos.col + 3)
			{
				isFindend = true;
				break;
			}
			
		}
		if (isFindend)
		{
			std::cout << "找到终点" << std::endl;
			std::cout << "path:";
			treeNode* retree;
			retree = pCurrent;
			return retree;
		}
		std::cout << "寻路失败" << std::endl;
		return NULL;
	}



	void scan()
	{
		memset(map, 0, sizeof(map));
		for (ColliderBox& p : allbox)
		{
			for (int i = (int)(p.my); i <= (int)(p.my + p.height); i++)
			{
				for (int j = (int)(p.mx); j <= (int)(p.mx) + p.width; j++)
				{
					if (i < 0 || j < 0)
						continue;
					map[j][i] = p.tag;
				}
			}
		}
		map[(int)mx][(int)my] = 0;
	}

	bool canWalk(MyPoint pos, unsigned char map[ROWS][COLS], bool isFind[ROWS][COLS])
	{
		if (pos.row < 0 || pos.col < 0 || pos.row >= ROWS || pos.col >= COLS)
			return false;//越界返回false
		if (isFind[pos.row][pos.col])
			return false;//走过的返回false
		if (map[pos.row][pos.col] == 1)
			return false;//障碍物不能走
		for (int i = 12; i < 86; i++)
		{
			if (map[pos.row + i][pos.col] == 1 || map[pos.row + i][pos.col] == 3)
				return false;
		}
		for (int i = 30; i < 79; i++)
		{
			if (map[pos.row][pos.col+i] == 1 || map[pos.row][pos.col+i] == 3)
				return false;
		}
		for (int i = 30; i > -10; i--)
		{
			if (map[pos.row][pos.col + i] == 1 || map[pos.row][pos.col + i] == 3)
				return false;
		}
		for (int i = 12; i > -10; i--)
		{
			if (map[pos.row + i][pos.col] == 1 || map[pos.row + i][pos.col] == 3)
				return false;
		}
		return true;
	}

	double getH(MyPoint pos, MyPoint end)
	{
		int x, y;
		if (end.row > pos.row)
		{
			y = end.row - pos.row;
		}
		else
		{
			y = pos.row - end.row;
		}
		if (end.col > pos.col)
		{
			x = end.col - pos.col;
		}
		else
		{
			x = pos.col - end.col;
		}
		return sqrt(x * x + y * y);
	}
	//释放八叉树的内存
	void freetree(treeNode* root)
	{
		treeNode* temp1;
		temp1 = root;
		treeNode* temp2 = NULL;
		int i = 0;
		while (!temp1->child.empty())
		{
			temp2 = temp1;
			bool jug = true;
			i = 0;
			while(1)//遇到有子项的就赋值，没有子项的直接释放
			{
				if (temp2->child[i]->child.empty())
				{
					delete temp2->child[i];
					temp2->child.erase(temp2->child.begin() + i);
				}
				else
				{
					temp1 = temp2->child[i];
					i++;
				}
				if (temp1 == temp2 && temp2->child.empty())//检测是否是最后一个节点
				{
					jug = false;
					break;
				}
				if (temp1 != temp2 && temp2->child.size() == 1)
				{
					jug = true;
					break;
				}
			}
			if (jug)
				delete temp1->father;
		}
		delete temp1;
	}


public:
	
	Enemy() :Tank(400, -30, 5)
	{
		std::cout << "An AI has joined the game." << std::endl;
		tag = 5;
		allbox.back().tag = 5;
	}
	void aicontrol(bool isgaimg)
	{
		//寻路
		//攻击
		typedef struct Quadrant//象限
		{
			int qua = 0;//象限数
			inline void calqua(int angle)
			{
				qua = angle / 90 + 1;
			}
			int operator- (const Quadrant& p)
			{
				if (qua < p.qua)
					return qua + 4 - p.qua;
				else
					return qua - p.qua;
			}
		}Quadrant;
		while (isgaimg)
		{
			treeNode* road;
			road = Astar((int)allbox[0].mx, (int)allbox[0].my, (int)mx, (int)my);
			if (road == NULL)
				continue;
			Point p1{ mx,my };
			for (int i = 0; i < 5; i++)
			{
				road = road->father;
				if (road == NULL || road->father == NULL)
					break;
			}
			Point p2{ (double)road->pos.row,(double)road->pos.col };
			//释放八叉树内存
			while (road->father != NULL)
				road = road->father;
			freetree(road);
			//找出路径中的两个点，求出其向量
			Vec temp;
			temp.x = (double)(p2.x - p1.x) / sqrt((double)(p2.x - p1.x) * (double)(p2.x - p1.x) + (double)(p2.y - p1.y) * (double)(p2.y - p1.y));
			temp.y = (double)(p2.y - p1.y) / sqrt((double)(p2.x - p1.x) * (double)(p2.x - p1.x) + (double)(p2.y - p1.y) * (double)(p2.y - p1.y));
			double tempcos = temp.x / sqrt(temp.x * temp.x + temp.y * temp.y);
			double tempsin = temp.y / sqrt(temp.x * temp.x + temp.y * temp.y);
			temp.angle = (int)(acos((long double)tempcos) / PI * 180.0);
			if (tempsin < 0)
				temp.angle = 360 - temp.angle;
			//将坦克转向方向向量
			Quadrant q1, q2;
			q1.calqua(temp.angle);
			q2.calqua(vec.angle);
			for (int i = 0; i < 20 && (temp.angle <= vec.angle - 3 || temp.angle >= vec.angle + 3); i++)
			{
				if (q1 - q2 > q2 - q1||temp.angle<vec.angle)
					Move(1);
				else
					Move(2);
				Sleep(25);
			}
			//移动
			if (temp.angle >= vec.angle - 3 && temp.angle <= vec.angle + 3)
			{
				Move(3);
				Sleep(25);
				Move(3);
			}
			double dis = distance(allbox[0].mx, allbox[0].my);
			while (dis<20.0)
			{
				Move(4);
				Sleep(23);
				dis = distance(allbox[0].mx, allbox[0].my);
			}
			bool canshoot = false;
			Point checkshoot{ mx + 48.5 + 37.5 * cos((double)vec.angle / 180.0 * PI) ,my + 40 + 37.5 * sin((double)vec.angle / 180.0 * PI) };
			for (int i = 0; i < 50; i++)
			{
				if (map[(int)checkshoot.x][(int)checkshoot.y] == 2 || map[(int)checkshoot.x][(int)checkshoot.y] == 3)
					canshoot = true;
			}
			if (canshoot)
				shoot(1);
		}
	}
};

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


