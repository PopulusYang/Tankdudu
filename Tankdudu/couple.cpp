#include"tankclass.h"
#include"tankhead.h"
extern std::vector<ColliderBox> allbox;
//双人游戏进入这个函数，避免main函数过长（C语言课设因为这个问题我要死了）
void couplelegame()
{
	cleardevice();
	RECT center = { 0,0,639,479 };
	settextstyle(36, 0, "华文隶书");
	settextcolor(WHITE);
	drawtext("开发中，按任意键开始测试。", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();

}