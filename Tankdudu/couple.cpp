#include"tankclass.h"
#include"tankhead.h"
extern std::vector<ColliderBox> allbox;
//˫����Ϸ�����������������main����������C���Կ�����Ϊ���������Ҫ���ˣ�
void couplelegame()
{
	cleardevice();
	RECT center = { 0,0,639,479 };
	settextstyle(36, 0, "��������");
	settextcolor(WHITE);
	drawtext("�����У����������ʼ���ԡ�", &center, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	_getch();

}