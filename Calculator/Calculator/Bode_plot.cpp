// Bode_plot.cpp: 实现文件
//

#include "stdafx.h"
#include "Bode_plot.h"
#include "math.h"
#include "afxdialogex.h"
#include "Resource.h"
//#include "Equation.h"

// Bode_plot 对话框
#define x_dec 1200/x_range			//一个完整的十倍频所占的像素数
#define phi 3.14159265358979323846
#define MAX_VALUE_SIZE 50

IMPLEMENT_DYNAMIC(Bode_plot, CDialogEx)

Bode_plot::Bode_plot(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	
}

Bode_plot::~Bode_plot()
{
	
}

void Bode_plot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Bode_plot, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Bode_plot::OnBnClickedButton1)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUTTON_comp, &Bode_plot::OnButtonComp)
END_MESSAGE_MAP()

bool plot_ex = false, comp_fun = false;
int lable = 10, n = 1;
double zeta;
bool Z_exist, C_exist;
CEdit *pInputZeta;
CMFCButton *pButComp;
CString x_lable_1[15] = { L"1e-5", L"1e-4",L"0.001",L"0.01",L"0.1",L"1",L"10" ,L"100" ,L"1K" ,L"10K" ,L"100K" ,L"1M" ,L"10M",L"100M",L"1G" };
CString x_lable_2[9] = { L" ", L"2",L"3",L"4" ,L"5" ,L"6" ,L"7" ,L"8" ,L"9" };
CString y_lable_1[9] = { L" 0" ,L"20" ,L"40",L"60",L"80",L"100",L"120",L"140",L"160" };
CString y_lable_2[8] = { L"-20" ,L"-40",L"-60",L"-80",L"-100",L"-120",L"-140",L"-160" };
CString y_lable_3[5] = { L" 0" ,L"45" ,L"90",L"135",L"180" };
CString y_lable_4[4] = { L"-45" ,L"-90",L"-135",L"-180" };

