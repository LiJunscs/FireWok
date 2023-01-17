#include"MyClass.h"
const char images[5][30] = { "resource\\shoot0.png","resource\\shoot1.png","resource\\shoot2.png","resource\\shoot3.png","resource\\shoot4.png" };
const char flowers[5][30] = { "resource\\flower0.png","resource\\flower1.png" ,"resource\\flower2.png" ,"resource\\flower3.png" ,"resource\\flower4.png" };
const int heart_cooradinates[12][2] = { {485,110},{717,110},{372,180},{830,180},{389,280},{813,280},{446,360},{756,360},{522,420},{680,420},{601,200},{601,460} };
const char happy_new_year[30] = "resource\\happy_new_year.png";
const int heart_speed[6] = { -25,-21,-16,-12,-9,-7 };
const int Win_width = 1200;
const int Win_height = 600;
vector<FireWork> fire_works;
vector<FireWork> clicks;
vector<FireWork> heart;
IMAGE new_year_img;
char text[6][80];
int textSize;
bool heart_flag;
DWORD* pMem;
void Init();
void clearImage();
void draw();
void updata();
void reset();
void userClick();
void printText();
void count_down();
void blessing_text();
void setTextStyle();
int main() {
	Init();
	mciSendString("play resource\\bk_music.mp3", 0, 0, 0);
	blessing_text();
	count_down();
	mciSendString("play resource\\firework.mp3 repeat", 0, 0, 0);
	BeginBatchDraw();
	while (1) {
		Sleep(25);
		clearImage();
		putimage(120, Win_height - 100, &new_year_img);
		userClick();
		draw();
		updata();
		printText();
		draw();
		FlushBatchDraw();
		reset();
	}
	getchar();
	closegraph();
	return 0;
}
void Init() {
	srand(time(NULL));
	initgraph(Win_width, Win_height);
	pMem = GetImageBuffer();//获取显存指针
	heart_flag = false;
	mciSendString("open resource\\bk_music.mp3", 0, 0, 0);
	mciSendString("open resource\\firework.mp3", 0, 0, 0);//打开烟花背景音乐并循环播放
	loadimage(&new_year_img, happy_new_year);
	setTextStyle();//设置字体和颜色
	setbkmode(TRANSPARENT);
	settextcolor(YELLOW);
	for (int i = 0; i < 4; i++) {//初始化三个烟花
		int x = rand() % (Win_width - 40) + i * 10;
		int hy = Win_height / 2 - rand() % 200;
		FireWork tmp(x, hy, i);
		fire_works.push_back(tmp);
	}
	for (int i = 0; i < 12; i++) {
		FireWork tmp(heart_cooradinates[i][0], heart_cooradinates[i][1], (i / 2) % 4);
		heart.push_back(tmp);
	}
	ifstream fp;
	fp.open("resource\\user_defined.txt");
	while (textSize < 6 && fp >> text[textSize])
		textSize++;
	fp.close();
}
void updata() {
	for (auto p = fire_works.begin(); p != fire_works.end(); p++)
		p->updata();
	for (auto p = clicks.begin(); p != clicks.end();) {
		if (p->isFinish())
			p = clicks.erase(p);
		else {
			p->updata();
			p++;
		}
	}
	if (heart_flag) {
		for (auto p = heart.begin(); p != heart.end(); p++)
			p->updata();
		if (heart[10].isFinish())
			heart_flag = false;
	}
}
void reset() {
	static auto it = fire_works.begin();
	if (it != fire_works.end()) {
		int x = 0;
		if (heart_flag) {
			x = rand() % 360;
			if (x & 1)
				x += 480;
		}
		else x= rand() % (Win_width - 20);
		int hy= Win_height / 2 - rand() % 250;
		int speed= -(rand() % 10 + 10);
		it->reset(x,hy,speed);
		it++;
	}
	else it = fire_works.begin();
	if (heart_flag == false) {
		for (auto p = heart.begin(); p != heart.end(); p++) {
			int index = p - heart.begin();
			int v = 0;
			if (index < 10)
				v = heart_speed[index / 2];
			else v = heart_speed[1];
			p->reset(heart_cooradinates[index][0], heart_cooradinates[index][1], v);
		}
	}
	//Sleep(5);
}
void draw() {
	for (auto p = fire_works.begin(); p != fire_works.end(); p++)
		p->draw();
	for (auto p = clicks.begin(); p != clicks.end(); p++)
		p->draw();
	for (auto p = heart.begin(); p != heart.end(); p++)
		p->draw();
}
void clearImage() {
	for (int i = 0; i < 2000; i++)
	{
		int px1 = rand() % Win_width; // 0  to 1199
		int py1 = rand() % (Win_height - 100);  // 0 to 599

		pMem[py1 * Win_width + px1] = BLACK;//将像素点赋值为背景色即为擦除
		pMem[py1 * Win_width + px1 + 1] = BLACK;	// 对显存赋值擦出像素点		
	}
}
void userClick() {
	ExMessage msg;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			int index = rand() % 4;
			FireWork tmp(msg.x, msg.y, index, true);
			clicks.push_back(tmp);
		}
		else if (msg.message == WM_RBUTTONDOWN) {
			heart_flag = true;
			clearImage();
		}
	}
}
void printText() {
	static int Time = 0;
	const int y = Win_height - 100 -textSize * 40;
	Time++;
	outtextxy(450, y + Time / 90 * 40, text[Time / 90]);
	if (Time == 450)
		Time = 0;
}
void count_down() {
	char count_img[5][30] = { "resource\\count_1.png","resource\\count_2.png","resource\\count_3.png","resource\\count_4.png","resource\\count_5.png" };
	for (int i = 0; i < 5; i++) {
		IMAGE img;
		loadimage(&img, count_img[4 - i]);
		putimage(80, 0, &img);
		Sleep(1000);
	}
	cleardevice();
}
void blessing_text() {
	char blessing[6][100] = { "\0" };
	int blessingSize = 0, x, y;
	ifstream fp;
	fp.open("resource\\user_defined_blessing.txt");
	while (blessingSize < 6 && fp >> blessing[blessingSize])
		blessingSize++;
	fp.close();
	x = 450, y = Win_height - 300 - blessingSize * 40;
	for (int i = 0; i < blessingSize; i++) {
		outtextxy(x, y + i * 40, blessing[i]);
		Sleep(1000);
	}
	cleardevice();
	outtextxy(400, 250, "来一场绚丽的烟火表演吧!");
	outtextxy(400, 300, "准备好了吗?");
	Sleep(1500);
	cleardevice();
}
void setTextStyle() {
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	_tcscpy_s(f.lfFaceName, "微软雅黑");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
}