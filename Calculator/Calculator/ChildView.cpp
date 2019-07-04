
// ChildView.cpp: CChildView 类的实现
//

#include "stdafx.h"
#include "ChildView.h"
#include "Bode_plot.h"
#include "Equation.h"
#include "math.h"
#include "time.h"
#include "windows.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUTTON_NUMBER 45
#define phi 3.14159265358979323846
// CChildView

head *E = new(head);					//声明公式计算链表E，申请存储空间
complex Memory;
child *Lbra_stack[9];

SYSTEMTIME sys;
CStdioFile userLog;

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_COMPLEX, &CChildView::OnComplex)					//复数计算器
	ON_COMMAND(ID_GRAPH, &CChildView::OnGraph)						//图形计算器

	ON_COMMAND(ID_BUTTON_0, &CChildView::OnButton0)					//数字键0
	ON_COMMAND(ID_BUTTON_1, &CChildView::OnButton1)					//数字键1
	ON_COMMAND(ID_BUTTON_2, &CChildView::OnButton2)					//数字键2
	ON_COMMAND(ID_BUTTON_3, &CChildView::OnButton3)					//数字键3
	ON_COMMAND(ID_BUTTON_4, &CChildView::OnButton4)					//数字键4
	ON_COMMAND(ID_BUTTON_5, &CChildView::OnButton5)					//数字键5
	ON_COMMAND(ID_BUTTON_6, &CChildView::OnButton6)					//数字键6
	ON_COMMAND(ID_BUTTON_7, &CChildView::OnButton7)					//数字键7
	ON_COMMAND(ID_BUTTON_8, &CChildView::OnButton8)					//数字键8
	ON_COMMAND(ID_BUTTON_9, &CChildView::OnButton9)					//数字键9
	ON_COMMAND(ID_BUTTON_point, &CChildView::OnButtonPoint)			//.	
	ON_COMMAND(ID_BUTTON_ADD, &CChildView::OnButtonAdd)				//＋
	ON_COMMAND(ID_BUTTON_MIN, &CChildView::OnButtonMin)				//-
	ON_COMMAND(ID_BUTTON_MUL, &CChildView::OnButtonMul)				//×
	ON_COMMAND(ID_BUTTON_DIV, &CChildView::OnButtonDiv)				//÷
	ON_COMMAND(ID_BUTTON_EQU, &CChildView::OnButtonEqu)				//=

	ON_COMMAND(ID_BUTTON_SIN, &CChildView::OnButtonSin)
	ON_COMMAND(ID_BUTTON_COS, &CChildView::OnButtonCos)
	ON_COMMAND(ID_BUTTON_TAN, &CChildView::OnButtonTan)
	ON_COMMAND(ID_BUTTON_ARCSIN, &CChildView::OnButtonArcsin)
	ON_COMMAND(ID_BUTTON_ARCCOS, &CChildView::OnButtonArccos)
	ON_COMMAND(ID_BUTTON_ARCTAN, &CChildView::OnButtonArctan)

	ON_COMMAND(ID_BUTTON_BS, &CChildView::OnButtonBs)				//退格
	ON_COMMAND(ID_BUTTON_C, &CChildView::OnButtonC)					//初始化
	ON_COMMAND(ID_BUTTON_CE, &CChildView::OnButtonCe)				//清空输入

	ON_COMMAND(ID_BUTTON_log, &CChildView::OnButtonLog)
	ON_COMMAND(ID_BUTTON_ln, &CChildView::OnButtonLn)
	ON_COMMAND(ID_BUTTON_EPOW, &CChildView::OnButtonEpow)
	ON_COMMAND(ID_BUTTON_10POW, &CChildView::OnButton10pow)
	ON_COMMAND(ID_BUTTON_EXTA, &CChildView::OnButtonExta)
	ON_COMMAND(ID_BUTTON_CONT, &CChildView::OnButtonCont)
	ON_COMMAND(ID_BUTTON_FACT, &CChildView::OnButtonFact)
	ON_COMMAND(ID_BUTTON_i, &CChildView::OnButtonI)
	ON_COMMAND(ID_BUTTON_θ, &CChildView::OnButtonθ)
	ON_COMMAND(ID_BUTTON_LBR, &CChildView::OnButtonLbr)
	ON_COMMAND(ID_BUTTON_RBR, &CChildView::OnButtonRbr)
	ON_COMMAND(ID_BUTTON_phi, &CChildView::OnButtonPhi)
	ON_COMMAND(ID_BUTTON_POW, &CChildView::OnButtonPow)
	ON_COMMAND(ID_BUTTON_RECI, &CChildView::OnButtonReci)
	ON_COMMAND(ID_BUTTON_POW_2, &CChildView::OnButtonPow2)
	ON_COMMAND(ID_BUTTON_ROOT, &CChildView::OnButtonRoot)
	ON_COMMAND(ID_BUTTON_MS, &CChildView::OnButtonMS)
	ON_COMMAND(ID_BUTTON_MR, &CChildView::OnButtonMR)
	ON_COMMAND(ID_BUTTON_MC, &CChildView::OnButtonMC)
	ON_COMMAND(ID_BUTTON_Switch, &CChildView::OnButtonSwitch)

	ON_COMMAND(ID_BUTTON_SOLVE, &CChildView::OnStart)
	ON_COMMAND(ID_BUTTON_BODE, &CChildView::OnButtonBode)			//生成波特图
	ON_COMMAND(ID_BUTTON_NYQU, &CChildView::OnButtonNyqu)			//生成Nyquist曲线
	ON_WM_CLOSE()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	GetLocalTime(&sys);
	CString Local_Day , Local_Time;
	char Time[32];
	sprintf_s(Time, "%4d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay);
	Local_Day = Time;
	userLog.Open(L"./" + Local_Day + L".txt", CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	sprintf_s(Time, "%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
	Local_Time = Time;
	userLog.SeekToEnd();
	userLog.WriteString(L"\n");
	userLog.WriteString(L"-------------------" + Local_Time + L"-------------------" + L"\n");
	
	return TRUE;
}

void CChildView::OnPaint() 
{
	CRect   rc;
	CPaintDC dc(this); // 用于绘制的设备上下文
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(225, 225, 225));		//对背景进行染色
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

CEdit *pInputBox = NULL, *pAnswerBox = NULL, *pBox_1 = NULL, *pBox_2 = NULL;
CStatic *pTitle = NULL;
CMFCButton *pBut[BUTTON_NUMBER] = { NULL }, *E_pBut = NULL, *G_pBut = NULL, *N_pBut = NULL;
LPCTSTR lable[BUTTON_NUMBER] = {	L"x²",		L"x^y",		L"sin",		L"cos",		L"tan",
									L"√",		L"y√x",	L"◇MS",	L"MR",		L"MC",
									L"¹switch",	L"CE",		L"C",		L"<--",		L"÷",
									L"π",		L"7",		L"8",		L"9",		L"×",
									L"n!",		L"4",		L"5",		L"6",		L"-",
									L"±",		L"1",		L"2",		L"3",		L"+",
									L"⁰(",		L")",		L"0",		L".",		L"=",
									L"i",		L"∠",		L"arcsin",	L"arccos",	L"arctan",
									L"1/x",		L"10^x",	L"log",		L"e^x",		L"ln",
								};
LPCTSTR Lbr_lable[10] = { L"⁰(", L"¹(", L"²(", L"³(", L"⁴(", L"⁵(", L"⁶(", L"⁷(", L"⁸(", L"⁹(" };
enum Program_state
{
	HOLD, COMPLEX, GRAPH
	//HOLD：软件主界面；COMPLEX：复数计算器界面；GRPHA：频率特性界面
}state;
CRect rect;
unsigned int x = 5, y = 205, tmp_x, tmp_y;
unsigned int page_number = 1;

void CChildView::OnComplex()
{
	Frm_Init();
	state = COMPLEX;
	Complex_Init();
}

void CChildView::OnGraph()
{
	Frm_Init();
	state = GRAPH;
	Graph_Init();
}

void CChildView::Frm_Init()
{
	GetClientRect(&rect);
	if (!pTitle)
	{
		pTitle = new(CStatic);
		pTitle->Create(L" ", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(5, 5, rect.right - 5, 25), this, ID_TITLE);
	}
	for (int i = 0; i < BUTTON_NUMBER; i++) {
		if (pBut[i] != NULL)
		{
			delete pBut[i];
			pBut[i] = NULL;
		}
	}
	if (pBox_1 != NULL)
	{
		delete pBox_1;
		pBox_1 = NULL;
	}
	if (pBox_2 != NULL)
	{
		delete pBox_2;
		pBox_2 = NULL;
	}
	if (pAnswerBox != NULL)
	{
		delete pAnswerBox;
		pAnswerBox = NULL;
	}
	if (pInputBox != NULL)
	{
		delete pInputBox;
		pInputBox = NULL;
	}
	if (G_pBut != NULL)
	{
		delete G_pBut;
		G_pBut = NULL;
	}
	if (N_pBut != NULL)
	{
		delete N_pBut;
		N_pBut = NULL;
	}
	if (E_pBut != NULL)
	{
		delete E_pBut;
		E_pBut = NULL;
	}
	E->start = new(child);				//对E内部进行初始化 
	E->start->next = E->start;
	child_value(E->start, 0, 0, '+');
	E->tail = E->start;
	state = HOLD;
}

void CChildView::Complex_Init()
{
	GetClientRect(&rect);
	pTitle->SetWindowTextW(L"复数计算器");
	if (pAnswerBox == NULL)
	{
		pAnswerBox = new(CEdit);
		pAnswerBox->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_RIGHT | ES_READONLY, CRect(5, 30, rect.right - 5, 90), this, ID_ANSWERBOX);
//		AnswerBox_exist = true;
	}
	if (pInputBox == NULL)
	{
		pInputBox = new(CEdit);
		pInputBox->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_RIGHT | ES_READONLY, CRect(5, 95, rect.right - 5, 200), this, ID_INPUTBOX);
//		InputBox_exist = true;
	}
	for (int i = 0; i < BUTTON_NUMBER - 10; i++)
	{
		if (pBut[i] == NULL) {
			tmp_x = x + 82 * (i % 5);
			tmp_y = y + 52 * (i / 5);
			pBut[i] = new CMFCButton;
			pBut[i]->Create(lable[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(tmp_x, tmp_y, tmp_x + 80, tmp_y + 50), this, ID_BUTTON_POW_2 + i);
			if (i == 16 || i == 17 || i == 18 || i == 21 || i == 22 || i == 23 || i == 26 || i == 27 || i == 28 || i == 32)		//对数字键染色
			{
				pBut[i]->SetFaceColor(RGB(128, 255, 255));		
			}
			else 
			{
				if (i >= 10) 
				{
					pBut[i]->SetFaceColor(RGB(245, 245, 245));
				}
			}
		}
	}
	comInit(&Memory, 0, 0);
}

void CChildView::Graph_Init()
{
	pTitle->SetWindowTextW(L"频率特性");
	GetClientRect(&rect);
	if (pInputBox == NULL)						//用于输入公式的文本框
	{
		pInputBox = new(CEdit);
		pInputBox->Create(WS_CHILD | WS_VISIBLE | ES_CENTER | ES_AUTOHSCROLL, CRect(30, 110, rect.right - 30, 140), this, ID_INPUTBOX);
		pInputBox->SetWindowTextW(L"[20]/[10,17,8,1]");	
		//	[1]/[1,2,1]  [0.01,1.01,1]  [1]/[1,0.5,1] [1]/[1,1]
		//	Nyquist [20]/[10,17,8,1]
	}
	if (pAnswerBox == NULL)
	{
		pAnswerBox = new(CEdit);
		pAnswerBox->Create(WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY | ES_MULTILINE, CRect(25, 250, rect.right - 25, 300), this, ID_ANSWERBOX);
	}
	if (pBox_1 == NULL)
	{
		pBox_1 = new(CEdit);
		pBox_1->Create(WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY, CRect(30, 80, rect.right - 30, 110), this, ID_BOX_1);
		pBox_1->SetWindowTextW(L"请输入公式");
	}
	if (pBox_2 == NULL)
	{
		pBox_2 = new(CEdit);
		pBox_2->Create(WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY | ES_MULTILINE, CRect(25, 160, rect.right - 25, 220), this, ID_BOX_2);
		pBox_2->SetWindowTextW(L"输入格式: 4s^2 + 2s / 5s^3 - 1 == [4,2,0] / [5,0,0,-1]     (波特图最大支持5阶,方程最大支持三阶)");
	}
	if (G_pBut == NULL)
	{
		G_pBut = new CMFCButton;
		G_pBut->Create(L"生成波特图", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(160, 350, 260, 400), this, ID_BUTTON_BODE);
	}
	if (N_pBut == NULL)
	{
		N_pBut = new CMFCButton;
		N_pBut->Create(L"生成生成Nyquist曲线", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(110, 405, 310, 455), this, ID_BUTTON_NYQU);
	}
	if (E_pBut == NULL)
	{
		E_pBut = new CMFCButton;
		E_pBut->Create(L"解方程", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(160, 460, 260, 510), this, ID_BUTTON_SOLVE);
	}
}

//——————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————————————————————————————————————————
//下方是按键响应函数和计算程序
constexpr auto const_e = 2.718281828;

void Invalid_input();
int fact(int n);											//阶乘的计算函数
double strTodou(CString input);								//将CSrting内容转换为double
complex strTocom(CString input);							//将CSrting内容转换为complex
CString comTostr(complex input);							//将complex内容转换为CSrting
void clearStr(CString *input);								//清除一个CString内的全部内容
void calculator_init(bool flag);							//再按下=后判断是否初始化计算过程
double root();
complex pow();
complex Sqrt(complex z);
complex Sqrt(double z);
complex Ln(complex z);
complex Epow(complex z);
complex Sin(complex z);
complex Cos(complex z);
int sgn(double N);

CString Input, Equation;		//存放输入的字符，存放已输入的算式
complex answer;					//算式计算的结果
bool point_flag = false, end_flag = false, i_flag = false, root_flag = false, pow_flag = false;
int complex_input = 0, Lbr_n = 0;								//在复数输入模式下判断当前是第几次输入加/减号;记录剩余的未配对"("数量

//常数
void CChildView::OnButton0()
{
	calculator_init(end_flag);
	Input += '0';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton1()
{
	calculator_init(end_flag);
	Input += '1';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton2()
{
	calculator_init(end_flag);
	Input += '2';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton3()
{
	calculator_init(end_flag);
	Input += '3';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton4()
{
	calculator_init(end_flag);
	Input += '4';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton5()
{
	calculator_init(end_flag);
	Input += '5';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton6()
{
	calculator_init(end_flag);
	Input += '6';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton7()
{
	calculator_init(end_flag);
	Input += '7';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton8()
{
	calculator_init(end_flag);
	Input += '8';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButton9()
{
	calculator_init(end_flag);
	Input += '9';
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButtonPoint()
{
	if (!point_flag) 
	{
		point_flag = true;
		Input += '.';
		pInputBox->SetWindowTextW(Input);	
	}
}

void CChildView::OnButtonPhi()
{
	calculator_init(end_flag);
	if (Input.GetLength() > 0)			//不能在有输入的情况下输入phi参数
	{
		return;
	}
	complex tmp;
	comInit(&tmp, phi, 0);
	E->tail->next = new(child);
	E->tail = E->tail->next;
	E->tail->C = tmp;
	E->tail->next = E->start;
	Input = L"π";
	Equation += L"π";
	clearStr(&Input);
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
	point_flag = false;
}

//基础算子
void CChildView::OnButtonAdd()
{
	if (Input.GetLength() <= 0)					//如果没有输入新数字则不能输入新的操作符
	{
		return;
	}
	complex tmp;		//保存由输入的字符串转换来的复数
	if (Input.Find(L'i') > 0 && complex_input == 0)
	{
		//代数输入第一次输入加/减号，在输入内添加符号
		Input += L" + ";
		pInputBox->SetWindowTextW(Input);
		complex_input = 1;
	}		//代数输入第二次输入加/减号（或为几何输入,或为实数输入），生成新的节点保存数据，后续操作与实数输入基本一致，不做叙述
	else 
	{
		if (root_flag) 
		{
			comInit(&tmp, root(), 0);
			root_flag = false;
		}
		else 
		{
			if (pow_flag)
			{
				tmp = pow();
				pow_flag = false;
			}
			else 
			{
				tmp = strTocom(Input);
			}
		}
		if (E->tail->oper == '+' || E->tail->oper == '-' || E->tail->oper == '*' || E->tail->oper == '/')
		{
			E->tail->next = new(child);
			E->tail = E->tail->next;
			E->tail->C = tmp;
			E->tail->next = E->start;
			E->tail->bra_n = Lbr_n;
			if (i_flag)
			{
				Equation += L'(' + Input + L')';
			}
			else
			{
				Equation += Input;
			}
			
			E->tail->oper = '+';
		}
		Equation += L" + ";
		pAnswerBox->SetWindowTextW(Equation);
		clearStr(&Input);
		complex_input = 0;			//将输入次数复原
		i_flag = false;				//结束复数输入模式
	}
	point_flag = false;
	end_flag = false;
}

void CChildView::OnButtonMin()
{
	if (Input.GetLength() <= 0)					//如果没有输入新数字则不能输入新的操作符
	{
		return;
	}
	complex tmp;		//保存由输入的字符串转换来的复数
	if (Input.Find(L'i') > 0 && complex_input == 0)
	{
		//代数输入第一次输入加/减号，在输入内添加符号
		Input += L" - ";
		pInputBox->SetWindowTextW(Input);
		complex_input = 1;
	}		//代数输入第二次输入加/减号（或者为几何输入），生成新的节点保存数据，后续操作与实数输入基本一致，不做叙述
	else 
	{
		if (root_flag)
		{
			comInit(&tmp, root(), 0);
			root_flag = false;
		}
		else
		{
			if (pow_flag)
			{
				tmp = pow();
				pow_flag = false;
			}
			else
			{
				tmp = strTocom(Input);
			}
		}
		if (E->tail->oper == '+' || E->tail->oper == '-' || E->tail->oper == '*' || E->tail->oper == '/')
		{
			E->tail->next = new(child);
			E->tail = E->tail->next;
			E->tail->C = tmp;
			E->tail->next = E->start;
			E->tail->bra_n = Lbr_n;
			if (i_flag)
			{
				Equation += L'(' + Input + L')';
			}
			else
			{
				Equation += Input;
			}
		}
		E->tail->oper = '-';
		Equation += L" - ";
		pAnswerBox->SetWindowTextW(Equation);
		clearStr(&Input);
		complex_input = 0;			//将输入次数复原
		i_flag = false;				//结束复数输入模式
	}
	point_flag = false;
	end_flag = false;
}

void CChildView::OnButtonMul()
{
	if (Input.GetLength() <= 0) 
	{
		return;
	}
	complex tmp;		//保存由输入的字符串转换来的复数
	//在复数模式内输入乘除号会直接结束复数输入模式
	if (root_flag)
	{
		comInit(&tmp, root(), 0);
		root_flag = false;
	}
	else
	{
		if (pow_flag)
		{
			tmp = pow();
			pow_flag = false;
		}
		else
		{
			tmp = strTocom(Input);
		}
	}
	if (E->tail->oper == '+' || E->tail->oper == '-' || E->tail->oper == '*' || E->tail->oper == '/')
	{
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		E->tail->bra_n = Lbr_n;
		if (i_flag)
		{
			Equation += L'(' + Input + L')';
		}
		else
		{
			Equation += Input;
		}
	}
	E->tail->oper = '*';
	Equation += L" * ";
	pAnswerBox->SetWindowTextW(Equation);
	clearStr(&Input);
	complex_input = 0;			//将输入次数复原
	i_flag = false;				//结束复数输入模式
	point_flag = false;
	end_flag = false;
}

void CChildView::OnButtonDiv()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	complex tmp;		//保存由输入的字符串转换来的复数
	//在复数模式内输入乘除号会直接结束复数输入模式
	if (root_flag)
	{
		comInit(&tmp, root(), 0);
		root_flag = false;
	}
	else
	{
		if (pow_flag)
		{
			tmp = pow();
			pow_flag = false;
		}
		else
		{
			tmp = strTocom(Input);
		}
	}
	if (E->tail->oper == '+' || E->tail->oper == '-' || E->tail->oper == '*' || E->tail->oper == '/')
	{
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		E->tail->bra_n = Lbr_n;
		if (i_flag)
		{
			Equation += L'(' + Input + L')';
		}
		else
		{
			Equation += Input;
		}
	}
	E->tail->oper = '/';
	Equation += L" ÷ ";
	pAnswerBox->SetWindowTextW(Equation);
	clearStr(&Input);
	complex_input = 0;			//将输入次数复原
	i_flag = false;				//结束复数输入模式
	point_flag = false;
	end_flag = false;
}

void CChildView::OnButtonEqu()
{
	if (Lbr_n > 0) 
	{
		return;
	}
	if (Input.GetLength() <= 0)
	{
		return;
	}
	complex tmp;
	if (root_flag)
	{
		comInit(&tmp, root(), 0);
		root_flag = false;
	}
	else
	{
		if (pow_flag)
		{
			tmp = pow();
			pow_flag = false;
		}
		else
		{
			tmp = strTocom(Input);
		}
	}
	if (E->tail->oper == '+' || E->tail->oper == '-' || E->tail->oper == '*' || E->tail->oper == '/')
	{
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		if (i_flag)
		{
			Equation += L'(' + Input + L')';
		}
		else
		{
			Equation += Input;
		}
	}
	E->tail->oper = 'E';
	Equation += L" = ";
	calc(E);
	CString real, imag;
	real.Format(_T("%g"), E->result.real);
	Input = real;
	if (E->result.imag != 0) 
	{
		imag.Format(_T("%g"), E->result.imag);
		Input += L" + " + imag + L'i';
	}
	Equation += Input;
	pAnswerBox->SetWindowTextW(Equation);
	pInputBox->SetWindowTextW(Input);
	userLog.WriteString(Equation + "\n");
	clearStr(&Equation);
	end_flag = true;
	i_flag = false;				//结束复数输入模式
}

//特殊算子
void CChildView::OnButtonI() 
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	i_flag = true;
	Input += L"i";
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButtonθ()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	i_flag = true;
	Input += L"∠";
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButtonCont()			//相反数
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag) { return; }
	CString tmp = Input;
	if (tmp.Mid(0, 1) == L'-') 
	{
		tmp.Delete(0, 1);
		Input = tmp;
	}
	else 
	{
		Input = L'-' + tmp;
	}
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButtonReci()			//倒数
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag) { return; }
	if (strTodou(Input) == 0) 
	{		Invalid_input();	}
	else 
	{
		Input.Format(_T("%g"),1 / strTodou(Input) );
		pInputBox->SetWindowTextW(Input);	
	}
}

void CChildView::OnButtonFact()			//阶乘
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag) { return; }
	double n = strTodou(Input), tmp;		//调用fact函数计算阶乘，结果存入tmp	
	if (!n)		//n=0时，报错 
	{		Invalid_input();	}
	else 
	{
		if (modf(n, &tmp) != 0)			//n为小数时，报错
		{		Invalid_input();	}
		else 
		{
			int output = fact(n);
			E->tail->next = new(child);			//申请新的节点，将计算结果存入。但不更改操作符
			E->tail = E->tail->next;			
			E->tail->C.real = output;
			E->tail->next = E->start;
			Equation += Input + L'!';			//input为之前输入的数字，存入公式
			Input.Format(_T("%d"), output);		//将阶乘的结果转为字符串，显示在输入框中
			pInputBox->SetWindowTextW(Input);
			pAnswerBox->SetWindowTextW(Equation);	
		}	
	}
}

//左右括号

void CChildView::OnButtonLbr()
{
	if (Lbr_n > 8) 
	{
		return;
	}
	Equation += L'(';
	pAnswerBox->SetWindowTextW(Equation);
	Lbra_stack[Lbr_n] = E->tail;
	Lbr_n++;
	SetDlgItemTextW(ID_BUTTON_LBR, Lbr_lable[Lbr_n]);
}

void CChildView::OnButtonRbr()
{
	
	if (Lbr_n == 0)
	{
		return;									//不能再没有输入"("的情况下输入")"
	}
	if (Input.GetLength() <= 0)					//如果没有输入新数字则不能输入新的操作符
	{
		return;
	}
	complex tmp;
	if (root_flag)
	{
		comInit(&tmp, root(), 0);
		root_flag = false;
	}
	else
	{
		if (pow_flag)
		{
			tmp = pow();
			pow_flag = false;
		}
		else
		{
			tmp = strTocom(Input);
		}
	}
	if (E->tail->oper == '+' || E->tail->oper == '-' || E->tail->oper == '*' || E->tail->oper == '/')
	{
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		E->tail->bra_n = -Lbr_n;
		if (i_flag)
		{
			Equation += L'(' + Input + L')';
		}
		else
		{
			Equation += Input;
		}
	}
	calc_bra(Lbra_stack[Lbr_n - 1], E);
	Lbr_n--;
	Input = L" ";
	Equation += L')';
	pAnswerBox->SetWindowTextW(Equation);
	SetDlgItemTextW(ID_BUTTON_LBR, Lbr_lable[Lbr_n]);
}

//指数运算和幂运算
void CChildView::OnButtonPow()					//幂函数
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag) 
	{ 
		Input = L"(" + Input + L")" + L"^";
	}
	else 
	{
		Input += L" ^ ";
	}
	pow_flag = true;
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButtonPow2()					//平方
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)				//输入为实数
	{
		double tmp = strTodou(Input);
		tmp *= tmp;
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += Input + L"^2";			//input为之前输入的数字，存入公式
		Input.Format(_T("%g"), tmp);				//将平方的结果转为字符串，显示在输入框中
	}
	else 
	{
		complex tmp = strTocom(Input);
		tmp = complexM(tmp, tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		Equation += L'(' + Input + L")^2";			//input为之前输入的数字，存入公式
		Input = comTostr(tmp);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonRoot()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag) { return; }
	Input += L" √ ";
	root_flag = true;
	pInputBox->SetWindowTextW(Input);
}

void CChildView::OnButtonExta()					//开平方
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)
	{
		double tmp = strTodou(Input);
		tmp = sqrt(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L'√' + Input;			//input为之前输入的数字，存入公式
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	}
	else 
	{
		complex tmp = Sqrt(strTocom(Input));
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		Equation += L"√(" + Input + L')';		//input为之前输入的数字，存入公式
		Input = comTostr(tmp);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButton10pow()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag) { return; }
	double tmp = strTodou(Input);
	tmp = pow(10, tmp);
	E->tail->next = new(child);
	E->tail = E->tail->next;
	E->tail->C.real = tmp;
	E->tail->next = E->start;
	Equation += L"10^"+ Input;			//input为之前输入的数字，存入公式
	Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonEpow()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag)
	{
		double tmp = strTodou(Input);
		tmp = pow(const_e, tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"e^" + Input;			//input为之前输入的数字，存入公式
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	}
	else 
	{
		complex tmp = Epow(strTocom(Input));
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		Equation += L"e^(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(tmp);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonLog()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag) { return; }
	double tmp = strTodou(Input);
	tmp = log10(tmp);
	E->tail->next = new(child);
	E->tail = E->tail->next;
	E->tail->C.real = tmp;
	E->tail->next = E->start;
	Equation += L"log" + Input;			//input为之前输入的数字，存入公式
	Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonLn()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (i_flag)
	{
		double tmp = strTodou(Input);
		tmp = log(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"ln" + Input;			//input为之前输入的数字，存入公式
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	}
	else 
	{
		complex tmp = Ln(strTocom(Input));
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		Equation += L"Ln(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(tmp);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

//三角函数
void CChildView::OnButtonSin()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)
	{
		double tmp = strTodou(Input);
		tmp = sin(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"sin(" + Input + L')';			//input为之前输入的数字，存入公式
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	}
	else 
	{
		complex tmp = Sin(strTocom(Input));
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		Equation += L"Sin(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(tmp);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonCos()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)
	{
		double tmp = strTodou(Input);
		tmp = cos(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"cos(" + Input + L')';			//input为之前输入的数字，存入公式
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	}
	else
	{
		complex tmp = Cos(strTocom(Input));
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = tmp;
		E->tail->next = E->start;
		Equation += L"Cos(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(tmp);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonTan()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)
	{
		double tmp = strTodou(Input);
		tmp = tan(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"tan(" + Input + L')';			//input为之前输入的数字，存入公式
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
	}
	else
	{
		complex tmp = strTocom(Input), result = complexD(Sin(tmp), Cos(tmp));
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = result;
		E->tail->next = E->start;
		Equation += L"Tan(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(result);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonArcsin()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)
	{
		double tmp = strTodou(Input);
		tmp = asin(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"arcsin(" + Input + L')';			//input为之前输入的数字，存入公式
		tmp /= phi;
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
		Input += L"π";
	}
	else
	{
		complex tmp = strTocom(Input), result;
		result = Ln(complexA(tmp, Sqrt(complexI(complexM(tmp, tmp), 1))));
		comInit(&result, result.imag, -result.real);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = result;
		E->tail->next = E->start;
		Equation += L"Arcsin(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(result);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonArccos()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)
	{
		double tmp = strTodou(Input);
		tmp = acos(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"arccos(" + Input + L')';			//input为之前输入的数字，存入公式
		tmp /= phi;
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
		Input += L"π";
	}
	else
	{
		complex tmp = strTocom(Input), result, i;
		comInit(&i, 0, 1);
		result = Ln(complexA(complexM(i, tmp), Sqrt(complexI(1, complexM(tmp, tmp)))));
		comInit(&result, result.imag, -result.real);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = result;
		E->tail->next = E->start;
		Equation += L"Arccos(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(result);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

void CChildView::OnButtonArctan()
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	if (!i_flag)
	{
		double tmp = strTodou(Input);
		tmp = atan(tmp);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C.real = tmp;
		E->tail->next = E->start;
		Equation += L"arctan(" + Input + L')';			//input为之前输入的数字，存入公式
		tmp /= phi;
		Input.Format(_T("%g"), tmp);				//将结果转为字符串，显示在输入框中
		Input += L"π";
	}
	else
	{
		complex tmp = strTocom(Input), result;
		result = Ln(complexD(complexA(tmp, 1), complexI(1, tmp)));
		comInit(&result, -result.imag / 2, result.real / 2);
		E->tail->next = new(child);
		E->tail = E->tail->next;
		E->tail->C = result;
		E->tail->next = E->start;
		Equation += L"Arctan(" + Input + L')';			//input为之前输入的数字，存入公式
		Input = comTostr(result);
	}
	pInputBox->SetWindowTextW(Input);
	pAnswerBox->SetWindowTextW(Equation);
}

//算式修改
void CChildView::OnButtonBs()
{
	USES_CONVERSION;
	int tmp = Input.GetLength();			//获得字符串的长度
	char* p = T2A(Input.Mid(tmp - 1, 1).GetBuffer(0));
	Input.Mid(tmp - 1, 1).ReleaseBuffer();
	switch (*p)
	{
	case 'i':
		i_flag = false;
		break;
	case '∠':
		i_flag = false;
		break;
	case '+':
	case '-':
		complex_input = 0;
		break;
	case '.':
		point_flag = false;
		break;
	case '(':
		Lbr_n--;
		break;
	default:
			break;
	}
	Input.Delete(tmp - 1, 1);				//从字符串的最后一个位置删除一个字符
	pInputBox->SetWindowTextW(Input);		//更新文本框
}

void CChildView::OnButtonC()
{
	calculator_init(true);
	complex_input = 0;
}

void CChildView::OnButtonCe()
{
	clearStr(&Input);						//清空输入
	pInputBox->SetWindowTextW(Input);		//更新文本框
	complex_input = 0;
	point_flag = false;
	i_flag = false;
}

void CChildView::OnButtonMS()				
{
	if (Input.GetLength() <= 0)
	{
		return;
	}
	else 
	{
		SetDlgItemTextW(ID_BUTTON_MS, LPCTSTR(L"◆MS"));
		Memory = strTocom(Input);
	}
}

void CChildView::OnButtonMR()			
{
	if (Memory.real == 0 && Memory.imag == 0) 
	{
		return;
	}
	else 
	{
		Input = comTostr(Memory);
	}
}

void CChildView::OnButtonMC()				
{
	SetDlgItemTextW(ID_BUTTON_MS, LPCTSTR(L"◇MS"));
	comInit(&Memory, 0, 0);
}

void CChildView::OnButtonSwitch()				
{
	int n, m;
	switch (page_number) 
	{
	case 1:
		n = 0;
		m = 35;
		page_number = 2;
		SetDlgItemTextW(ID_BUTTON_Switch, LPCTSTR(L"²switch"));
		break;
	case 2:
		n = 35;
		m = 0;
		page_number = 1;
		SetDlgItemTextW(ID_BUTTON_Switch, LPCTSTR(L"¹switch"));
		break;
	default:
		break;
	}
	for (int i = 0 + n; i < 10 + n; i++)
	{
		delete pBut[i];
		pBut[i] = NULL;
	}
	for (int i = 35 - n; i < 45 - n; i++)
	{
		tmp_x = x + 82 * ((i - m) % 5);
		tmp_y = y + 52 * ((i - m) / 5);
		pBut[i] = new CMFCButton;
		pBut[i]->Create(lable[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(tmp_x, tmp_y, tmp_x + 80, tmp_y + 50), this, ID_BUTTON_POW_2 + i);
		pBut[i]->SetFaceColor(RGB(222, 222, 222));
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------
//11x^4 + 5s^2 + 0.5x - 1 == [11,0,5,0.5,-1]
void CChildView::OnStart()
{
	CString A;
	pInputBox->GetWindowTextW(A);
	if (A.GetLength() <= 0)
	{
		userLog.WriteString(L"Wrong Input \n");
		return;
	}
	userLog.WriteString(A + "\n");
	if (A.Find(L"/")) 
	{
		CString tmp = L"Wrong Input";
		pAnswerBox->SetWindowTextW(tmp);
	}
	CString X_1, X_2, X_3, X_4;
	CString R, I;
	double para[5] = { 0 };
	int ip, n = 0;
	A.Remove('[');		 A.Remove(']');			A.Remove(' ');
	do {
		ip = A.ReverseFind(',');					//首先找到数字之间分隔符号的位置(倒序查找)
		para[n] = strTodou(A.Mid(ip + 1));			//之后将数字存入数组，数组的标号对应着s的幂次
		A.Delete(ip, A.GetLength() - ip + 1);		//最后从字符串中删除数字和分隔符
		n++;
	} while (ip > 0);
	if (para[4] != 0)		
	{
		//一元四次方程，简化法
		double a = para[4], b = para[3], c = para[2], d = para[1], e = para[0];
		double D = 3 * b - 8 * a*c, E = -b + 4 * a*b*c - 8 * a*d,		 //重根判别式
			F = 3 * b + 16 * a*c - 16 * a*b*c + 16 * a*b*d - 64 * a*e,
			A = D - 3 * F, B = D * F - 9 * E, C = F - 3 * D*E;
		double dert = pow(B, 2) - 4 * A*C;								//总判别式
		if (D == 0 && E == 0 && F == 0)
		{
			X_1.Format(_T("%.4g"), -b / (4 * a));
			Input = L"x_1 = x_2 = x_3 = x_4 = " + X_1;
		}
		if (A == 0 && B == 0 && C == 0 && D*E*F != 0)
		{
			X_1.Format(_T("%.4g"), (-b * D + 9 * E) / (4 * a*D));
			X_2.Format(_T("%.4g"), (-b * D - 3 * E) / (4 * a*D));
			Input = L"x_1 = " + X_1 + L"    x_2 = x_3 = x_4 = " + X_2;
		}
		if (D > 0 && E == 0 && F == 0)
		{
			X_1.Format(_T("%.4g"), (-b + sqrt(D)) / (4 * a));
			X_2.Format(_T("%.4g"), (-b - sqrt(D)) / (4 * a));
			Input = L"x_1 = x_2 = " + X_1 + L"     x_3 = x_4 = " + X_2;
		}
		if (D < 0 && E == 0 && F == 0)
		{

		}
		if (A*B*C != 0 && dert == 0 && A*B > 0)
		{
			X_1.Format(_T("%.4g"), (-b + sgn(A*B*E)*sqrt(D - B / A) + sqrt(2 * B / A)) / (4 * a));
			X_2.Format(_T("%.4g"), (-b + sgn(A*B*E)*sqrt(D - B / A) - sqrt(2 * B / A)) / (4 * a));
			X_3.Format(_T("%.4g"), (-b - sgn(A*B*E)*sqrt(D - B / A)) / (4 * a));
			Input = L"x_1 = " + X_1 + L"     x_2 = " + X_2 + L"     x_3 = x_4 = " + X_3;
		}
		if (A*B*C != 0 && dert == 0 && A*B < 0)
		{

		}
		if (dert > 0)
		{
			double z_1 = A * D + 3 * (-B + sqrt(dert)) / 2,
				z_2 = A * D + 3 * (-B - sqrt(dert)) / 2,
				k = pow(z_1, 1 / 3) + pow(z_2, 1 / 3),
				z = pow(D, 2) - D * k + pow(k, 2) - 3 * A;
			X_1.Format(_T("%.4g"), (-b + sgn(E)*sqrt((D + k) / 3) + sqrt((2 * D - k + 2 * sqrt(z)) / 3)) / (4 * a));
			X_2.Format(_T("%.4g"), (-b + sgn(E)*sqrt((D + k) / 3) - sqrt((2 * D - k + 2 * sqrt(z)) / 3)) / (4 * a));
			X_3.Format(_T("%.4g"), (-b - sgn(E)*sqrt((D + k) / 3)) / (4 * a));
			X_4.Format(_T("%.4g"), sqrt((-2 * D + k + 2 * sqrt(z)) / 3) / (4 * a));				//虚部
			Input = L"x_1 = " + X_1 + L"     x_2 = " + X_2;
			Input += L"     x_3 = " + X_3 + L"+ i(" + X_4 + L")  x_4 = " + X_3 + L"- i(" + X_4 + L")";
		}
		else
		{
			double th = acos((3 * B - 2 * A*D) / 2 * A*sqrt(A)), y_1 = (D - 2 * sqrt(A)*cos(th / 3)) / 3,
				y_2 = (D + sqrt(A)*(cos(th / 3) + sqrt(3)*sin(th / 3))),
				y_3 = (D + sqrt(A)*(cos(th / 3) - sqrt(3)*sin(th / 3)));
			if (E == 0 && F > 0)
			{
				X_1.Format(_T("%.4g"), -(b / (4 * a)) + sqrt(abs(D) + 2 * sqrt(F)) / (4 * a));
				X_2.Format(_T("%.4g"), -(b / (4 * a)) - sqrt(abs(D) + 2 * sqrt(F)) / (4 * a));
				X_3.Format(_T("%.4g"), -(b / (4 * a)) + sqrt(abs(D) - 2 * sqrt(F)) / (4 * a));
				X_4.Format(_T("%.4g"), -(b / (4 * a)) - sqrt(abs(D) - 2 * sqrt(F)) / (4 * a));
				Input = L"x_1 = " + X_1 + L"     x_2 = " + X_2 + L"     x_3 = " + X_3 + L"     x_4 = " + X_4;
			}
			if (E == 0 && F < 0)
			{
				X_1.Format(_T("%.4g"), (-2 * b + sqrt(2 * D + 2 * sqrt(A - F))) / (8 * a));
				X_2.Format(_T("%.4g"), sqrt(-2 * D + 2 * sqrt(A - F)) / (8 * a));		//虚部
				X_3.Format(_T("%.4g"), (-2 * b - sqrt(2 * D + 2 * sqrt(A - F))) / (8 * a));
				X_4.Format(_T("%.4g"), sqrt(-2 * D + 2 * sqrt(A - F)) / (8 * a));		//虚部
				Input = L"x_1 = " + X_1 + L"+ i(" + X_2 + L")  x_2 = " + X_1 + L"- i(" + X_2;
				Input += L")  x_3 = " + X_3 + L"+ i(" + X_4 + L")  x_4 = " + X_3 + L"- i(" + X_4 + L")";
			}
			if (E != 0 && D > 0 && F > 0)
			{
				X_1.Format(_T("%.4g"), (-b + sgn(E)*sqrt(y_1) + (sqrt(y_2) + sqrt(y_3))) / (4 * a));
				X_2.Format(_T("%.4g"), (-b + sgn(E)*sqrt(y_1) - (sqrt(y_2) + sqrt(y_3))) / (4 * a));
				X_3.Format(_T("%.4g"), (-b - sgn(E)*sqrt(y_1) + (sqrt(y_2) - sqrt(y_3))) / (4 * a));
				X_4.Format(_T("%.4g"), (-b - sgn(E)*sqrt(y_1) - (sqrt(y_2) - sqrt(y_3))) / (4 * a));
				Input = L"x_1 = " + X_1 + L"     x_2 = " + X_2 + L"     x_3 = " + X_3 + L"     x_4 = " + X_4;
			}
			if (E != 0 && (D < 0 || F < 0))
			{
				X_1.Format(_T("%.4g"), (-b - sqrt(y_2)) / (4 * a));
				X_2.Format(_T("%.4g"), (sgn(E)*sqrt(-y_1) + sqrt(-y_3)) / (4 * a));		//虚部
				X_3.Format(_T("%.4g"), (-b + sqrt(y_2)) / (4 * a));
				X_4.Format(_T("%.4g"), (sgn(E)*sqrt(-y_1) - sqrt(-y_3)) / (4 * a));		//虚部
				Input = L"x_1 = " + X_1 + L"+ i(" + X_2 + L")  x_2 = " + X_1 + L"- i(" + X_2;
				Input += L")  x_3 = " + X_3 + L"+ i(" + X_4 + L")  x_4 = " + X_3 + L"- i(" + X_4 + L")";
			}
		}
	}
	else 
	{
		if(para[3] != 0) 
		{
			//一元三次函数
			double a = para[0] / para[3], b = para[1] / para[3], c = para[2] / para[3];
			double m = 36 * (a*b) - 8 * pow(a, 3) - 108 * c, dert = m * m - pow(4 * (a*a) - 12 * b, 3);
			if (dert >= 0)
			{
				double n = sqrt(dert), p = pow(m + n, 1 / 3), q;
				if (p == 0)
				{
					q = pow(m - n, 1 / 3);
				}
				else
				{
					q = (4 * (a*a) - 12 * b) / p;
				}
				X_1.Format(_T("%g"), (p + q - 2 * a) / 6);
				R.Format(_T("%g"), -(p + q + 4 * a) / 12);
				I.Format(_T("%g"), sqrt(3)*(p - q) / 12);
				X_2 = R + L"+ i" + I;
				X_3 = R + L"- i" + I;
				Input = L"x_1 = " + X_1 + L"  x_2 = " + X_2 + L"  x_3 = " + X_3;
			}
			else
			{
				Input = L"无实数解";
			}
		}
		else 
		{
			//一元二次函数 
			double a = para[2], b = para[1], c = para[0], dert = b * b - 4 * (a * c);
			if (dert >= 0)
			{

				X_1.Format(_T("%g"), (-b + sqrt(dert)) / (2 * a));
				X_2.Format(_T("%g"), (-b - sqrt(dert)) / (2 * a));
				Input = L"x_1 = " + X_1 + L"    x_2 = " + X_2;
				if (dert == 0)
				{
					Input = L"x_1 = x_2 =  " + X_2;
				}
			}
			else
			{
				complex z = Sqrt(dert);
				X_1.Format(_T("%g"), (-b + z.real) / (2 * a));
				X_2.Format(_T("%g"), (-b - z.real) / (2 * a));
				I.Format(_T("%g"), z.imag / (2 * a));
				Input = L"x_1 = " + X_1 + L"+ i(" + I + L")  x_2 = " + X_1 + L"- i(" + I + L")";
			}
		}
	}
	pAnswerBox->SetWindowTextW(Input);
}

//-------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------
void CChildView::OnButtonBode()
{
	pInputBox->GetWindowText(Equation);
	Equation.Remove('[');		 Equation.Remove(']');			Equation.Remove(' ');
	if (Equation.GetLength() <= 0)
	{
		userLog.WriteString(L"Wrong Input \n");
		return;
	}
	userLog.WriteString(Equation + L"  Bode plot " + "\n");
	double left_num[5] = { 0 }, right_num[6] = { 0 };

	int mid = Equation.Find('/'), ip, n = 0;
	CString Left = Equation.Left(mid), Right = Equation.Mid(mid + 1), tmp;
	tmp = Left;	
	do{		
		ip = tmp.ReverseFind(',');					//首先找到数字之间分隔符号的位置(倒序查找)
		left_num[n] = strTodou(tmp.Mid(ip + 1));	//之后将数字存入数组，数组的标号对应着s的幂次
		tmp.Delete(ip, tmp.GetLength() - ip + 1);	//最后从字符串中删除数字和分隔符
		n++;
	}	while (ip > 0);
	n = 0;
	tmp = Right;
	do	{
		ip = tmp.ReverseFind(',');						//首先找到数字之间分隔符号的位置
		right_num[n] = strTodou(tmp.Mid(ip + 1));		//之后将数字存入数组
		tmp.Delete(ip, tmp.GetLength() - ip + 1);		//最后从字符串中删除数字和分隔符
		n++;
	}	while (ip > 0);

	Bode_plot *Bode = new Bode_plot;
	memcpy(Bode->left_num, left_num, sizeof(left_num));
	memcpy(Bode->right_num, right_num, sizeof(right_num));
	Bode->Create(IDD_DIALOG1, this);
	Bode->ShowWindow(SW_SHOW);
	Bode->Function = TRUE;
}

void CChildView::OnButtonNyqu()
{
	pInputBox->GetWindowText(Equation);
	Equation.Remove('[');		 Equation.Remove(']');			Equation.Remove(' ');
	if (Equation.GetLength() <= 0)
	{
		userLog.WriteString(L"Wrong Input \n");
		return;
	}
	userLog.WriteString(Equation + L"  Nyquist " + "\n");
	double left_num[5] = { 0 }, right_num[6] = { 0 };

	int mid = Equation.Find('/'), ip, n = 0;
	CString Left = Equation.Left(mid), Right = Equation.Mid(mid + 1), tmp;
	tmp = Left;
	do {
		ip = tmp.ReverseFind(',');					//首先找到数字之间分隔符号的位置(倒序查找)
		left_num[n] = strTodou(tmp.Mid(ip + 1));	//之后将数字存入数组，数组的标号对应着s的幂次
		tmp.Delete(ip, tmp.GetLength() - ip + 1);	//最后从字符串中删除数字和分隔符
		n++;
	} while (ip > 0);
	n = 0;
	tmp = Right;
	do {
		ip = tmp.ReverseFind(',');						//首先找到数字之间分隔符号的位置
		right_num[n] = strTodou(tmp.Mid(ip + 1));		//之后将数字存入数组
		tmp.Delete(ip, tmp.GetLength() - ip + 1);		//最后从字符串中删除数字和分隔符
		n++;
	} while (ip > 0);

	Bode_plot *Nyqu = new Bode_plot;
	memcpy(Nyqu->left_num, left_num, sizeof(left_num));
	memcpy(Nyqu->right_num, right_num, sizeof(right_num));
	Nyqu->Create(IDD_DIALOG1, this);
	Nyqu->ShowWindow(SW_SHOW);
	Nyqu->Function = FALSE;
}


//-------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------

void calculator_init(bool flag)				//算式运算结束后的初始化程序
{		//按下等号后如果按下运算符则继续计算，按下数字则清空计算历史
	if (flag) 
	{
		struct child *P = E->start;
		child_value(P, 0, 0, '+');
		while (P->next != E->start)
		{
			After(P);
		}
		E->tail = P;
		clearStr(&Input);
		clearStr(&Equation);
		pInputBox->SetWindowTextW(Input);
		pAnswerBox->SetWindowTextW(Equation);
		Lbr_n = 0;
		end_flag = false;
		point_flag = false;
		i_flag = false;
	}
}

complex strTocom(CString input)			//从CString到complex的转换函数
{										//字符串中复数的输入结构为xi +/- y，其中i的位置为ip，字符串的长度为length
	complex answer;
	CString tmp = input;	tmp.Remove(' ');	tmp.Remove('(');	tmp.Remove(')');	//删除掉字符串中的全部多余字符
	if (Input.Find(L'i') > 0)
	{
		int length = tmp.GetLength(), ip = tmp.Find(L'i');
		answer.imag = strTodou(tmp.Left(ip));			//提取虚部
		if (length == ip + 1)				//如果最后一个字符为'i'，即实部留空
		{
			answer.real = 0;
		}
		else
		{
			answer.real = strTodou(tmp.Mid(ip + 1));
		}
	}
	else 
	{
		if (tmp.Find(L'∠') > 0) 
		{
			int length = tmp.GetLength(), ip = tmp.Find(L'∠');
			double r, θ;
			r = strTodou(tmp.Left(ip));
			θ = strTodou(tmp.Mid(ip + 1));
			comInit(&answer, r * cos(θ), r * sin(θ));
		}
		else 
		{
			comInit(&answer, strTodou(tmp), 0);
		}
	}
	return answer;
}

double strTodou(CString input)			//从CString到double的转换函数
{
	input.Remove('+');
	input.Remove(' ');
	bool minus_flag = false;
	int length = input.GetLength(), i = 0;
	double answer = 0;
	for (i; i < length; i++)
	{
		if (input.Find(L'-') >= 0)
		{
			i++;
			minus_flag = true;
		}
		if (input.Mid(i, 1) == L'.')
		{
			break;
		}
		answer *= 10;
		answer += _ttoi(input.Mid(i, 1));
	}
	for (int j = length; j > i; j--)
	{
		answer += _ttoi(input.Mid(j, 1)) * (1 / pow(10, j - i));
	}
	if (minus_flag)
	{
		answer = -answer;
	}
	return answer;
}

CString comTostr(complex input)
{
	CString real, imag;
	real.Format(_T("%g"), input.real);
	imag.Format(_T("%g"), input.imag);
	if (input.imag == 0)
	{
		return real;
	}
	if (input.real > 0) 
	{
		imag += L"i + " + real;
	}
	else 
	{
		if (input.real == 0) 
		{
			imag += L'i';
		}
		else 
		{
			real.Delete(0, 1);
			imag += L"i - " + real;
		}
	}
	return imag;
}

void clearStr(CString *input)			//清除一个字符串内的全部内容
{
	input->Delete(0, input->GetLength());
}

double root() 
{
	CString tmp = Input;
	int length = tmp.GetLength(), ip = tmp.Find(L'√');
	double y = strTodou(tmp.Left(ip)),	
	x = strTodou(tmp.Mid(ip + 1));
	return pow(x, 1 / y);
}

complex pow() 
{
	CString tmp = Input;
	complex x, result;
	double y;
	int length = tmp.GetLength(), ip = tmp.Find(L"^");
	x = strTocom(tmp.Left(ip));
	result = x;
	y = strTodou(tmp.Mid(ip + 1));
	for (int i = 1; i < y; i++) 
	{
		result = complexM(result, x);
	}
	if (y = 0) 
	{
		if (result.imag != 0) {
			comInit(&result, 1, 1);
		}
		else 
		{
			comInit(&result, 1, 0);
		}
	}
	return result;
}

int fact(int n) 
{	
	int tmp = 1;
	if (n < 0) { return 0; }
	if (n == 0) { return 1; }
	while (n > 0)	
	{
		tmp *= n;
		n--;
	}
	return tmp;
}

complex Sqrt(complex z) 
{
	double real = z.real, imag = z.imag;
	double r = sqrt(real * real + imag * imag), θ = atan(imag / real);
	complex result;
	result.real = sqrt(r)*cos(θ / 2);
	result.imag = sqrt(r)*sin(θ / 2);
	return result;
}

complex Sqrt(double z)
{
	double real = z, imag = 0;
	double r = sqrt(real * real + imag * imag), θ = atan(imag / real);
	complex result;
	result.real = sqrt(r)*cos(θ / 2);
	result.imag = sqrt(r)*sin(θ / 2);
	return result;
}

complex Ln(complex z)
{
	double real = z.real, imag = z.imag;
	double r = sqrt(real * real + imag * imag), θ = atan(imag / real);
	complex result;
	result.real = log(r);
	result.imag = θ;
	return result;
}

complex Epow(complex z) 
{
	complex result;
	result.real = pow(const_e, z.real)*cos(z.imag);
	result.imag = pow(const_e, z.real)*sin(z.imag);
	return result;
}

complex Sin(complex z) 
{
	double real = z.real, imag = z.imag;
	complex iz, miz, result;
	comInit(&iz, -imag, real);
	comInit(&miz, imag, -real);
	result = complexI(Epow(iz), Epow(miz));
	comInit(&result, result.imag / 2, -result.real / 2);
	return result;
}

complex Cos(complex z) 
{
	double real = z.real, imag = z.imag;
	complex iz, miz, result;
	comInit(&iz, -imag, real);
	comInit(&miz, imag, -real);
	result = complexA(Epow(iz), Epow(miz));
	comInit(&result, result.real / 2, result.imag / 2);
	return result;
}

int sgn(double N) 
{
	if (N == 0) 		return 0;
	else 
		if (N > 0) 			return 1;
		else 
			return -1;
}

//当运算的输入为非法数值时的错误警告（比如复数或小数的阶乘，0的倒数）
void Invalid_input() 
{
	calculator_init(true);
	pInputBox->SetWindowTextW(L"Invalid input");
}


BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		int nkeyDown = pMsg->wParam;
		SHORT nkeyCtrl = GetKeyState(VK_CONTROL),			//获取Ctrl键的状态
			nkeyShift = GetKeyState(VK_SHIFT);			
		//判断值为键盘上文字对应的ASCII码对应的int值
		if (state == COMPLEX) 
		{
			switch (nkeyDown) 
			{
			case VK_BACK:
				OnButtonBs();
				break;
			case VK_TAB:
				OnButtonSwitch();
				break;
			case VK_RETURN:
			case 61:
				OnButtonEqu();
				break;
			case VK_MULTIPLY:
				OnButtonMul();
				break;
			case VK_ADD:
				OnButtonAdd();
				break;
			case VK_SUBTRACT:
				OnButtonMin();
				break;
			case 46:
			case VK_DECIMAL:
				OnButtonPoint();
				break;
			case VK_DIVIDE:
				OnButtonDiv();
				break;
			case 48:
			case VK_NUMPAD0:
				OnButton0();
				break;
			case 49:
			case VK_NUMPAD1:
				OnButton1();
				break;
			case 50:
			case VK_NUMPAD2:
				OnButton2();
				break;
			case 51:
			case VK_NUMPAD3:
				OnButton3();
				break;
			case 52:
			case VK_NUMPAD4:
				OnButton4();
				break;
			case 53:
			case VK_NUMPAD5:
				OnButton5();
				break;
			case 54:
			case VK_NUMPAD6:
				OnButton6();
				break;
			case 55:
			case VK_NUMPAD7:
				OnButton7();
				break;
			case 56:
			case VK_NUMPAD8:
				OnButton8();
				break;
			case 57:
			case VK_NUMPAD9:
				OnButton9();
				break;
			default:
				break;
			}	
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}


void CChildView::OnClose()
{
	userLog.Close();
	CWnd::OnClose();
}
