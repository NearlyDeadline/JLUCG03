
// Project3View.h: CProject3View 类的接口
//

#pragma once
#pragma pack(push, 16)
#include <Eigen/Dense>
#include <vector>
#pragma pack(pop)
class CProject3View : public CView
{
protected: // 仅从序列化创建
	CProject3View() noexcept;
	DECLARE_DYNCREATE(CProject3View)

// 特性
public:
	CProject3Doc* GetDocument() const;

// 操作
public:

private:
	std::vector<Eigen::RowVector4d, Eigen::aligned_allocator<Eigen::RowVector4d>> Points;
	std::vector<CPoint> Points2D;
	int Scale = 100; //放大比例
	int TranslationStep = 10; //一次平移移动10个像素
	double Zoomin = 1.1; //一次放大为1.1倍
	double Zoomout = 0.9; //一次缩小为0.9倍
	int ProjectionType = 0; //0代表平行投影，1代表透视投影
	int PerspectiveProjection = 120; //透视投影的d
	int r = 0; //颜色
	int g = 0;
	int b = 0;
	Eigen::RowVector3d ProjectionCenter; //投影中心
	CRect Rect;
	CPoint CenterPoint; //原点
	bool NeedDraw = true; //Points内有数据改变，应同步至Points2D并画图
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CProject3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPingxing();
	afx_msg void OnToushi();
private:
	void translation(Eigen::RowVector4d & Point, int dx, int dy, int dz);
	void rotateX(Eigen::RowVector4d & Point, double theta);
	void rotateY(Eigen::RowVector4d & Point, double theta);
	void rotateZ(Eigen::RowVector4d & Point, double theta);
	void scaleTransformation(Eigen::RowVector4d & Point, double rate);
	double RotateDegree = 0.0174533; //15度的弧度
public:
	void DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);
private:
	void DrawEdges(CDC* pDC);
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnIddToushitouying();
private:
	CPoint GetParallelProjectionPoint(Eigen::RowVector4d Point);
	CPoint GetPerspectiveProjectionPoint(Eigen::RowVector4d Point);
};

#ifndef _DEBUG  // Project3View.cpp 中的调试版本
inline CProject3Doc* CProject3View::GetDocument() const
   { return reinterpret_cast<CProject3Doc*>(m_pDocument); }
#endif

