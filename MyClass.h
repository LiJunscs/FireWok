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
	//int getX() {//��ȡ��ǰ�̻�����x����
	//	return this->x;
	//}
	//int getHy() {//��ȡ��ǰ�̻��������λ��y����
	//	return this->hy;
	//}
	bool shoot_finish() {
		return !this->isshoot;
	}
	~Jet();

private:
	IMAGE img;
	int x, y, speed, hy;//��ǰλ�ã��ٶȣ����λ��
	bool isshoot;
};
Jet::Jet() {}
Jet::Jet(int x, int hy, int img_index, int speed =  -10) {
	this->y = Win_height;//�̻�����ʼλ������Ϊ��Ļ���·�
	this->x = x;//��ʼx��������Ϊ�������
	this->speed = speed;//��ʼ�����ٶ�����Ϊ-10
	this->hy = hy;//��ʼ���λ��y��������Ϊ�������
	this->isshoot = true;//��ʼ����״̬����Ϊ������
	loadimage(&(this->img), images[img_index]);//����ͼƬ
}
void Jet::updata() {
	if (this->isshoot) {//����̻������ڷ����У������ٶ��޸�y����
		this->y += this->speed;
	}
	if (this->y <= this->hy) {//����̻����������λ�ã�����״̬�޸�Ϊfalse
		this->isshoot = false;
	}
}
void Jet::draw() {
	if (this->isshoot) {//����̻������ڷ����У���ͼ
		putimage(this->x, this->y, &(this->img), SRCINVERT);
	}
}
void Jet::reset(int x,int hy,int speed) {//���䲢������ɺ��̻����������ã���ʡ�ռ�
	this->x = x;//rand() % (Win_width - 20);//���õ��̻�������ʼ������ٶȶ����
	this->hy = hy;//Win_height / 2 - rand() % 200;
	this->speed = speed;//-(rand() % 10 + 10);
	this->y = Win_height;//��ʼy������Ϊ��Ļ���·�
	this->isshoot = true;//���÷���״̬Ϊtrue
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
	bool bomb_finish() {//�̻��Ƿ��������
		return this->isbomb;
	}
	void set_draw(bool isdraw) {//��������״̬
		this->isdraw = isdraw;
	}
	~Bomb();

private:
	IMAGE img;
	vector<vector<int>> pixels;
	int dtr[4] = { 5,5,7,7};
	double t1, t2, dt;//��ʱ�������Ʊ�ըɢ���ٶ�
	int center_x, center_y;//Բ������
	int x, y;//����ڴ��ڵ�����;
	int r, max_r;//��ը�뾶�����뾶
	bool isdraw, isbomb;//�Ƿ�ʼ��Ⱦ���Ƿ�ը���
};
Bomb::Bomb() {}
Bomb::Bomb(int x, int y, int img_index){
	this->center_x = 40;//�̻�����ʱ��Բ����������ΪͼƬ����������
	this->center_y = 40;
	this->x = x;//�̻�����ʱ������ڴ��ڵ�������Ϊ����Ĳ���(��Ϊ�̻�������ʼx��������λ��y����)
	this->y = y;
	this->t1 = GetTickCount64();//�ٶ���ʱ��������
	this->dt = 5;//��ʼʱ��������Ϊ5ms���������Ű뾶���������
	this->r = 0;//���ŵĳ�ʼ�뾶
	this->max_r = 40;//���ŵ����뾶
	this->isdraw = false;//��ʼ����Ⱦ״̬Ϊfalse
	this->isbomb = false;//��ʼ������״̬Ϊfalse
	loadimage(&(this->img), flowers[img_index]);//����ͼƬ
	this->pixels.resize(80, vector<int>(80));//��ȡͼƬ���ص�
	SetWorkingImage(&(this->img));
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			this->pixels[i][j] = getpixel(i, j);
		}
	}
	SetWorkingImage(0);
}
void Bomb::reset(int x,int y) {
	this->t1 = GetTickCount64();//���»�ȡʱ����
	this->r = 0;//�ָ���ʼ�뾶
	this->dt = 5;//�ָ���ʼʱ����
	this->x = x;//��������ʱ������ڴ��ڵ�����
	this->y = y;
	this->isbomb = false;//��������״̬Ϊfalse
	this->isdraw = false;//��������״̬Ϊfalse
}
void Bomb::draw() {
	this->t2 = GetTickCount64();//��ȡ��ǰʱ��
	if (this->t2 - this->t1 > this->dt && this->isdraw) {//�ж�ʱ������С���Ƿ��������
		if (this->r < this->max_r) {//���Ű뾶���ٶ�������ʱ������
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
	if (this->isdraw) {//���������Ⱦ�������Բ�Ĳ������̼�������������Χ���ص����ʵ���꣬�������ͼƬ���ĵ�����
		for (double i = 0.0; i < 6.28; i += 0.01) {
			double degree_x = this->r * cos(i);//���ص�ĽǶ�ֵ
			double degree_y = this->r * sin(i);
			int x1 = (int)(this->center_x + degree_x);//���ص������ͼƬ���ĵ�����
			int y1 = (int)(this->center_y + degree_y);
			int xx = this->x + degree_x;//���ص�����ڴ��ڵ�����
			int yy = this->y + degree_y;
			//�ж�Խ��,��������Դ�
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
	Jet jet;//���̻���ֳ��̻����ͱ�ը����������
	Bomb bomb;
	bool iserase;//��������Ƿ�����������ɵ��̻����������������Ž���������������ǳ�ʼʱ���̻��򲻻�
};

FireWork::FireWork():iserase(false){}
FireWork::FireWork(int x, int hy, int img_index,bool erase = false) :jet(x, hy, img_index), bomb(x, hy, img_index), iserase(erase) {
}
void FireWork::updata() {
	this->jet.updata();//�ȸ����̻�����λ��
	if (this->jet.shoot_finish()) {//����̻���������������̻�ͼ������{
		this->bomb.set_draw(true);
	}

}
//�ڴ����л��̻������̻�����
void FireWork::draw() {
	this->jet.draw();
	this->bomb.draw();
}
//��ʼ���̻���������������ԣ�����ʹ��
void FireWork::reset(int x,int hy,int speed) {
	//���������������ɵ��̻����Ѿ��������������
	if (this->iserase == false&&this->bomb.bomb_finish()) {
		this->jet.reset(x,hy,speed);
		this->bomb.reset(x,hy);
	}
}
FireWork::~FireWork()
{
	cout << "�̻������������������" << endl;
}