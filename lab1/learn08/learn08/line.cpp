
#include <math.h>
#include "common.h"
#include "line.h"

//DDA算法绘制直线
//(xa,ya),起点像素坐标
//(xb,yb),终点像素坐标
//setPixel,像素处理函数
void DDA(int xa, int ya, int xb, int yb, void(*setPixel)(int x, int y)) {
	//Bres(xa, ya, xb, yb, setPixel);
	//添加代码

	float x = 0.0;
	float y = 0.0;
	float m = 0.0;
	// 添加增量，实现增量思想
	float dx = xb - xa;
	float dy = yb - ya;

	//
	if (dx != 0)
	{
		m = dy / dx;
		if (m <= 1 && m >= -1)
		{
			//
			if (xa > xb) {
				int helper = xa;
				xa = xb;
				xb = helper;
				//
				helper = ya;
				ya = yb;
				yb = helper;
			}
			//

			y = ya;
			for (x = xa; x <= xb; x++)
			{
				setPixel(x, int(y + 0.5));
				y += m;
			}
		}
		if (m > 1 || m < -1)
		{
			m = 1 / m;

			//
			if (ya > yb) {
				int helper = xa;
				xa = xb;
				xb = helper;
				//
				helper = ya;
				ya = yb;
				yb = helper;
			}
			//

			x = xa;
			for (y = ya; y <= yb; y++)
			{
				setPixel(int(x + 0.5), y);
				x += m;
			}
		}
	}
	else
	{
		int x = xa;
		int y = 0;
		y = (ya <= yb) ? ya : yb;
		int d;
		//d = fabs((double)(ya - yb));
		d = abs(ya - yb);
		while (d >= 0)
		{
			setPixel(x, y);
			y++;
			d--;
		}
	}


}

//Bresenham算法绘制直线
//参数同DDA
void Bres(int xa, int ya, int xb, int yb, void(*setPixel)(int x, int y)) {

	bool u = false, v = false, w = false;
	int tx, ty;
	int dsx = xb - xa;
	int dsy = yb - ya;

	if (dsx < 0) {
		dsx = -dsx;//dsx取反
		u = true;
	}
	if (dsy < 0) {
		dsy = -dsy;//dsy取反
		v = true;
	}
	if (dsx < dsy) {
		swap(dsx, dsy);//dsx,dsy互换
		w = true;
	}

	int dx = dsx, dy = dsy;
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyDx = 2 * (dy - dx);
	int x, y, xEnd;

	x = 0;
	y = 0;
	xEnd = dsx;
	setPixel(xa, ya);

	while (x < xEnd) {
		x++;

		if (p < 0)
			p += twoDy;
		else {
			y++;
			p += twoDyDx;
		}
		tx = ROUND(x);
		ty = ROUND(y);

		if (w)swap(tx, ty);
		if (v)ty = -ty;
		if (u)tx = -tx;

		tx += xa;
		ty += ya;
		setPixel(tx, ty);
	}
}