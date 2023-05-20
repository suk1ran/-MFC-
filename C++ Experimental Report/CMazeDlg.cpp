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
#include <ctime>

#include <fstream>
#include <chrono>
#include <iomanip>

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::setfill;
using std::ofstream;
using std::ios;


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
	ON_STN_CLICKED(IDC_MAZEPIC, &CMazeDlg::OnStnClickedMazepic)
    ON_BN_CLICKED(IDC_BUTTON_START2, &CMazeDlg::OnBnClickedButtonIds)
    ON_BN_CLICKED(IDC_BUTTON_START3, &CMazeDlg::OnBnClickedButtonStart3)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMazeDlg::OnBnClickedButtonSave)
    ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOOK, &CMazeDlg::OnBnClickedButtonLook)
END_MESSAGE_MAP()


// CMazeDlg 消息处理程序


BOOL CMazeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	(this->GetDlgItem(IDC_MAZEPIC))->GetWindowRect(&rect); // 获取控件相对于屏幕的位置
	ScreenToClient(&rect); // 转化为相对于客户区的位置
	GetDlgItem(IDC_MAZEPIC)->MoveWindow(rect.left, rect.top - 30, 820, 820, false);



	// 画刷初始化
	CBitmap  herobmp, wallbmp, bgbmp, ptbmp, swbmp;
	bgbmp.LoadBitmapW(IDB_BACKGROUND);
	wallbmp.LoadBitmapW(IDB_WALL);
	herobmp.LoadBitmapW(IDB_HERO);
	ptbmp.LoadBitmapW(IDB_POINT);
	swbmp.LoadBitmapW(IDB_SHOWWAY);
	m_brush[0].CreatePatternBrush(&ptbmp);//终点
	m_brush[1].CreatePatternBrush(&herobmp);//人物
	m_brush[2].CreatePatternBrush(&wallbmp);//墙体
	m_brush[3].CreatePatternBrush(&bgbmp);//背景
	m_brush[4].CreatePatternBrush(&swbmp);//路径

	step = 0;
	min_step = 999;
	GetDlgItem(IDC_EDIT_STEPS)->SetWindowTextW(_T("0"));



	m_second = 0;           // 初始化秒数
	m_minute = 0;           // 初始化分钟数
	m_timerFlag = FALSE;    // 计时器没有开始计时
	GetDlgItem(IDC_EDIT_COUNTTIME)->SetWindowTextW(_T("00:00")); // 在Edit Control控件中显示时间




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMazeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	
    
	CRect rect;
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
	ReleaseDC(pClientDC);

	


	// 游戏区
	CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	// 迷宫初始化
	CBrush* poldBrs = pdc->SelectObject(&m_brush[3]); 
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 41; j++)
		{
			m_map[i][j].left = 0 + j * 20;
			m_map[i][j].right = 20 + j * 20;
			m_map[i][j].top = 0 + i * 20;
			m_map[i][j].bottom = 20 + i * 20;
			//pdc->SelectObject(m_brush[3]);
			pdc->Rectangle(m_map[i][j]);
	
		}
	}
	pdc->SelectObject(&poldBrs);
	poldBrs->DeleteObject();
	ReleaseDC(pdc);
}


