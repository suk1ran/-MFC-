
// C++ Experimental ReportView.cpp: CCExperimentalReportView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "C++ Experimental Report.h"
#endif

#include "C++ Experimental ReportDoc.h"
#include "C++ Experimental ReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// CCExperimentalReportView

IMPLEMENT_DYNCREATE(CCExperimentalReportView, CView)

BEGIN_MESSAGE_MAP(CCExperimentalReportView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_COMMAND(IDM_MAZE, &CCExperimentalReportView::OnMaze)
//ON_COMMAND(IDM_MAZE_DLG, &CCExperimentalReportView::OnMazeDlg)
END_MESSAGE_MAP()

// CCExperimentalReportView 构造/析构

CCExperimentalReportView::CCExperimentalReportView() noexcept
{
	// TODO: 在此处添加构造代码

}

CCExperimentalReportView::~CCExperimentalReportView()
{
}

BOOL CCExperimentalReportView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCExperimentalReportView 绘图

void CCExperimentalReportView::OnDraw(CDC* /*pDC*/)
{
	CCExperimentalReportDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCExperimentalReportView 打印

BOOL CCExperimentalReportView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCExperimentalReportView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCExperimentalReportView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCExperimentalReportView 诊断

#ifdef _DEBUG
void CCExperimentalReportView::AssertValid() const
{
	CView::AssertValid();
}

void CCExperimentalReportView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCExperimentalReportDoc* CCExperimentalReportView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCExperimentalReportDoc)));
	return (CCExperimentalReportDoc*)m_pDocument;
}
#endif //_DEBUG


// CCExperimentalReportView 消息处理程序







