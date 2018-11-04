#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"

class Scene2D : public Camera2D
{
private:
	typedef double (*Func)(double);
public:
	Scene2D(double L, double R, double B, double T) : Camera2D(L, R, B, T)
	{
	//	this->B = B;
	//	this->R = R;
	//	this->L = L;
	//	this->T = T;
	}
	void Plot(HDC dc, Func f, bool axes=true)
	{
		if (axes)
			Axes(dc);

		RECT r;
		GetClientRect(WindowFromDC(dc), &r);

		//le costyle - чтобы ось не окрашивалась в синий
		LineTo1(dc, ScreenToWorldX(0), f(ScreenToWorldX(0)));

		HPEN pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 3, RGB(0, 0, 255)));

		for (int pixel = 0; pixel < r.right; pixel++)
		{
			LineTo1(dc, ScreenToWorldX(pixel), f(ScreenToWorldX(pixel)));
		}
		pen = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
	}
};

#endif SCENE_2D_H