void Bode_plot::OnBnClickedButton1()
{
	plot_ex = true;
	MoveWindow(160, 20, 1520, 1050);			//设置对话框的初始位置和大小
	CDC *pDC = GetDC(), MemDC;					//用于绘制的设备上下文
	CRect rect;
	CBitmap MemBitmap;
	CPen pen_axis(PS_SOLID, 1, RGB(0, 0, 0));			//创建绘制坐标系的画笔
	CPen pen_bode(PS_SOLID, 2, RGB(255, 0, 0));			//创建绘制波特图图像的画笔
	GetClientRect(&rect);
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC, 1520, 1050);
	MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(rect, RGB(235, 235, 235));

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
	if (Function) 
	{
		double dB_value[MAX_VALUE_SIZE * 14 + 1], Ang_value[MAX_VALUE_SIZE * 14 + 1];		//存放频率响应函数的计算结果
		double max_dB, min_dB, max_Ang, min_Ang;			//幅频响应函数中的最大值和最小值
		//绘图时所取的各参数点,下一个周期的参数点为当前周期参数点的值*10
		double x_value[MAX_VALUE_SIZE], differ = (1.0 / MAX_VALUE_SIZE);

		if (left_num[4] == 0 && left_num[3] == 0 && left_num[2] == 0 && left_num[1] == 0 && left_num[0] == 1)
		{
			if (right_num[5] == 0 && right_num[4] == 0 && right_num[3] == 0 && right_num[2] != 0 && right_num[0] == 1)
			{
				if (Z_exist)
				{
					delete pInputZeta;
					Z_exist = false;
				}
				if (C_exist)
				{
					delete pButComp;
					C_exist = false;
				}
				pInputZeta = new(CEdit);
				pInputZeta->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_LEFT , CRect(120, 15, 300, 40), this, ID_BOX_zeta);
				Z_exist = true;
				pButComp = new CMFCButton;
				pButComp->Create(L"生成对比曲线", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(300, 15, 400, 40), this, ID_BUTTON_comp);
				C_exist = true;
				pInputZeta->SetWindowTextW(L"0:1");
				MemDC.TextOutW(400, 15, L"输入阻尼比范围，格式：A:B");
				zeta = right_num[1] / (2 * sqrt(right_num[2]));
			}
		}

		for (int i = 0; i < MAX_VALUE_SIZE; i++) 			//10^(0:differ:1)
		{
			x_value[i] = pow(10, i*differ);
		}
		for (int i = 0; i <= MAX_VALUE_SIZE * 14; i++)
		{
			double freq = x_value[i % MAX_VALUE_SIZE] * pow(10, i / MAX_VALUE_SIZE - 5);
			double A_i = left_num[1] * freq - left_num[3] * pow(freq, 3),
				A_r = left_num[0] - left_num[2] * pow(freq, 2) + left_num[4] * pow(freq, 4),
				B_i = right_num[1] * freq - right_num[3] * pow(freq, 3) + right_num[5] * pow(freq, 5),
				B_r = right_num[0] - right_num[2] * pow(freq, 2) + right_num[4] * pow(freq, 4);
			dB_value[i] = 10 * log10(pow(A_i, 2) + pow(A_r, 2)) - 10 * log10(pow(B_i, 2) + pow(B_r, 2));
			double Ang_A = atan(A_i / A_r), Ang_B = atan(B_i / B_r);

			//将反正切函数的运算结果由两象限转换到四象限
			if (A_r < 0)
			{
				if (A_i >= 0)
				{
					Ang_A += phi;
				}
				else
				{
					Ang_A -= phi;
				}
			}
			if (B_r < 0)
			{
				if (B_i >= 0)
				{
					Ang_B += phi;
				}
				else
				{
					Ang_B -= phi;
				}
			}
			Ang_value[i] = (Ang_A - Ang_B) / phi * 180;
		}

		//在已经计算完显示范围内所有的数据后，每个十倍频内取十个采样点，找出有效范围
		for (int i = MAX_VALUE_SIZE * 14; i >= 0; i -= MAX_VALUE_SIZE / 10)
		{
			double test = abs(dB_value[i] - dB_value[i - MAX_VALUE_SIZE]) / 20, tmp;
			if (modf(test, &tmp) > 0.5 && modf(test, &tmp) < 0.99)
			{
				R_range = i / MAX_VALUE_SIZE + 1;
				if (R_range > 12)	R_range = 12;
				break;
			}
		}
		for (int i = 0; i <= MAX_VALUE_SIZE * 14; i += MAX_VALUE_SIZE / 10)
		{
			double test = abs(dB_value[i] - dB_value[i + MAX_VALUE_SIZE]);
			if (test > 0.1)
			{
				L_range = i / MAX_VALUE_SIZE;
				if (L_range < 0)	L_range = 0;
				break;
			}
		}
		x_range = R_range - L_range;

		for (int i = 0; i <= MAX_VALUE_SIZE * x_range; i++)
		{
			dB_value[i] = dB_value[i + MAX_VALUE_SIZE * L_range];
			Ang_value[i] = Ang_value[i + MAX_VALUE_SIZE * L_range];
			if (i == 0) {
				max_dB = dB_value[i];
				min_dB = dB_value[i];
				max_Ang = Ang_value[i];
				min_Ang = Ang_value[i];
			}
			else
			{
				if (max_dB < dB_value[i])		max_dB = dB_value[i];
				if (min_dB > dB_value[i])		min_dB = dB_value[i];
				if (max_Ang < Ang_value[i])		max_Ang = Ang_value[i];
				if (min_Ang > Ang_value[i])		min_Ang = Ang_value[i];
			}
		}

		//---------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------
		//绘制幅频特性曲线

		int x_axis[9] = { 0,0.301*x_dec,0.477*x_dec,0.602*x_dec,0.699*x_dec,0.778*x_dec,0.845*x_dec,0.903*x_dec,0.954*x_dec };	//对数坐标系的坐标点参照(像素值)
		int OrX = 50, dBY = 50, AngY = 550;
		//首先将幅频响应曲线的幅度范围量化为以20为间隔的若干个区间,并计算每个区间分配到的像素点数
		if (min_dB > 0)
		{
			if (modf(max_dB / 20, &max_dB) > 0.2)
				max_dB += 1;
			min_dB = 0;
		}
		else
		{
			if (max_dB < 0)
			{
				max_dB = 0;
				if (modf(abs(min_dB / 20), &min_dB) > 0.2)
					min_dB += 1;
			}
			else
			{
				if (modf(max_dB / 20, &max_dB) > 0.2)
					max_dB += 1;
				if (modf(abs(min_dB / 20), &min_dB) > 0.2)
					min_dB += 1;
			}
		}
		y_dec = 450 / (max_dB + min_dB + 1);

		MemDC.SelectObject(pen_axis);
		//绘制x轴
		for (int i = 0; i <= max_dB; i++)
		{
			int y = 60 + y_dec * i;
			MemDC.MoveTo(OrX - 20, y);
			MemDC.LineTo(1280, y);
			MemDC.TextOutW(OrX - 20, y - 15, y_lable_1[int(max_dB) - i]);			
			if (i == max_dB)		dB_y_axis = y;
		}
