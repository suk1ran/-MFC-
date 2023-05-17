#pragma once
#include "afxdialogex.h"

#include <afx.h>
#include <afxwin.h>
#include <vector>
#include <queue>

#include "afxtempl.h"

#include "CHero.h"


#include <vector>


// CMazeDlg 对话框

class CMazeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMazeDlg)

public:
	CMazeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMazeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	
	


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	
	void PrimGenerate();
	
	void DfsSolve();

private:
	CStatic m_MazePicCtrl;



	CComboBox Box;
	CFont m_showFont1;
	CFont m_showFont2;
	CFont m_showFont3;
	CBrush m_brush[5];
	
	CHero m_hero;
	CPen pen1;
	int speed = 150; // 设置速度
	int score = 0;
	bool INIT = true;
	bool START = false;
	bool DEAD = false;


	static const int L = 40;//TODO 可自定义
	CRect m_map[L][L];



public:

	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonShow();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnEnChangeEditCols();
	afx_msg void OnEnChangeEditRows();
	afx_msg void OnStnClickedMazepic();
};

