#include"MyClass.h"
const int Win_width = 1200;
const int Win_height = 600;
const char images[5][20] = { "shoot0.png","shoot1.png","shoot2.png","shoot3.png","shoot4.png" };
const char flowers[5][20] = { "flower0.png","flower1.png" ,"flower2.png" ,"flower3.png" ,"flower4.png" };
vector<Bomb> fire_works;
vector<Bomb> clicks;
DWORD* pMem;
bool MouseClick;
void Init();
void draw();
void clearImage();
void updata();
void userClick();
int main() {
	Init();
	BeginBatchDraw();
	while (1) {
		Sleep(16);
		clearImage();
		draw();
		userClick();
		updata();
		draw();
		FlushBatchDraw();
	}
	getchar();
	closegraph();
	return 0;
}
void Init() {
	srand(time(NULL));
	initgraph(Win_width, Win_height);
	pMem = GetImageBuffer();
	for (int i = 0; i < 5; i++) {
		int x = rand() % (Win_width - 40) + i * 10;
		int hy = Win_height / 2 - rand() % 200;
		Bomb tmp(x, hy, i);
		fire_works.push_back(tmp);
	}
}
void updata() {
	for (auto p = fire_works.begin(); p != fire_works.end(); p++)
		p->updata();
	int i = 0;
	while (clicks.empty() == false && i < clicks.size()) {
		if (clicks[i].iserase())
			clicks.erase(clicks.begin() + i);
		else {
			clicks[i].updata();
			i++;
		}
	}
}
void draw() {
	for (auto p = fire_works.begin(); p != fire_works.end(); p++) {
		p->draw();
	}
	for (auto p = clicks.begin(); p != clicks.end(); p++)
		p->draw();
}
void clearImage() {
	for (int i = 0; i < 2000; i++)
	{
		int px1 = rand() % Win_width; // 0  to 1199
		int py1 = rand() % Win_height;  // 0 to 799

		pMem[py1 * Win_width + px1] = BLACK;//将像素点赋值为背景色即为擦除
		pMem[py1 * Win_width + px1 + 1] = BLACK;	// 对显存赋值擦出像素点		
	}
}
void userClick() {
	ExMessage msg;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN)
			MouseClick = true;
		if (MouseClick) {
			int index = rand() % 5;
			Bomb tmp(msg.x, Win_height - 44, index);
			clicks.push_back(tmp);
		}
	}
}