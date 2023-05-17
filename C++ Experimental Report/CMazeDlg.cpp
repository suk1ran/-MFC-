// CMazeDlg.cpp: 实现文件
//

#include "pch.h"
#include "C++ Experimental Report.h"
#include "afxdialogex.h"
#include "CMazeDlg.h"

#include "afxtempl.h"

#include <queue>
#include <random>


#include <vector>
#include <set>
#include <stack>






// CMazeDlg 对话框

IMPLEMENT_DYNAMIC(CMazeDlg, CDialogEx)







CMazeDlg::CMazeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAZE, pParent)
{
	
}

CMazeDlg::~CMazeDlg()
{
}

void CMazeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAZEPIC, m_MazePicCtrl);
}


BEGIN_MESSAGE_MAP(CMazeDlg, CDialogEx)
	ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON_START, &CMazeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CMazeDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CMazeDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CMazeDlg::OnBnClickedButtonSet)
	ON_EN_CHANGE(IDC_EDIT_COLS, &CMazeDlg::OnEnChangeEditCols)
	ON_EN_CHANGE(IDC_EDIT_ROWS, &CMazeDlg::OnEnChangeEditRows)
	ON_STN_CLICKED(IDC_MAZEPIC, &CMazeDlg::OnStnClickedMazepic)
END_MESSAGE_MAP()


// CMazeDlg 消息处理程序


BOOL CMazeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMazeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	
    
	CRect rect;
	(this->GetDlgItem(IDC_MAZEPIC))->GetWindowRect(&rect); // 获取控件相对于屏幕的位置
	ScreenToClient(&rect); // 转化为相对于客户区的位置
	GetDlgItem(IDC_MAZEPIC)->MoveWindow(rect.left, rect.top -30, 800, 800, false);

	

	/*CWnd* pPicCtrl = GetDlgItem(IDC_MAZEPIC);
	CRect rcNewPos(10, 10, 800, 800);
	pPicCtrl->MoveWindow(&rcNewPos, TRUE);*/





	CDC* pClientDC = GetDC();
	(this->GetDlgItem(IDC_MAZEPIC))->GetWindowRect(&rect); // 获取控件相对于屏幕的位
	ScreenToClient(rect); // 转化为相对于客户区的位置
	CPen pen(PS_SOLID, 6, RGB(255, 160, 122));
	CPen pen2(PS_SOLID, 18, RGB(255, 160, 122));
	CPen* oldPen = pClientDC->SelectObject(&pen2);//描绘边缘
	pClientDC->Rectangle(rect);
	rect.left -= 5;
	rect.right += 5;
	rect.top -= 5;
	rect.bottom += 5;
	pClientDC->SelectObject(&pen);//描绘外围框
	pClientDC->Rectangle(rect);
	pen.DeleteObject(); 
	pen2.DeleteObject();
	pClientDC->DeleteDC();

	// 画刷初始化
	CBitmap  herobmp, wallbmp, bgbmp, ptbmp, swbmp;
	bgbmp.LoadBitmapW(IDB_BACKGROUND);
	wallbmp.LoadBitmapW(IDB_WALL);
	herobmp.LoadBitmapW(IDB_HERO);
	ptbmp.LoadBitmapW(IDB_POINT);
	swbmp.LoadBitmapW(IDB_SHOWWAY);
	m_brush[0].CreatePatternBrush(&ptbmp);
	m_brush[1].CreatePatternBrush(&herobmp);
	m_brush[2].CreatePatternBrush(&wallbmp);
	m_brush[3].CreatePatternBrush(&bgbmp);
	m_brush[4].CreatePatternBrush(&swbmp);


	// 游戏区
	CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	// 迷宫初始化
	CBrush* poldBrs = pdc->SelectObject(&m_brush[3]); 
	CPen* poldPen = pdc->SelectObject(&pen1); 
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			m_map[i][j].left = 0 + j * 20;
			m_map[i][j].right = 20 + j * 20;
			m_map[i][j].top = 0 + i * 20;
			m_map[i][j].bottom = 20 + i * 20;
			pdc->SelectObject(m_brush[3]);
			pdc->Rectangle(m_map[i][j]);
			pdc->SelectObject(&poldBrs);
		}
	}
	pdc->SelectObject(&poldBrs);
}

