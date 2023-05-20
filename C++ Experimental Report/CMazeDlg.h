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

	void PrimGenerate(int random_point);

	int Find(int t);
	void Union(int a, int b);
	bool IsSame(int a, int b);
	void UnionFindGenerate();

	void DfsSolve(int x, int y);

	bool isValidMove(int x, int y);
	void movePlayer(int x, int y);

	bool SaveMazeData();
	void ShowMazeData();

private:
	CStatic m_MazePicCtrl;
	CBrush m_brush[5];
	CPen pen1;
	CHero m_hero;
	
	
	//单元格队列，包括X , Y
	//std::vector<std::pair<int, int>> Block;

	

	bool randGenerate = false;
	std::pair<int, int> randPoint;
	static const int L = 41;//TODO 可自定义
	CRect m_map[L][L];

	int pre[L * L + L];//存储每个结点的前驱结点 
	int rank[L * L + L];//树的高度 

	int m_x;
	int m_y;
	int BOOK[L][L]; //标记可走路线, 用于迷宫求解,为1时可走
	int step;
	int min_step;

	int m_second;           // 秒数
	int m_minute;           // 分钟数
	BOOL m_timerFlag;    // 标记计时器是否在计时


	const int dx[4] = { -1, 1, 0, 0 };
	const int dy[4] = { 0, 0, -1, 1 };


public:

	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonShow();
	afx_msg void OnEnChangeEditCols();
	afx_msg void OnEnChangeEditRows();
	afx_msg void OnStnClickedMazepic();
	afx_msg void OnBnClickedButtonIds();
	afx_msg void OnBnClickedButtonStart3();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLook();
};

