#pragma once

// Bode_plot 对话框

class Bode_plot : public CDialogEx
{
	DECLARE_DYNAMIC(Bode_plot)

public:
	Bode_plot(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Bode_plot();
	double left_num[5] = { 0 }, right_num[6] = { 0 };
	double A = 0, B = 0;				//阻尼比的左右限
	int x_range, L_range, R_range, y_dec, dB_y_axis, Ang_y_axis;
	bool Function;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnButtonComp();
};