//固定入口,固定出口
void CMazeDlg::PrimGenerate()
{
	//求解扳机
	randGenerate = false;

	// 清空步数和时间
	step = 0;
	m_second = 0;
	m_minute = 0;
	min_step = 999;
	GetDlgItem(IDC_EDIT_STEPS)->SetWindowTextW(_T("0"));
	GetDlgItem(IDC_EDIT_COUNTTIME)->SetWindowTextW(_T("00:00"));
	// 停止计时
	KillTimer(1);
	m_timerFlag = FALSE;
	
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
	std::vector<std::pair<int, int>> Wall;



	//设置迷宫入口
	Maze[0][1] = 1;
	m_x = 0;
	m_y = 1;

	//设置迷宫出口
	//Maze[L - 1][L - 2] = 1;
	randPoint.first = L - 2;
	randPoint.second = L - 2;
	

	//把起点的邻居放入墙队列
	Wall.emplace_back(1, 1);



	//当墙队列为空时结束循环
	while (!Wall.empty())
	{
		//在墙队列中随机取一点
		int idx = rand() % Wall.size();
		int x = Wall[idx].first;
		int y = Wall[idx].second;

		//判断上下左右四个方向是否为路
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
						Wall.emplace_back(i, j);
					}
				}
			}
		}

		//删除当前墙
		Wall.erase(Wall.begin() + idx);

	}


	if (Maze[L - 2][L - 2] != 1)//确保迷宫有解
	{
		PrimGenerate();
		return;
	}



	//标记可走路线,用于迷宫求解
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			BOOK[i][j] = Maze[i][j];
		}
	}
	
	
	

	// 游戏区
	CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	// 迷宫初始化,绘制背景
	CBrush* poldBrs = pdc->SelectObject(&m_brush[3]);
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 41; j++)
		{
			m_map[i][j].left = 0 + j * 20;
			m_map[i][j].right = 20 + j * 20;
			m_map[i][j].top = 0 + i * 20;
			m_map[i][j].bottom = 20 + i * 20;
			//pdc->SelectObject(m_brush[3]);
			pdc->Rectangle(m_map[i][j]);
			//pdc->SelectObject(&poldBrs);
		}
	}
	pdc->SelectObject(&poldBrs);
	//ReleaseDC(pdc);



	//画迷宫
	pdc->SelectObject(m_brush[2]);
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			if (Maze[i][j] == 1) // 路
			{
				continue;
			}
			else // 墙或边界
			{

				m_map[i][j].left = 0 + j * 20;
				m_map[i][j].right = 20 + j * 20;
				m_map[i][j].top = 0 + i * 20;
				m_map[i][j].bottom = 20 + i * 20;


				//pdc->SelectObject(m_brush[2]);
				pdc->Rectangle(m_map[i][j]);
				//pdc->SelectObject(&poldBrs);
				//ReleaseDC(pdc);
				
				poldBrs->DeleteObject();
			}
		}
	}

	pdc->SelectObject(&poldBrs);

	/*CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	CBrush* poldBrs = pdc->SelectObject(&m_brush[1]);*/


	//画人物
	pdc->SelectObject(&m_brush[1]);
	pdc->Rectangle(m_map[0][1]);
	pdc->SelectObject(&poldBrs);

	//画终点
	pdc->SelectObject(&m_brush[0]);
	pdc->Rectangle(m_map[L - 1][L - 2]);
	pdc->SelectObject(&poldBrs);

	poldBrs->DeleteObject();
	ReleaseDC(pdc); // 释放设备上下文对象

}


