#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<time.h>
#include<Windows.h>
#include<easyx.h>
#include<mmsystem.h>
#include<cmath>
#pragma comment(lib,"winmm.lib")
using namespace std;
extern const int Win_width;
extern const int Win_height;
extern const char images[5][20];
extern const char flowers[5][20];
extern DWORD* pMem;
class FireWork
{
public:
	FireWork();
	FireWork(int, int, int);
	void updata();
	void draw();
	void reset();
	~FireWork();

private:
	IMAGE img;
	int x, y, speed, hy;//当前位置，速度，最高位置
	bool isshoot;
	friend class Bomb;
};
FireWork::FireWork() {}
FireWork::FireWork(int x, int hy, int img_index) {
	this->y = Win_height;
	this->x = x;
	this->speed = -5;
	this->hy = hy;
	this->isshoot = true;
	loadimage(&(this->img), images[img_index]);
}
void FireWork::updata() {
	if (this->isshoot)
		this->y += this->speed;
	if (this->y <= this->hy) {
		this->isshoot = false;
	}
}
void FireWork::draw() {
	if (this->isshoot) {
		putimage(this->x, this->y, &(this->img), SRCINVERT);
	}
}
void FireWork::reset() {
	if (this->isshoot == false) {
		this->x = rand() % (Win_width - 20);
		this->hy = Win_height / 2 - rand() % 200;
		this->speed = -(rand() % 10 + 10);
		this->y = Win_height;
		this->isshoot = true;
	}
}
FireWork::~FireWork()
{
}
class Bomb
{
public:
	Bomb();
	Bomb(int, int, int);
	void draw();
	void reset();
	void updata();
	bool iserase() {//用户点击生成的烟花的销毁函数
		return this->jet.y == Win_height;
	}
	~Bomb();

private:
	IMAGE img;
	vector<vector<int>> pixels;
	const int dtr[9] = { 5,5,7,7,12,12,20,20,27 };
	double t1, t2, dt;//用时间来控制爆炸散开速度
	int center_x, center_y;//圆心坐标
	int x, y;//相对于窗口的坐标;
	int r, max_r;//爆炸半径和最大半径
	bool isdraw;
	FireWork jet;
};
Bomb::Bomb() {}
Bomb::Bomb(int x, int y, int img_index) :jet(x, y, img_index) {
	this->center_x = 90;
	this->center_y = 90;
	this->x = x;
	this->y = y;
	this->t1 = GetTickCount64();
	this->dt = 5;
	this->r = 0;
	this->max_r = 90;
	this->isdraw = false;
	loadimage(&(this->img), flowers[img_index]);
	this->pixels.resize(180, vector<int>(180));
	SetWorkingImage(&(this->img));
	for (int i = 0; i < 180; i++) {
		for (int j = 0; j < 180; j++) {
			this->pixels[i][j] = getpixel(i, j);
		}
	}
	SetWorkingImage(0);
}
void Bomb::updata() {
	this->jet.updata();
	if (this->jet.isshoot == false)
		this->isdraw = true;
}
void Bomb::reset() {
	this->t1 = GetTickCount64();
	this->r = 0;
	this->dt = 5;
	this->x = this->jet.x;
	this->y = this->jet.hy;
}
void Bomb::draw() {
	this->jet.draw();
	this->t2 = GetTickCount64();
	if (this->t2 - this->t1 > this->dt && this->isdraw) {
		if (this->r < this->max_r) {
			this->r++;
			this->dt = this->dtr[this->r / 10];
		}
		if (this->r >= this->max_r - 1) {
			this->isdraw = false;
			this->jet.reset();
			this->reset();
		}
		this->t1 = this->t2;
	}
	if (this->isdraw) {
		for (double i = 0.0; i < 6.28; i += 0.01) {
			double degree_x = this->r * cos(i);
			double degree_y = this->r * sin(i);
			int x1 = (int)(this->center_x + degree_x);
			int y1 = (int)(this->center_y + degree_y);
			int xx = this->x + degree_x;
			int yy = this->y + degree_y;
			if (x1 >= 0 && x1 < 180 && y1 >= 0 && y1 < 180 && xx >= 0 && xx <= Win_width && yy >= 0 && yy <= Win_height)
				pMem[yy * Win_width + xx] = BGR(this->pixels[x1][y1]);
		}
	}
}
Bomb::~Bomb()
{
}