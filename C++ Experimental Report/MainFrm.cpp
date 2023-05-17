
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "C++ Experimental Report.h"

#include "MainFrm.h"
#include "InfoFile.h"
#include "CSelectView.h"
#include "CDisplayView.h"

#include "CUserDlg.h"

//挂载何解？？？？？？？？？？？？？
//#include "CMazeDlg.h"



#define GCL_HICON           (-12)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()

	ON_MESSAGE(NM_A, OnMyChange)
	ON_MESSAGE(NM_B, OnMyChange)
	ON_MESSAGE(NM_C, OnMyChange)
	ON_MESSAGE(NM_D, OnMyChange)
	ON_MESSAGE(NM_E, OnMyChange)


END_MESSAGE_MAP()






static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//设置图标
    //SetClassLong(m_hWnd, GCL_HICON, (LONG)AfxGetApp()->LoadIconW(IDI_ICON_GOAT));
	
	
	//设置右标题
	SetTitle(TEXT("v1.0.0-beta.1 （Powered By he277）"));

	//设置窗口大小
	MoveWindow(0,0,1920,1080);

	//窗口居中
	CenterWindow();



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	//return CFrameWnd::OnCreateClient(lpcs, pContext);

	//拆分为一行两列
	m_spliter.CreateStatic(this, 1, 2);

	//左侧与右侧的参数
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CSelectView), CSize(200, 500), pContext);
	m_spliter.CreateView(0, 1, RUNTIME_CLASS(CDisplayView), CSize(600, 500), pContext);

	//ShowWindow(SW_SHOWMAXIMIZED);

	return TRUE;
}


LRESULT CMainFrame::OnMyChange(WPARAM wParam, LPARAM lParam)
{
	CCreateContext Context;
	switch (wParam)
	{
	case NM_A:
	{
		//CUserDlg类需要加入头文件 UserDlg.h
		Context.m_pNewViewClass = RUNTIME_CLASS(CUserDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CUserDlg), CSize(600, 0), &Context);
		CUserDlg* pNewview = (CUserDlg*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewview->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
	}
		break;
	case NM_B:
	{
		//CMazeDlg类需要加入头文件 CMazeDlg.h
		/*Context.m_pNewViewClass = RUNTIME_CLASS(CMazeDlg);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CMazeDlg), CSize(600, 0), &Context);
		CMazeDlg* pNewview = (CMazeDlg*)m_spliter.GetPane(0, 1);
		m_spliter.RecalcLayout();
		pNewview->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);*/
		MessageBox(TEXT("敬请期待"));
	}
		break;
	case NM_C:
		MessageBox(TEXT("敬请期待"));
		break;
	case NM_D:
		MessageBox(TEXT("敬请期待"));
		break;
	case NM_E:
		MessageBox(TEXT("敬请期待"));
		break;
	default:
		MessageBox(TEXT("error"));
	}
	return 0;
}