//固定入口,随机出口
void CMazeDlg::PrimGenerate(int random_point)
{
	//求解扳机
	randGenerate = true;

	// 清空步数和时间
	step = 0;
	m_second = 0;
	m_minute = 0;
	min_step = 999;
	GetDlgItem(IDC_EDIT_STEPS)->SetWindowTextW(_T("0"));
	GetDlgItem(IDC_EDIT_COUNTTIME)->SetWindowTextW(_T("00:00"));
	// 停止计时
	KillTimer(1);
	m_timerFlag = FALSE;


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
	std::vector<std::pair<int, int>> Wall;

	//记录可作为终点的点的个数
	std::vector<std::pair<int, int>> Point;


	//设置迷宫入口
	Maze[0][1] = 1;
	m_x = 0;
	m_y = 1;


	//把起点的邻居放入墙队列
	Wall.emplace_back(1, 1);



	//当墙队列为空时结束循环
	while (!Wall.empty())
	{
		//在墙队列中随机取一点
		int idx = rand() % Wall.size();
		int x = Wall[idx].first;
		int y = Wall[idx].second;

		//判断上下左右四个方向是否为路
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
			Point.emplace_back(x, y);
			//在墙队列中插入新的墙
			for (int i = x - 1; i < x + 2; i++) {
				for (int j = y - 1; j < y + 2; j++) {
					if (abs(x - i) + abs(y - j) == 1 && Maze[i][j] == 0) {
						Wall.emplace_back(i, j);
					}
				}
			}
		}

		//删除当前墙
		Wall.erase(Wall.begin() + idx);

	}


	//标记可走路线,用于迷宫求解
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			BOOK[i][j] = Maze[i][j];
		}
	}


	//设置随机出口
	int p_idx = rand() % Point.size();
	int p_x = Point[p_idx].first;
	int p_y = Point[p_idx].second;
	randPoint.first = p_x;
	randPoint.second = p_y;


	// 游戏区
	CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	// 迷宫初始化,绘制背景
	CBrush* poldBrs = pdc->SelectObject(&m_brush[3]);
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 41; j++)
		{
			m_map[i][j].left = 0 + j * 20;
			m_map[i][j].right = 20 + j * 20;
			m_map[i][j].top = 0 + i * 20;
			m_map[i][j].bottom = 20 + i * 20;
			//pdc->SelectObject(m_brush[3]);
			pdc->Rectangle(m_map[i][j]);
			//pdc->SelectObject(&poldBrs);
		}
	}
	pdc->SelectObject(&poldBrs);
	//ReleaseDC(pdc);



	//画迷宫
	pdc->SelectObject(m_brush[2]);
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			if (Maze[i][j] == 1) // 路
			{
				continue;
			}
			else // 墙或边界
			{

				m_map[i][j].left = 0 + j * 20;
				m_map[i][j].right = 20 + j * 20;
				m_map[i][j].top = 0 + i * 20;
				m_map[i][j].bottom = 20 + i * 20;


				//pdc->SelectObject(m_brush[2]);
				pdc->Rectangle(m_map[i][j]);
				//pdc->SelectObject(&poldBrs);
				//ReleaseDC(pdc);

				poldBrs->DeleteObject();
			}
		}
	}

	pdc->SelectObject(&poldBrs);

	/*CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	CBrush* poldBrs = pdc->SelectObject(&m_brush[1]);*/


	//画人物
	pdc->SelectObject(&m_brush[1]);
	pdc->Rectangle(m_map[0][1]);
	pdc->SelectObject(&poldBrs);

	//画终点
	pdc->SelectObject(&m_brush[0]);
	pdc->Rectangle(m_map[p_x][p_y]);
	pdc->SelectObject(&poldBrs);

	poldBrs->DeleteObject();
	ReleaseDC(pdc); // 释放设备上下文对象

}


void CMazeDlg::Union(int a, int b)
{
	a = Find(a);						//寻找 x的代表元
	b = Find(b);						//寻找 y的代表元
	if (a == b) return;		//如果 x和 y的代表元一致，说明他们共属同一集合，则不需要合并，返回 false，表示合并失败；否则，执行下面的逻辑
	if (rank[a] > rank[b]) pre[b] = a;		//如果 x的高度大于 y，则令 y的上级为 x
	else								//否则
	{
		if (rank[a] == rank[b]) rank[b]++;	//如果 x的高度和 y的高度相同，则令 y的高度加1
		pre[a] = b;						//让 x的上级为 y
	}
	return;						//返回 true，表示合并成功
	
}


int CMazeDlg::Find(int t)
{
	if (pre[t] == t)//递归出口：x的上级为 x本身，即 x为根结点
	{
		return t;
	}
	//此代码相当于先找到根结点 root_t，然后 pre[t]=root_t
	return pre[t] = Find(pre[t]);
}


bool CMazeDlg::IsSame(int a, int b)
{
	return Find(a) == Find(b);  	//判断两个结点的根结点（即代表元）是否相同 
}


