// SetProjectionCenter.cpp: 实现文件
//

#include "stdafx.h"
#include "Project3.h"
#include "SetProjectionCenter.h"
#include "afxdialogex.h"


// SetProjectionCenter 对话框

IMPLEMENT_DYNAMIC(SetProjectionCenter, CDialog)

SetProjectionCenter::SetProjectionCenter(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TOUSHITOUYING, pParent)
	, x(0)
	, y(0)
	, z(0)
{

}

SetProjectionCenter::~SetProjectionCenter()
{
}

void SetProjectionCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, x);
	DDX_Text(pDX, IDC_EDIT_Y, y);
	DDX_Text(pDX, IDC_EDIT_Z, z);
}


BEGIN_MESSAGE_MAP(SetProjectionCenter, CDialog)
END_MESSAGE_MAP()


// SetProjectionCenter 消息处理程序
