﻿#pragma once



// CMazeDlg 窗体视图

class CMazeDlg : public CFormView
{
	DECLARE_DYNCREATE(CMazeDlg)

protected:
	CMazeDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CMazeDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAZE };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


