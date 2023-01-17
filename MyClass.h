#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<string>
#include<time.h>
#include<Windows.h>
#include<easyx.h>
#include<mmsystem.h>
#include<cmath>
#pragma comment(lib,"winmm.lib")
using namespace std;
extern const int Win_width;
extern const int Win_height;
extern const char images[5][30];
extern const char flowers[5][30];
extern DWORD* pMem;
class Jet
{
public:
	Jet();
	Jet(int, int, int ,int);
	void updata();
	void draw();
	void reset(int,int,int);
	//int getX() {//获取当前烟花弹的x坐标
	//	return this->x;
	//}
	//int getHy() {//获取当前烟花弹的最高位置y坐标
	//	return this->hy;
	//}
	bool shoot_finish() {
		return !this->isshoot;
	}
	~Jet();

private:
	IMAGE img;
	int x, y, speed, hy;//当前位置，速度，最高位置
	bool isshoot;
};
Jet::Jet() {}
Jet::Jet(int x, int hy, int img_index, int speed =  -10) {
	this->y = Win_height;//烟花弹初始位置设置为屏幕最下方
	this->x = x;//初始x坐标设置为传入参数
	this->speed = speed;//初始上升速度设置为-10
	this->hy = hy;//初始最高位置y坐标设置为传入参数
	this->isshoot = true;//初始发生状态设置为发射中
	loadimage(&(this->img), images[img_index]);//加载图片
}
void Jet::updata() {
	if (this->isshoot) {//如果烟花弹处于发生中，根据速度修改y坐标
		this->y += this->speed;
	}
	if (this->y <= this->hy) {//如果烟花弹到达最高位置，发射状态修改为false
		this->isshoot = false;
	}
}
void Jet::draw() {
	if (this->isshoot) {//如果烟花弹处于发射中，贴图
		putimage(this->x, this->y, &(this->img), SRCINVERT);
	}
}
void Jet::reset(int x,int hy,int speed) {//发射并绽放完成后烟花弹进行重置，节省空间
	this->x = x;//rand() % (Win_width - 20);//重置的烟花弹的起始坐标和速度都随机
	this->hy = hy;//Win_height / 2 - rand() % 200;
	this->speed = speed;//-(rand() % 10 + 10);
	this->y = Win_height;//起始y坐标仍为屏幕最下方
	this->isshoot = true;//设置发射状态为true
}
Jet::~Jet()
{
}
class Bomb
{
public:
	Bomb();
	Bomb(int, int, int);
	void draw();
	void reset(int,int);
	void updata();
	bool bomb_finish() {//烟花是否绽放完成
		return this->isbomb;
	}
	void set_draw(bool isdraw) {//设置绽放状态
		this->isdraw = isdraw;
	}
	~Bomb();

private:
	IMAGE img;
	vector<vector<int>> pixels;
	int dtr[4] = { 5,5,7,7};
	double t1, t2, dt;//用时间来控制爆炸散开速度
	int center_x, center_y;//圆心坐标
	int x, y;//相对于窗口的坐标;
	int r, max_r;//爆炸半径和最大半径
	bool isdraw, isbomb;//是否开始渲染，是否爆炸完成
};
Bomb::Bomb() {}
Bomb::Bomb(int x, int y, int img_index){
	this->center_x = 40;//烟花绽放时的圆心坐标设置为图片的中心坐标
	this->center_y = 40;
	this->x = x;//烟花绽放时的相对于窗口的坐标设为传入的参数(即为烟花弹的起始x坐标和最高位置y坐标)
	this->y = y;
	this->t1 = GetTickCount64();//速度由时间间隔控制
	this->dt = 5;//初始时间间隔设置为5ms，随着绽放半径增大而增加
	this->r = 0;//绽放的初始半径
	this->max_r = 40;//绽放的最大半径
	this->isdraw = false;//初始的渲染状态为false
	this->isbomb = false;//初始的绽放状态为false
	loadimage(&(this->img), flowers[img_index]);//加载图片
	this->pixels.resize(80, vector<int>(80));//获取图片像素点
	SetWorkingImage(&(this->img));
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			this->pixels[i][j] = getpixel(i, j);
		}
	}
	SetWorkingImage(0);
}
void Bomb::reset(int x,int y) {
	this->t1 = GetTickCount64();//重新获取时间间隔
	this->r = 0;//恢复初始半径
	this->dt = 5;//恢复初始时间间隔
	this->x = x;//设置绽放时的相对于窗口的坐标
	this->y = y;
	this->isbomb = false;//设置绽放状态为false
	this->isdraw = false;//设置绽放状态为false
}
void Bomb::draw() {
	this->t2 = GetTickCount64();//获取当前时间
	if (this->t2 - this->t1 > this->dt && this->isdraw) {//判断时间间隔大小和是否绽放完成
		if (this->r < this->max_r) {//绽放半径和速度随绽放时间增长
			this->r++;
			this->dt = this->dtr[this->r / 10];
			//this->isdraw = true;
		}
		if (this->r >= this->max_r - 1) {
			this->isdraw = false;
			this->isbomb = true;
		}
		this->t1 = this->t2;
	}
	if (this->isdraw) {//如果允许渲染，则根据圆的参数方程计算绽放中心周围像素点的真实坐标，和相对于图片中心的坐标
		for (double i = 0.0; i < 6.28; i += 0.01) {
			double degree_x = this->r * cos(i);//像素点的角度值
			double degree_y = this->r * sin(i);
			int x1 = (int)(this->center_x + degree_x);//像素点相对于图片中心的坐标
			int y1 = (int)(this->center_y + degree_y);
			int xx = this->x + degree_x;//像素点相对于窗口的坐标
			int yy = this->y + degree_y;
			//判断越界,并输出到显存
			if (x1 >= 0 && x1 < 80 && y1 >= 0 && y1 < 80 && xx >= 0 && xx <= Win_width && yy >= 0 && yy <= Win_height)
				pMem[yy * Win_width + xx] = BGR(this->pixels[x1][y1]);
		}
	}
}
Bomb::~Bomb()
{
}
class FireWork
{
public:
	FireWork();
	FireWork(int, int, int, bool);
	void updata();
	void draw();
	void reset(int,int,int);
	bool isFinish() {
		return this->bomb.bomb_finish();
	}
	~FireWork();

private:
	Jet jet;//将烟花拆分成烟花弹和爆炸绽放两部分
	Bomb bomb;
	bool iserase;//用来标记是否是鼠标点击生成的烟花，如果是则会在绽放结束后被析构，如果是初始时的烟花则不会
};

FireWork::FireWork():iserase(false){}
FireWork::FireWork(int x, int hy, int img_index,bool erase = false) :jet(x, hy, img_index), bomb(x, hy, img_index), iserase(erase) {
}
void FireWork::updata() {
	this->jet.updata();//先更新烟花弹的位置
	if (this->jet.shoot_finish()) {//如果烟花弹发射结束，则烟花图案绽放{
		this->bomb.set_draw(true);
	}

}
//在窗口中画烟花弹和烟花绽放
void FireWork::draw() {
	this->jet.draw();
	this->bomb.draw();
}
//初始的烟花绽放完后重置属性，继续使用
void FireWork::reset(int x,int hy,int speed) {
	//如果不是鼠标点击生成的烟花且已经绽放完成则重置
	if (this->iserase == false&&this->bomb.bomb_finish()) {
		this->jet.reset(x,hy,speed);
		this->bomb.reset(x,hy);
	}
}
FireWork::~FireWork()
{
	cout << "烟花类的析构函数被调用" << endl;
}