//		MemDC.TextOutW(1250, dB_y_axis + 5, L"Hz");
		for (int i = 0; i < min_dB; i++)
		{
			int y = 60 + y_dec * (i + 1 + max_dB);
			MemDC.MoveTo(OrX - 20, y);
			MemDC.LineTo(1280, y);
			MemDC.TextOutW(OrX - 20, y - 15, y_lable_2[i]);
		}
//		MemDC.TextOutW(OrX - 20, 40, L"dB");
		//绘制y轴
		for (int i = 0; i <= 9 * x_range; i++)
		{
			int x = OrX + x_axis[i % 9] + x_dec * (i / 9);
			MemDC.MoveTo(x, dBY + 450);
			MemDC.LineTo(x, dBY);
			if (i % 9 == 0)
			{
				MemDC.TextOutW(x + 2, dB_y_axis - 20, x_lable_1[i / 9 + L_range]);
			}
			else
			{
				MemDC.TextOutW(x + 2, dB_y_axis - 20, x_lable_2[i % 9]);
			}
		}

		//根据计算结果绘制波特图
		MemDC.SelectObject(pen_bode);
		MemDC.MoveTo(OrX, dB_y_axis - dB_value[0] / 20 * y_dec);
		for (int i = 0; i <= MAX_VALUE_SIZE * x_range; i++)
		{
			double X = OrX + (log10(x_value[i % MAX_VALUE_SIZE]) * x_dec) + (i / MAX_VALUE_SIZE) * x_dec;
			double Y = dB_y_axis - dB_value[i] / 20 * y_dec;
			MemDC.LineTo(X, Y);
		}
		//----------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------
		//绘制相频特性曲线	
		if (min_Ang > 0)
		{
			max_Ang = int(max_Ang + 45) / 45;
			min_Ang = 0;
		}
		else
		{
			if (max_Ang < 0)
			{
				max_Ang = 0;
				min_Ang = abs(int(min_Ang - 45) / 45);
			}
			else
			{
				max_Ang = int(max_Ang + 45) / 45;
				min_Ang = abs(int(min_Ang - 45) / 45);
			}
		}

		y_dec = 450 / (max_Ang + min_Ang + 1);

		MemDC.SelectObject(pen_axis);
		//绘制x轴
		for (int i = 0; i <= max_Ang; i++)
		{
			int y = 560 + y_dec * i;
			MemDC.MoveTo(OrX - 20, y);
			MemDC.LineTo(1280, y);
			MemDC.TextOutW(OrX - 20, y - 15, y_lable_3[int(max_Ang) - i]);
			if (i == max_Ang)		Ang_y_axis = y;
		}
//		MemDC.TextOutW(1250, Ang_y_axis + 5, L"Hz");
		for (int i = 0; i < min_Ang; i++)
		{
			int y = 560 + y_dec * (i + 1 + max_Ang);
			MemDC.MoveTo(OrX - 20, y);
			MemDC.LineTo(1280, y);
			MemDC.TextOutW(OrX - 20, y - 15, y_lable_4[i]);
		}