//固定入口,随机出口
void CMazeDlg::UnionFindGenerate()
{
	//求解扳机
	randGenerate = true;


	//初始化
	for (int i = 0; i < (L * L + L); i++)
	{
		pre[i] = i;     			//每个结点的上级都是自己 
		rank[i] = 1;    			//每个结点构成的树的高度为 1 
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

	//设置迷宫入口和邻居
	Maze[0][1] = 1;
	Maze[1][1] = 1;

	//设置迷宫出口和邻居
	Maze[L-1][L-2] = 1;
	Maze[L-2][L-2] = 1;

	
	//设定上下左右
	enum {UP,DOWN,LEFT,RIGHT};

	//随机取一单元格
	//一维数组转化为二维数组 a[i*L+j]相当于a[i][j]
	int i = rand() % L;
	int j = rand() % L;

	/*int q;
	int e;
	bool look = true;*/

	//int tmp = i * L + j;
	while (!IsSame(7, 42))
	{
		i = rand() % 40;
		j = rand() % 40;

		//随机选取一面墙
		int dir = rand() % 4;
		switch (dir)
		{
		case UP:
			/* q = i - 1;
			 e = j;
			
			if (q - 1 >= 0 && Maze[q - 1][e] == 1&&Maze[q+1][e]==1&&Maze[q][e-1]==1&&Maze[q][e+1]==1)
			{
				look = false;
			}*/
			if (Maze[i - 1][j] == -1)
			{
				break;
			}
			else if (!IsSame(i * L + j, (i - 1) * L + j))
			{
				Maze[i - 1][j] = 1;
				Union(i * L + j, (i - 1) * L + j);
			}
			else
			{
				break;
			}
		case DOWN:
			if (Maze[i + 1][j] == -1)
			{
				break;
			}
			else if (!IsSame(i * L + j, (i + 1) * L + j))
			{
				Maze[i + 1][j] = 1;
				Union(i * L + j, (i + 1) * L + j);
			}
			else
			{
				break;
			}
		case LEFT:
			if (Maze[i][j - 1] == -1)
			{
				break;
			}
			else if (!IsSame(i * L + j, i * L + j - 1))
			{
				Maze[i][j - 1] = 1;
				Union(i * L + j, i * L + j - 1);
			}
			else
			{
				break;
			}
		case RIGHT:
			if (Maze[i][j + 1] == -1)
			{
				break;
			}
			else if (!IsSame(i * L + j, i * L + j - 1))
			{
				Maze[i][j + 1] = 1;
				Union(i * L + j, i * L + j - 1);
			}
			else
			{
				break;
			}
		}
	}



	//标记可走路线,用于迷宫求解
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			BOOK[i][j] = Maze[i][j];
		}
	}
	step = 0;
	min_step = 999;


	// 游戏区
	CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	// 迷宫初始化,绘制背景
	CBrush* poldBrs = pdc->SelectObject(&m_brush[3]);
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 41; j++)
		{
			m_map[i][j].left = 0 + j * 20;
			m_map[i][j].right = 20 + j * 20;
			m_map[i][j].top = 0 + i * 20;
			m_map[i][j].bottom = 20 + i * 20;
			//pdc->SelectObject(m_brush[3]);
			pdc->Rectangle(m_map[i][j]);
			//pdc->SelectObject(&poldBrs);
		}
	}
	pdc->SelectObject(&poldBrs);
	//ReleaseDC(pdc);



	//画迷宫
	pdc->SelectObject(m_brush[2]);
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			if (Maze[i][j] == 1) // 路
			{
				continue;
			}
			else // 墙或边界
			{

				m_map[i][j].left = 0 + j * 20;
				m_map[i][j].right = 20 + j * 20;
				m_map[i][j].top = 0 + i * 20;
				m_map[i][j].bottom = 20 + i * 20;


				//pdc->SelectObject(m_brush[2]);
				pdc->Rectangle(m_map[i][j]);
				//pdc->SelectObject(&poldBrs);
				//ReleaseDC(pdc);

				poldBrs->DeleteObject();
			}
		}
	}

	pdc->SelectObject(&poldBrs);

	/*CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	CBrush* poldBrs = pdc->SelectObject(&m_brush[1]);*/


	//画人物
	pdc->SelectObject(&m_brush[1]);
	pdc->Rectangle(m_map[0][1]);
	pdc->SelectObject(&poldBrs);

	//画终点
	pdc->SelectObject(&m_brush[0]);
	pdc->Rectangle(m_map[L - 1][L - 2]);
	pdc->SelectObject(&poldBrs);

	poldBrs->DeleteObject();
	ReleaseDC(pdc); // 释放设备上下文对象

}


