
// C++ Experimental ReportView.h: CCExperimentalReportView 类的接口
//

#pragma once


class CCExperimentalReportView : public CView
{
protected: // 仅从序列化创建
	CCExperimentalReportView() noexcept;
	DECLARE_DYNCREATE(CCExperimentalReportView)

// 特性
public:
	CCExperimentalReportDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCExperimentalReportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:

//	afx_msg void OnMazeDlg();
};

#ifndef _DEBUG  // C++ Experimental ReportView.cpp 中的调试版本
inline CCExperimentalReportDoc* CCExperimentalReportView::GetDocument() const
   { return reinterpret_cast<CCExperimentalReportDoc*>(m_pDocument); }
#endif