//		MemDC.TextOutW(OrX - 20, 540, L"Degree");
		//绘制y轴
		for (int i = 0; i <= 9 * x_range; i++)
		{
			int x = OrX + x_axis[i % 9] + x_dec * (i / 9);
			MemDC.MoveTo(x, AngY + 450);
			MemDC.LineTo(x, AngY);
			if (i % 9 == 0)
			{
				MemDC.TextOutW(x + 2, Ang_y_axis - 20, x_lable_1[i / 9 + L_range]);
			}
			else
			{
				MemDC.TextOutW(x + 2, Ang_y_axis - 20, x_lable_2[i % 9]);
			}
		}

		//根据计算结果绘制波特图
		MemDC.SelectObject(pen_bode);
		MemDC.MoveTo(OrX, Ang_y_axis - Ang_value[0] / 45 * y_dec);
		for (int i = 1; i <= MAX_VALUE_SIZE * x_range; i++)
		{
			double X = OrX + (log10(x_value[i % MAX_VALUE_SIZE]) * x_dec) + i / MAX_VALUE_SIZE * x_dec;
			double Y = Ang_y_axis - Ang_value[i] / 45 * y_dec;
			MemDC.LineTo(X, Y);
		}
	}
	else 
	{
	//Nyquist曲线绘制函数
		MoveWindow(160, 20, 620, 605);			//设置对话框的初始位置和大小

		double Re[MAX_VALUE_SIZE * 12 + 1], Im[MAX_VALUE_SIZE * 12 + 1], Ang_value[MAX_VALUE_SIZE * 12 + 1];		//存放频率响应函数的计算结果
		double max = 0;			//曲线中的最大值
		double tmp;
		//绘图时所取的各参数点,下一个周期的参数点为当前周期参数点的值*10

		for (int i = 0; i <= MAX_VALUE_SIZE * 12; i++)
		{
			
			double freq = pow(10, i*0.01 - 3);
			if (i == 0)		 freq = 0;
			double A_i = left_num[1] * freq - left_num[3] * pow(freq, 3),
				A_r = left_num[0] - left_num[2] * pow(freq, 2) + left_num[4] * pow(freq, 4),
				B_i = right_num[1] * freq - right_num[3] * pow(freq, 3) + right_num[5] * pow(freq, 5),
				B_r = right_num[0] - right_num[2] * pow(freq, 2) + right_num[4] * pow(freq, 4);
			double Ang_A = atan(A_i / A_r), Ang_B = atan(B_i / B_r);
			//将反正切函数的运算结果由两象限转换到四象限
			if (A_r < 0)
			{
				if (A_i >= 0)
				{
					Ang_A += phi;
				}
				else
				{
					Ang_A -= phi;
				}
			}
			if (B_r < 0)
			{
				if (B_i >= 0)
				{
					Ang_B += phi;
				}
				else
				{
					Ang_B -= phi;
				}
			}
			Ang_value[i] = (Ang_A - Ang_B) / phi * 180;
			Re[i] = (A_r * B_r + A_i * B_i) / (B_r * B_r + B_i * B_i);
			Im[i] = (A_i * B_r - A_r * B_i) / (B_r * B_r + B_i * B_i);

			if (max < abs(Re[i]))			max = abs(Re[i]);
			if (max < abs(Im[i]))			max = abs(Im[i]);
		}
		if (modf(max, &tmp) > 0)		max = tmp + 1;
		MemDC.SelectObject(pen_axis);
		//绘制y轴
		MemDC.MoveTo(290, 20);
		MemDC.LineTo(290, 540);
		for (int i = 0; i < 11; i++) 
		{
			if (i == 5) {}
			else 
			{
				CString tmp;
				tmp.Format(_T("%g"), max / 5 * (i - 5));
				MemDC.MoveTo(280, 20 + 52 * i);
				MemDC.LineTo(300, 20 + 52 * i);			
				MemDC.TextOutW(300, 20 + 52 * i, tmp);
			}
		}
		//绘制x轴
		MemDC.MoveTo(20, 280);
		MemDC.LineTo(560, 280);
		for (int i = 0; i < 11; i++)
		{
			CString tmp;
			MemDC.MoveTo(20 + 54 * i, 270);
			MemDC.LineTo(20 + 54 * i, 290);
			tmp.Format(_T("%g"), max / 5 * (i - 5));
			MemDC.TextOutW(20 + 54 * i, 285, tmp);
		}
		//绘制曲线
		double X = 290 + 270 * (Re[0] / max);
		double Y = 280 - 270 * (Im[0] / max);
		MemDC.SelectObject(pen_bode);
		MemDC.MoveTo(X, Y);
		MemDC.TextOutW(X + 5, Y - 20, L"f = 0");
		for (int i = 1; i <= MAX_VALUE_SIZE * 12; i++)
		{
			X = 290 + 270 * (Re[i] / max);
			Y = 280 - 260 * (Im[i] / max);
			MemDC.LineTo(X, Y);

			if ((Im[i] < 0 && Im[i - 1]>0) || (Im[i] > 0 && Im[i - 1] < 0))
			{
				CString _X;
				_X.Format(_T("%.4g"), (Re[i] + Re[i - 1]) / 2);
				MemDC.TextOutW(X - 40, Y - 30,_X);
			}
		}
		MemDC.TextOutW(X + 5, Y - 20, L"f = +∞");
	}
	
	pDC->BitBlt(0, 0, 1520, 1050, &MemDC, 0, 0, SRCCOPY);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

	comp_fun = false;
}