//固定起点（1，1）的DFS求解
void CMazeDlg::DfsSolve(int x, int y)
{
	int direction[4][2] =
	{
		{-1, 0}, // 上
		{1, 0},  // 下
		{0, -1}, // 左
		{0, 1}   // 右
	};

	BOOK[x][y] = 2; // 标记当前节点为已访问

	if (x == randPoint.first && y == randPoint.second)
	{
		//更新最短路径
		if (step < min_step)
		{
			min_step = step;
		}

		CDC* pDC = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
		pDC->SelectObject(&m_brush[4]);
		// 画路径
		for (int i = 1; i < L; i++)
		{
			for (int j = 1; j < L; j++)
			{
				//CDC* pDC = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
				if (pDC != nullptr)
				{
					if (BOOK[i][j] == 2)
					{
						//pDC->SelectObject(&m_brush[4]);
						CRect rect(j * 20 + 1, i * 20 + 1, (j + 1) * 20 - 1, (i + 1) * 20 - 1);
						pDC->FillRect(rect, &m_brush[4]);
						//ReleaseDC(pDC);
					}
				}
			}
		}
		ReleaseDC(pDC);
		//如果是随机终点需要绘制完路径后重绘终点
		if (randGenerate == true)
		{
			CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
			CBrush* poldBrs = pdc->SelectObject(&m_brush[0]);
			pdc->Rectangle(m_map[randPoint.first][randPoint.second]);
			pdc->SelectObject(&poldBrs);
			poldBrs->DeleteObject();
			ReleaseDC(pdc);
		}

		// 找到出口
		MessageBox(_T("恭喜您，成功找到出口！"), _T("迷宫寻路成功"), MB_OK);
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		int nx = x + direction[i][0];
		int ny = y + direction[i][1];

		if (nx >= 0 && nx <= L - 1 && ny >= 0 && ny <= L - 1 && BOOK[nx][ny] == 1)
		{

			// 标记下一个点为已经走过
			BOOK[nx][ny] = 2;
			step++;

			// 递归执行下一步
			DfsSolve(nx, ny);

			// 取消下一个点的标记并回溯
			BOOK[nx][ny] = 1;
			step--;
		}
	}

	return;
}


