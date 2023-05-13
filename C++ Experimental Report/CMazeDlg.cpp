// CMazeDlg.cpp: 实现文件
//

#include "pch.h"
#include "C++ Experimental Report.h"
#include "CMazeDlg.h"


// CMazeDlg

IMPLEMENT_DYNCREATE(CMazeDlg, CFormView)

CMazeDlg::CMazeDlg()
	: CFormView(IDD_MAZE)
{

}

CMazeDlg::~CMazeDlg()
{
}

void CMazeDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMazeDlg, CFormView)
END_MESSAGE_MAP()


// CMazeDlg 诊断

#ifdef _DEBUG
void CMazeDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMazeDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMazeDlg 消息处理程序
