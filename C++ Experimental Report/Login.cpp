// Login.cpp: 实现文件
//

#include "pch.h"
#include "C++ Experimental Report.h"
#include "afxdialogex.h"
#include "Login.h"
#include "InfoFile.h"


// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, m_user(_T(""))
	, m_pwd(_T(""))
{

}

Login::~Login()
{
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_pwd);
}



BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Login::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Login::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Login 消息处理程序


void Login::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	if (m_user.IsEmpty() || m_pwd.IsEmpty())
	{
		MessageBox(TEXT("输入的内容不能为空"));
		return ;
	}

	InfoFile file;
	CString name;
	CString pwd;
	file.ReadLogin(name, pwd);

	if (name == m_user)
	{
		if (pwd == m_pwd)
		{
			CDialog::OnCancel();
		}
		else
		{
			MessageBox(TEXT("密码错误"));
		}
	}
	else
	{
		MessageBox(TEXT("用户名不存在"));

	}
}


BOOL Login::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//默认登录信息
	InfoFile file;
	CString name;
	CString pwd;
	file.ReadLogin(name, pwd);
	m_user = name;
	m_pwd = pwd;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE


}


void Login::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	exit(0);
}


void Login::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	InfoFile file;
	CString name;
	CString pwd;
	file.ReadLogin(name, pwd);
	UpdateData(TRUE);
	if (m_user == name && m_pwd == pwd)
	{
		CDialogEx::OnOK();
	}
	else
	{
		MessageBox(TEXT("用户名或密码错误"));
	}
	
}


void Login::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnClose();

	//退出程序
	exit(0);
}
