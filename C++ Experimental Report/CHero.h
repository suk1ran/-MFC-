#pragma once


struct Point
{
	int x;
	int y;
};

class CHero
{
public:
	CHero();
	~CHero();
	int direc;//�˶�����
	void UpMove();
	void DownMove();
	void LeftMove();
	void RightMove();
	void HeroInit();

private:

};

