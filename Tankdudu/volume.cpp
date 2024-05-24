#include"tankclass.h"
#include"tankhead.h"
//音量调节进入这个函数

void volume()
{
	cleardevice();
	button* b1 = new button(260, 230, 120, 50, "单人游戏");
	button* b2 = new button(260, 310, 120, 50, "双人游戏");
	button* b4 = new button(260, 390, 120, 50, "退出游戏");
	button* b3 = new button(460, 390, 60, 50, "音量");

	

}