bool CMazeDlg::isValidMove(int x, int y)//传入下一步的坐标
{
	if (BOOK[x][y] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void CMazeDlg::movePlayer(int x, int y)//传入下一步的坐标
{
	CDC* pdc = GetDlgItem(IDC_MAZEPIC)->GetWindowDC();
	CBrush* poldBrs = pdc->SelectObject(&m_brush[3]);
	pdc->Rectangle(m_map[m_x][m_y]);
	pdc->SelectObject(&poldBrs);
	pdc->SelectObject(&m_brush[1]);
	pdc->Rectangle(m_map[x][y]);
	pdc->SelectObject(&poldBrs);
	m_x = x;
	m_y = y;
	ReleaseDC(pdc);


	++step;
	CString str;
	str.Format(_T("%d"), step);
	GetDlgItem(IDC_EDIT_STEPS)->SetWindowTextW(str);


	// 判断计时器是否已经开始计时
	if (!m_timerFlag)
	{
		// 开始计时
		m_timerFlag = TRUE;
		SetTimer(1, 1000, NULL);
	}


	if (x == randPoint.first && y == randPoint.second)
	{
		// 停止计时
		KillTimer(1);
		m_timerFlag = FALSE;
		MessageBox(_T("恭喜你走出迷宫！！！"));
		return;
	}
}


bool CMazeDlg::SaveMazeData()
{
	std::ofstream ofs;
	ofs.open("mazeDaza.txt", std::ios::app);
	if (ofs.is_open())
	{
		int s_step;             // 步数
		int s_minute;           // 分钟数
		int s_second;           // 秒数
		std::string m_date;     // 日期时间
		// 定义一个 SYSTEMTIME 结构体 sys
		SYSTEMTIME sys;
		// GetLocalTime是一个Windows API 函数，用来获取当地的当前系统日期和时间
		GetLocalTime(&sys);

		s_step = step;
		s_minute = m_minute;
		s_second = m_second;



		ofs << "MAZE  " << sys.wYear << "/" <<
			sys.wMonth << "/" << sys.wDay << "  " << std::setfill('0') << std::setw(2) << sys.wHour
			<< ":" << std::setfill('0') << std::setw(2)
			<< sys.wMinute << ":" << std::setfill('0') << std::setw(2)
			<< sys.wSecond << "   " << "所用步数：" << s_step << "    " << "所用时间：" <<
			std::setw(2) << std::setfill('0') << s_minute << ":" <<
			std::setw(2) << std::setfill('0') << s_second << std::endl;

		ofs.close();
		return true;
	}
	return false;
}


void CMazeDlg::ShowMazeData()
{
	// 指定文件名
	LPCTSTR file_name = L"mazeDaza.txt";

	// 打开文件
	HINSTANCE hInstance = ShellExecute(NULL, L"open", file_name, NULL, NULL, SW_SHOWNORMAL);

	if ((int)hInstance <= 32)
	{
		// 打开文件失败
		MessageBox(NULL, L"打开文件失败！", MB_OK);
	}
}


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
	DfsSolve(0,1);
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


void CMazeDlg::OnBnClickedButtonIds()
{
	// TODO: 在此添加控件通知处理程序代码
	PrimGenerate(0);
}


void CMazeDlg::OnBnClickedButtonStart3()
{
	// TODO: 在此添加控件通知处理程序代码
	UnionFindGenerate();
}


void CMazeDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (SaveMazeData())
	{
		MessageBox(_T("保存成功"));
	}
	else
	{
		MessageBox(_T("保存成功"));
	}
}


void CMazeDlg::OnBnClickedButtonLook()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowMazeData();
}


void CMazeDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nChar)
	{
	case 'W':
	case 'w':
		if (isValidMove(m_x - 1, m_y))
		{
			movePlayer(m_x - 1, m_y);
		}
		break;
	case 'A':
	case 'a':
		if (isValidMove(m_x, m_y - 1))
		{
			movePlayer(m_x, m_y - 1);
		}
		break;
	case 'S':
	case 's':
		if (isValidMove(m_x + 1, m_y))
		{
			movePlayer(m_x + 1, m_y);
		}
		break;
	case 'D':
	case 'd':
		if (isValidMove(m_x, m_y + 1))
		{
			movePlayer(m_x, m_y + 1);
		}
		break;
	}

	

	




	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CMazeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//SendMessage(pMsg->message, pMsg->wParam);

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case 'W':
		case 'w':
			if (isValidMove(m_x - 1, m_y))
			{
				movePlayer(m_x - 1, m_y);
			}
			break;
		case 'A':
		case 'a':
			if (isValidMove(m_x, m_y - 1))
			{
				movePlayer(m_x, m_y - 1);
			}
			break;
		case 'S':
		case 's':
			if (isValidMove(m_x + 1, m_y))
			{
				movePlayer(m_x + 1, m_y);
			}
			break;
		case 'D':
		case 'd':
			if (isValidMove(m_x, m_y + 1))
			{
				movePlayer(m_x, m_y + 1);
			}
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
	
}


void CMazeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 1)
	{
		// 更新秒数和分钟数
		++m_second;
		if (m_second >= 60)
		{
			++m_minute;
			m_second = 0;
		}

		// 更新时间
		CString str;
		str.Format(_T("%02d:%02d"), m_minute, m_second);
		GetDlgItem(IDC_EDIT_COUNTTIME)->SetWindowTextW(str);
	}


	CDialogEx::OnTimer(nIDEvent);
}


