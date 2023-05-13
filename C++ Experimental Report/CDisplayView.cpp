// CDisplayView.cpp: 实现文件
//

#include "pch.h"
#include "C++ Experimental Report.h"
#include "CDisplayView.h"


// CDisplayView

IMPLEMENT_DYNCREATE(CDisplayView, CTreeView)

CDisplayView::CDisplayView()
{

}

CDisplayView::~CDisplayView()
{
}

BEGIN_MESSAGE_MAP(CDisplayView, CTreeView)
END_MESSAGE_MAP()


// CDisplayView 诊断

#ifdef _DEBUG
void CDisplayView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CDisplayView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDisplayView 消息处理程序
