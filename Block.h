#ifndef _BLOCK_H
#define _BLOCK_H 

#include<graphics.h>
#include<unordered_map>
#include<string>
#include<iostream>

#define blockSize 100
#define lineSize 15
#define roundSize 40

enum Color
{
	cb = RGB(187, 173, 160),
	c0 = RGB(205, 193, 180),
	c2 = RGB(238, 228, 218),
	c4 = RGB(237, 224, 200),
	c8 = RGB(242, 177, 121),
	c16 = RGB(245, 149, 99),
	c32 = RGB(246, 124, 121),
	c64 = RGB(246, 94, 59),
	c128 = RGB(242, 177, 121),
	c256 = RGB(237, 204, 97),
	c512 = RGB(255, 0, 128),
	c1024 = RGB(145, 0, 72),
	c2048 = RGB(242, 17, 158),
	color_ERROR = RGB(255, 0, 0)
};
enum Direction {
	left = 1,
	right = 2,
	up = 3,
	down = 4,
};
std::unordered_map<int, Color> colorMap = {
	{0,c0},
	{2,c2},
	{4,c4},
	{8,c8},
	{16,c16},
	{32,c32},
	{64,c64},
	{128,c128},
	{256,c256},
	{512,c512},
	{1024,c1024},
	{2048,c2048}
};
class Block
{
public:
	int x;
	int y;
	int num;
	bool mixed; //当前回合中是否被合并
	Color color;

	Block(int x, int y, int num)
	{
		this->x = x;
		this->y = y;
		setNum(num);
		this->mixed = false;
	}
	void setNum(int num)
	{
		this->num = num;
		auto it = colorMap.find(num);
		if (it == colorMap.end())
			this->color = color_ERROR; // 未被定义，用红色标出
		else
			this->color = it->second;

	}

	void draw(void)
	{
		int l = (this->x + 1) * lineSize + (this->x) * blockSize;
		int r = (this->x + 1) * lineSize + (this->x + 1) * blockSize;
		int t = (this->y + 1) * lineSize + (this->y) * blockSize;
		int b = (this->y + 1) * lineSize + (this->y + 1) * blockSize;
		setfillcolor(this->color);
		solidroundrect(l, t, r, b, 40, 40);

		//文本
		if (this->isNotEmpty())
		{
			settextcolor(RGB(0, 0, 0));
			settextstyle(50, 0, "Consolas");
			setbkmode(TRANSPARENT);
			std::cout << this->x << " " << this->y << " " << this->num << std::endl;
			char s[5] = "";
			sprintf(s, "%d", this->num);
			int text_x = (l + r - textwidth(s)) / 2;
			int text_y = (t + b - textheight(s)) / 2;
			outtextxy(text_x, text_y, s);
		}

	}

	bool isEmpty(void)
	{
		return this->num == 0;
	}

	bool isNotEmpty(void)
	{
		return this->num != 0;
	}
};
#endif // !_BLOCK_H
