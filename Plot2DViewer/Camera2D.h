#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>
#include <iostream>
#include <fstream>

class Camera2D
{
protected:
	double offsetX, offsetY;
	double mapping;
	double L, R, B, T;					// ������� ���������� ������ ������� ������� ����
	int W, H;							// ���������� ������� ������� ����
	int WorldToScreenX(double X)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (X - L) * W * mapping / (R - L);
	}
	
	int WorldToScreenY(double Y)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (T - Y) * H * mapping / (T - B);
	}
	double ScreenToWorldX(int X)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return X * (R - L) * 1.0 / (W * mapping) + L;
	}
	double ScreenToWorldY(int Y)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return T - (T - B) * 1.0 * Y / (H * mapping);
	}
private:
	bool isDragging;
	double posX, posY;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{
		this->L = L;
		this->R = R;
		this->B = B;
		this->T = T;
		mapping = 1;
		W = 1;
		H = 1;
		isDragging = false;
		offsetX = 0;
		offsetY = 0;
		
	}
	void Clear(HDC dc)
	{
		// ������� ������� ������� ����
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		Rectangle(dc, 0, 0, r.right, r.bottom);
		SetResolution(dc);
	}
	void SetResolution(HDC dc)
	{
		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		if (W == 1 && H == 1) {
			W = r.right - r.left;
			H = r.bottom - r.top;
		}
	
	//	this->L = ScreenToWorldX(r.left);
	//	this->R = ScreenToWorldX(r.right);
	//	this->B = ScreenToWorldY(r.bottom);
	//	this->T = ScreenToWorldY(r.top);
		
		
	}
	void MoveTo(double X, double Y)
	{
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
		posX = X;
		posY = Y;
	}
	void LineTo1(HDC dc, double X, double Y)
	{
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)
		MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		posX = X;
		posY = Y;
		//MoveTo(X, Y);
	}
	void Axes(HDC dc)
	{
		MoveTo(0, ScreenToWorldY(0));
		LineTo1(dc, 0, ScreenToWorldY(H));

		MoveTo(ScreenToWorldX(0), 0);
		LineTo1(dc, ScreenToWorldX(W), 0);

		MoveTo(ScreenToWorldX(0), 0);
		// ��������� ������������ ����
	}

	void StartDragging(/*HDC dc,*/ int X, int Y)
	{
		/* ... */
		//this->dc = dc;
		isDragging = true;
		offsetX = ScreenToWorldX(X);
		offsetY = ScreenToWorldY(Y);
	}

	void Drag(int X, int Y)
	{
		if (offsetX - ScreenToWorldX(X) < 0) {
			//means moving to left
			L -= 0.1;
			R -= 0.1;
		} else 
		if (offsetX - ScreenToWorldX(X) > 0) {
			//means moving to right
			L += 0.1;
			R += 0.1;
		} 
		if (offsetY - ScreenToWorldY(Y) < 0) {
			//means moving to bottom
			B -= 0.1;
			T -= 0.1;
			std::fstream fout("filek.txt", std::ios::app);
			fout << "Bottom: " << T << " " << B << ";\n";
		} else
		if (offsetY - ScreenToWorldY(Y) > 0) {
			//means moving to bottom
		//	B += offsetY - ScreenToWorldY(Y);
		//	T += offsetY - ScreenToWorldY(Y);
			B += 0.1;
			T += 0.1;
			std::fstream fout("filek.txt", std::ios::app);
			fout << "Top: " << T << " " << B << ";\n";
		}
		offsetX = ScreenToWorldX(X);
		offsetY = ScreenToWorldY(Y);
		//L += X;
		//T += Y;
	//	R += X;
		
		//B += ScreenToWorldY(Y);
	}
	void StopDragging()
	{
		isDragging = false;
		
	}
	bool IsDragging()
	{
		return isDragging;
	}

	void IncreaseSize() {
		mapping += 0.1;
	}

	void DecreaseSize() {
		mapping -= 0.1;
	}
};

#endif CAMERA_2D_H
