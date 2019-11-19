#pragma once


// SetProjectionCenter 对话框

class SetProjectionCenter : public CDialog
{
	DECLARE_DYNAMIC(SetProjectionCenter)

public:
	SetProjectionCenter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SetProjectionCenter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOUSHITOUYING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double x;
	double y;
	double z;
};