void Bode_plot::OnButtonComp()
{
	if (!plot_ex || comp_fun)
	{
		return;
	}
	comp_fun = true;
	CString Z;
	CDC *pDC = GetDC();					//用于绘制的设备上下文
	CRect rect;
	CPen pen_bode(PS_SOLID, 2, RGB(255, 0, 0));			//创建绘制波特图图像的画笔
	CPen pen[6];					//创建绘制对比图像的画笔
	bool AAA = false;
	int lable_C = 20;
	pen[0].CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	pen[1].CreatePen(PS_SOLID, 2, RGB(255, 128, 0));
	pen[2].CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	pen[4].CreatePen(PS_SOLID, 2, RGB(0, 255, 255));
	pen[5].CreatePen(PS_SOLID, 2, RGB(128, 128, 192));
	pen[3].CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	Z.Format(_T("%g"), zeta);		
	GetClientRect(&rect);
	pDC->SelectObject(pen_bode);
	//---------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------
	double dB_value[MAX_VALUE_SIZE * 14 + 1], Ang_value[MAX_VALUE_SIZE * 14 + 1];		//存放频率响应函数的计算结果
	//绘图时所取的各参数点,下一个周期的参数点为当前周期参数点的值*10
	double x_value[MAX_VALUE_SIZE], differ = (1.0 / MAX_VALUE_SIZE);

	for (int i = 0; i < MAX_VALUE_SIZE; i++) 			//10^(0:differ:1)
	{
		x_value[i] = pow(10, i*differ);
	}

	pInputZeta->GetWindowText(Z);
	Z.Remove(' ');
	if (Z.GetLength() <= 0)
	{
		return;
	}
	int mid = Z.Find(':');
	CString L, R;
	L = Z.Left(mid);
	R = Z.Mid(mid + 1);

	int length = L.GetLength(), i = 0;
	for (i; i < length; i++)
	{
		if (L.Mid(i, 1) == L'.')
		{
			break;
		}
		A *= 10;
		A += _ttoi(L.Mid(i, 1));
	}
	for (int j = length; j > i; j--)
	{
		A += _ttoi(L.Mid(j, 1)) * (1 / pow(10, j - i));
	}
	length = R.GetLength();
	i = 0;
	for (i; i < length; i++)
	{
		if (R.Mid(i, 1) == L'.')
		{
			break;
		}
		B *= 10;
		B += _ttoi(R.Mid(i, 1));
	}
	for (int j = length; j > i; j--)
	{
		B += _ttoi(R.Mid(j, 1)) * (1 / pow(10, j - i));
	}

	if (B <= A)			return;

	for (int j = 0; j < 6; j++) 
	{
		double Ze = A + (B - A) / 5 * j;
		if (Ze == zeta || (Ze > zeta && !AAA))
		{
			if (Ze == zeta)	 j++;
			AAA = true;
			Z.Format(_T("%g"), zeta);
			pDC->SelectObject(pen_bode);
			pDC->MoveTo(1310, lable_C);						
			pDC->LineTo(1350, lable_C);
			pDC->TextOutW(1370, lable_C - 10, L"zeta = " + Z);
			lable_C += 25;
			Ze = A + (B - A) / 5 * j;
		}
		if (Ze == 0) 
		{
			j++;
			Ze = A + (B - A) / 5 * j;
		}
		right_num[1] = 2 * Ze * sqrt(right_num[2]);
		for (int i = 0; i <= MAX_VALUE_SIZE * x_range; i++)
		{
			double freq = x_value[i % MAX_VALUE_SIZE] * pow(10, i / MAX_VALUE_SIZE - 5 + L_range);
			double B_i = right_num[1] * freq, B_r = 1 - right_num[2] * pow(freq, 2);
			dB_value[i] = - 10 * log10(pow(B_i, 2) + pow(B_r, 2));
			double Ang_B = atan(B_i / B_r);

			//将反正切函数的运算结果由两象限转换到四象限
			if (B_r < 0)
			{
				if (B_i >= 0)
				{
					Ang_B += phi;
				}
				else
				{
					Ang_B -= phi;
				}
			}
			Ang_value[i] = (atan(0) - Ang_B) / phi * 180;
		}

		int OrX = 50;
		double X, Y;
		//根据计算结果绘制波特图
		pDC->SelectObject(pen[j]);
		pDC->MoveTo(OrX, dB_y_axis - dB_value[0] / 20 * y_dec);
		for (int i = 0; i <= MAX_VALUE_SIZE * x_range; i++)
		{
			X = OrX + (log10(x_value[i % MAX_VALUE_SIZE]) * x_dec) + (i / MAX_VALUE_SIZE) * x_dec;
			Y = dB_y_axis - dB_value[i] / 20 * y_dec;
			pDC->LineTo(X, Y);
		}

		pDC->MoveTo(OrX, Ang_y_axis - Ang_value[0] / 45 * y_dec);
		for (int i = 1; i <= MAX_VALUE_SIZE * x_range; i++)
		{
			X = OrX + (log10(x_value[i % MAX_VALUE_SIZE]) * x_dec) + i / MAX_VALUE_SIZE * x_dec;
			Y = Ang_y_axis - Ang_value[i] / 45 * y_dec;
			pDC->LineTo(X, Y);
		}
		Z.Format(_T("%g"), Ze);
		pDC->MoveTo(1310, lable_C);						//注明颜色与阻尼比的对应关系
		pDC->LineTo(1350, lable_C);
		pDC->TextOutW(1370, lable_C - 10, L"zeta = " + Z);
		lable_C += 25;
	}
}

