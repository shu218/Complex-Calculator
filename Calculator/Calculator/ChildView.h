
// ChildView.h: CChildView 类的接口
//


#pragma once
#include "resource.h"       // 主符号
// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();
	void Complex_Init();
	void Graph_Init();
	void Frm_Init();

// 特性
public:

// 操作
public:
	afx_msg void OnComplex();
	afx_msg void OnGraph();

	afx_msg void OnButton0();
	afx_msg void OnButton1();
	afx_msg void OnButton10pow();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonArccos();
	afx_msg void OnButtonArcsin();
	afx_msg void OnButtonArctan();
	afx_msg void OnButtonRbr();
	afx_msg void OnButtonBs();
	afx_msg void OnButtonC();
	afx_msg void OnButtonCe();
	afx_msg void OnButtonCont();
	afx_msg void OnButtonCos();
	afx_msg void OnButtonDiv();
	afx_msg void OnButtonEpow();
	afx_msg void OnButtonEqu();
	afx_msg void OnButtonExta();
	afx_msg void OnButtonFact();
	afx_msg void OnButtonI();
	afx_msg void OnButtonθ();
	afx_msg void OnButtonLbr();
	afx_msg void OnButtonLn();
	afx_msg void OnButtonLog();
	afx_msg void OnButtonMin();
	afx_msg void OnButtonMul();
	afx_msg void OnButtonPoint();
	afx_msg void OnButtonPhi();
	afx_msg void OnButtonPow();
	afx_msg void OnButtonReci();
	afx_msg void OnButtonPow2();
	afx_msg void OnButtonRoot();
	afx_msg void OnButtonSin();
	afx_msg void OnButtonTan();
	afx_msg void OnButtonMS();
	afx_msg void OnButtonMR();
	afx_msg void OnButtonMC();
	afx_msg void OnButtonSwitch();

	afx_msg void OnStart();
	afx_msg void OnButtonBode();

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButtonNyqu();
	afx_msg void OnClose();
};

// CCalculatorApp:
// 有关此类的实现，请参阅 Calculator.cpp
//

class CCalculatorApp : public CWinApp
{
public:
	CCalculatorApp() noexcept;


	// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// 实现

public:
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCalculatorApp theApp;