void CMazeDlg::PrimGenerate()
{

	// 游戏区
	CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	// 迷宫初始化,绘制背景
	CBrush* poldBrs = pdc->SelectObject(&m_brush[3]);
	CPen* poldPen = pdc->SelectObject(&pen1);
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			m_map[i][j].left = 0 + j * 20;
			m_map[i][j].right = 20 + j * 20;
			m_map[i][j].top = 0 + i * 20;
			m_map[i][j].bottom = 20 + i * 20;
			pdc->SelectObject(m_brush[3]);
			pdc->Rectangle(m_map[i][j]);
			pdc->SelectObject(&poldBrs);
		}
	}



	int Maze[L][L] = { 0 };//0为墙，1为路,-1为边界（无法破坏）

	//最外围设置为边界
	for (int i = 0; i < L; i++) 
	{
		Maze[i][0] = -1;
		Maze[0][i] = -1;
		Maze[L - 1][i] = -1;
		Maze[i][L - 1] = -1;
	}

	//墙队列，包括X , Y
	std::vector<int> X;
	std::vector<int> Y;


	//设置迷宫进出口
	Maze[0][1] = 1;
	Maze[L - 1][L - 2] = 1;
	


	//把起点和终点的邻居放入墙队列
	X.push_back(1);
	Y.push_back(1);

	X.push_back(L-2);
	Y.push_back(L-2);

	//当墙队列为空时结束循环
	while (X.size())
	{
		//在墙队列中随机取一点
		int r = rand() % X.size();
		int x = X[r];
		int y = Y[r];

		//判读上下左右四个方向是否为路
		int count = 0;
		for (int i = x - 1; i < x + 2; i++) {
			for (int j = y - 1; j < y + 2; j++) {
				if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] > 0) {
					++count;
				}
			}
		}

		if (count <= 1) 
		{
			Maze[x][y] = 1;
			//在墙队列中插入新的墙
			for (int i = x - 1; i < x + 2; i++) {
				for (int j = y - 1; j < y + 2; j++) {
					if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] == 0) {
						X.push_back(i);
						Y.push_back(j);
					}
				}
			}
		}

		//删除当前墙
		X.erase(X.begin() + r);
		Y.erase(Y.begin() + r);
	}

	

	//画迷宫
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			if (Maze[i][j] == 1) // 路
			{
				continue;
			}
			else // 墙
			{

				CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
				CBrush* poldBrs = pdc->SelectObject(&m_brush[2]);
				CPen* poldPen = pdc->SelectObject(&pen1);


				m_map[i][j].left = 0 + j * 20;
				m_map[i][j].right = 20 + j * 20;
				m_map[i][j].top = 0 + i * 20;
				m_map[i][j].bottom = 20 + i * 20;


				pdc->SelectObject(m_brush[2]);
				pdc->Rectangle(m_map[i][j]);
				pdc->SelectObject(&poldBrs);
			}
		}
	}

	//画人物
	pdc->SelectObject(&m_brush[1]);
	pdc->SelectObject(&pen1);
	pdc->Rectangle(m_map[0][1]);
	pdc->SelectObject(&poldBrs);


	//画终点
	pdc->SelectObject(&m_brush[0]);
	pdc->SelectObject(&pen1);
	pdc->Rectangle(m_map[L-1][L-2]);
	pdc->SelectObject(&poldBrs);


}











void CMazeDlg::DfsSolve()
{
	




}

	//MessageBox(_T("恭喜您，成功找到出口！"), _T("迷宫解决成功"), MB_OK);



void CMazeDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//CBitmap bitmap; // CBitmap对象，用于加载位图
	//HBITMAP hBmp;   // 保存CBitmap加载的位图的句柄

	//bitmap.LoadBitmapW(IDB_BACKGROUND);    // 将位图IDB_BITMAP1加载到bitmap
	//hBmp = (HBITMAP)bitmap.GetSafeHandle(); // 获取bitmap加载位图的句柄
	//m_MazePicCtrl.SetBitmap(hBmp);   // 设置图片空间m_pictureControl的位图为IDB_BITMAP1

	PrimGenerate();


}


void CMazeDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码

	EndDialog(0);
}


void CMazeDlg::OnBnClickedButtonShow()
{
	// TODO: 在此添加控件通知处理程序代码
	DfsSolve();
}


void CMazeDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMazeDlg::OnEnChangeEditCols()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMazeDlg::OnEnChangeEditRows()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMazeDlg::OnStnClickedMazepic()
{
	// TODO: 在此添加控件通知处理程序代码
}