void Bode_plot::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDblClk(nFlags, point);
	if (!plot_ex)
	{
		return;
	}

	CDC *pDC = GetDC();							//用于绘制的设备上下文
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	POINT p;
	GetWindowRect(&rect);
	GetCursorPos(&p);
	double x = (p.x - rect.left) - 8, y = (p.y - rect.top);	//鼠标位置的坐标值
	if (x < 50 || x > 1280 || n > 14)
	{
		return;
	}

	CPen pen_dot(PS_DOT, 1, RGB(0, 0, 255));
	double _x = (x - 50) / (x_dec), freq, dB, Ang;
	freq = pow(10, _x - 5 + L_range);
	CString  C_freq, C_dB, C_Ang, C_lable;
	pDC->SelectObject(pen_dot);

	if (!comp_fun) 
	{	
		pDC->MoveTo(x, 50);
		pDC->LineTo(x, 1000);

		double A_i = left_num[1] * freq - left_num[3] * pow(freq, 3),
			A_r = left_num[0] - left_num[2] * pow(freq, 2) + left_num[4] * pow(freq, 4),
			B_i = right_num[1] * freq - right_num[3] * pow(freq, 3) + right_num[5] * pow(freq, 5),
			B_r = right_num[0] - right_num[2] * pow(freq, 2) + right_num[4] * pow(freq, 4);
		dB = 10 * log10(pow(A_i, 2) + pow(A_r, 2)) - 10 * log10(pow(B_i, 2) + pow(B_r, 2));
		double Ang_A = atan(A_i / A_r), Ang_B = atan(B_i / B_r);
		if (A_r < 0)
		{
			if (A_i >= 0)
			{
				Ang_A += phi;
			}
			else
			{
				Ang_A -= phi;
			}
		}
		if (B_r < 0)
		{
			if (B_i >= 0)
			{
				Ang_B += phi;
			}
			else
			{
				Ang_B -= phi;
			}
		}
		Ang = (Ang_A - Ang_B) / phi * 180;
		
		C_freq.Format(_T("%.4g"), freq);
		C_dB.Format(_T("%.4g"), dB);
		C_Ang.Format(_T("%.4g"), Ang);
		C_lable.Format(_T("%d"), n);
		pDC->TextOutW(x, y, L"<" + C_lable + L">");
		pDC->TextOutW(1280, lable, L"<" + C_lable + L">");
		pDC->TextOutW(1310, lable, L"频率：" + C_freq +L"Hz");
		pDC->TextOutW(1310, lable + 20, L"幅值：" + C_dB + L"dB");
		pDC->TextOutW(1310, lable + 40, L"相角：" + C_Ang + L"°");
		lable += 70;
		n++;		
	}
	else 
	{
		if (n > 2)			return;
		if (n == 1)			lable = 180;
		pDC->MoveTo(x, 50);
		pDC->LineTo(x, 1000);
		right_num[1] = 2 * zeta * sqrt(right_num[2]);
		double B_i = right_num[1] * freq, B_r = 1 - right_num[2] * pow(freq, 2);
		dB = -10 * log10(pow(B_i, 2) + pow(B_r, 2));
		double Ang_B = atan(B_i / B_r);

		//将反正切函数的运算结果由两象限转换到四象限
		if (B_r < 0)
		{
			if (B_i >= 0)
			{
				Ang_B += phi;
			}
			else
			{
				Ang_B -= phi;
			}
		}
		Ang = (atan(0) - Ang_B) / phi * 180;

		C_freq.Format(_T("%.4g"), freq);
		C_dB.Format(_T("%.4g"), dB);
		C_Ang.Format(_T("%.4g"), Ang);
		C_lable.Format(_T("%d"), n);
		pDC->TextOutW(x, y, L"<" + C_lable + L">");
		pDC->TextOutW(1280, lable, L"<" + C_lable + L">");
		pDC->TextOutW(1310, lable, L"频率：" + C_freq + L"Hz");
		C_freq.Format(_T("%.4g"), zeta);
		pDC->TextOutW(1310, lable + 20, L"阻尼比：" + C_freq);
		pDC->TextOutW(1310, lable + 40, L"幅值：" + C_dB + L"dB");
		pDC->TextOutW(1310, lable + 60, L"相角：" + C_Ang + L"°");
		lable += 85;
		for (int i = 0; i < 6; i++) 
		{
			double Ze = A + (B - A) / 5 * i;
			if (Ze == zeta || Ze == 0)
			{
				i++;
				Ze = A + (B - A) / 5 * i;
			}
			right_num[1] = 2 * Ze * sqrt(right_num[2]);
			B_i = right_num[1] * freq;
			B_r = 1 - right_num[2] * pow(freq, 2);
			dB = -10 * log10(pow(B_i, 2) + pow(B_r, 2));
			Ang_B = atan(B_i / B_r);
			if (B_r < 0)
			{
				if (B_i >= 0)
				{
					Ang_B += phi;
				}
				else
				{
					Ang_B -= phi;
				}
			}
			Ang = (atan(0) - Ang_B) / phi * 180;

			C_freq.Format(_T("%.4g"), Ze);
			C_dB.Format(_T("%.4g"), dB);
			C_Ang.Format(_T("%.4g"), Ang);
			pDC->TextOutW(1310, lable, L"阻尼比：" + C_freq);
			pDC->TextOutW(1310, lable + 20, L"幅值：" + C_dB + L"dB");
			pDC->TextOutW(1310, lable + 40, L"相角：" + C_Ang + L"°");
			lable += 65;
		}
		n++;
		lable += 10;
	}
}

void Bode_plot::OnClose()
{
	CDialogEx::OnClose();
	plot_ex = false;
	comp_fun = false;
	lable = 10;
	n = 1;
}
