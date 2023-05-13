// CUserDlg.cpp: 实现文件
//

#include "pch.h"
#include "C++ Experimental Report.h"
#include "CUserDlg.h"

#include "InfoFile.h"


// CUserDlg

IMPLEMENT_DYNCREATE(CUserDlg, CFormView)

CUserDlg::CUserDlg()
	: CFormView(IDD_USER)
	, m_user(_T(""))
	, m_name(_T(""))
	, m_newPwd(_T(""))
	, m_surePwd(_T(""))
{

}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_name);
	DDX_Text(pDX, IDC_EDIT3, m_newPwd);
	DDX_Text(pDX, IDC_EDIT4, m_surePwd);
}

BEGIN_MESSAGE_MAP(CUserDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CUserDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUserDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CUserDlg 诊断

#ifdef _DEBUG
void CUserDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CUserDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUserDlg 消息处理程序


void CUserDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	m_user = TEXT("管理员");

	InfoFile file;
	CString name;
	CString pwd;
	file.ReadLogin(name, pwd);
	m_name = name;

	UpdateData(FALSE);

}


void CUserDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE); //更新控件内容到对应的变量中

	if (m_newPwd.IsEmpty() || m_surePwd.IsEmpty())
	{
		MessageBox(TEXT("新密码不能为空"));
		return;
	}

	if (m_newPwd != m_surePwd)
	{
		MessageBox(TEXT("新密码和确定密码不一致"));
		return;
	}

	InfoFile file; //需要头文件 InfoFile.h
	CString name, pwd;
	file.ReadLogin(name, pwd); //读取文件的用户名和密码

	if (m_surePwd == pwd)
	{
		MessageBox(TEXT("新密码和旧密码相同"));
		return;
	}

	//把用户名和密码的CString类型改为char *
	char* tmpName, * tmpPwd;
	//用户名
	CStringA tmp1;
	tmp1 = name;
	tmpName = tmp1.GetBuffer();
	//密码
	CStringA tmp2;
	tmp2 = m_newPwd;
	tmpPwd = tmp2.GetBuffer();

	file.WritePwd(tmpName, tmpPwd); //修改密码

	MessageBox(TEXT("密码修改成功"));

	//输入框内容清空
	m_surePwd.Empty();
	m_newPwd.Empty();
	UpdateData(FALSE); //把数据更新到控件上
	
}


void CUserDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//输入框内容清空
	m_surePwd.Empty();
	m_newPwd.Empty();
	UpdateData(FALSE); //把数据更新到控件上

}
