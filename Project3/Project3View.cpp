
// Project3View.cpp: CProject3View 类的实现
//
#pragma pack(push, 16)
#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Project3.h"
#endif

#include "Project3Doc.h"
#include "Project3View.h"
#include "SetProjectionCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma pack(pop)

// CProject3View

IMPLEMENT_DYNCREATE(CProject3View, CView)

BEGIN_MESSAGE_MAP(CProject3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_PINGXING, &CProject3View::OnPingxing)
	ON_COMMAND(ID_TOUSHI, &CProject3View::OnToushi)
	ON_WM_CHAR()
	ON_COMMAND(IDD_TOUSHITOUYING, &CProject3View::OnIddToushitouying)
END_MESSAGE_MAP()

// CProject3View 构造/析构

CProject3View::CProject3View() noexcept
{
	// TODO: 在此处添加构造代码
	CenterPoint.x = 0;
	CenterPoint.y = 0;
	ProjectionCenter << 0, 0, 0;
	Eigen::RowVector4d PointA;
	Eigen::RowVector4d PointB;
	Eigen::RowVector4d PointC;
	Eigen::RowVector4d PointD;
	Eigen::RowVector4d PointE;
	Eigen::RowVector4d PointF;
	Eigen::RowVector4d PointG;
	Eigen::RowVector4d PointH;
	PointA << Scale, Scale, Scale, 1;
	PointB << Scale, -Scale, Scale, 1;
	PointC << -Scale, -Scale, Scale, 1;
	PointD << -Scale, Scale, Scale, 1;
	PointE << Scale, Scale, -Scale, 1;
	PointF << Scale, -Scale, -Scale, 1;
	PointG << -Scale, -Scale, -Scale, 1;
	PointH << -Scale, Scale, -Scale, 1;
	//12条边为：AB BC CD DA EF FG GH HE AE BF CG DH
	Points.push_back(PointA);
	Points.push_back(PointB);
	Points.push_back(PointC);
	Points.push_back(PointD);
	Points.push_back(PointE);
	Points.push_back(PointF);
	Points.push_back(PointG);
	Points.push_back(PointH);
	CPoint temp;
	temp.x = 0;
	temp.y = 0;
	for (int i = 0; i < 8; i++)
		Points2D.push_back(temp);
}

CProject3View::~CProject3View()
{
}

BOOL CProject3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CProject3View 绘图

void CProject3View::OnDraw(CDC* pDC)
{
	CProject3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (NeedDraw) {
		this->Invalidate(); //清屏
		this->UpdateWindow();
		GetClientRect(this->Rect);
		CenterPoint.x = Rect.Width() / 2;
		CenterPoint.y = Rect.Height() / 2;
		pDC->DPtoLP(&CenterPoint);
		pDC->SetWindowOrg(-CenterPoint.x, -CenterPoint.y); //设备原点为（Rect.Width ()/2, Rect.Height ()/2），逻辑原点为（0,0）
		if (ProjectionType == 0) {
			for (int i = 0; i < 8; i++) {
				Points2D[i] = GetParallelProjectionPoint(Points[i]);
			}
			//12条边为：AB BC CD DA EF FG GH HE AE BF CG DH
			this->DDALine(pDC, Points2D[0].x, Points2D[0].y, Points2D[1].x, Points2D[1].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[1].x, Points2D[1].y, Points2D[2].x, Points2D[2].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[2].x, Points2D[2].y, Points2D[3].x, Points2D[3].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[3].x, Points2D[3].y, Points2D[0].x, Points2D[0].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[4].x, Points2D[4].y, Points2D[5].x, Points2D[5].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[5].x, Points2D[5].y, Points2D[6].x, Points2D[6].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[6].x, Points2D[6].y, Points2D[7].x, Points2D[7].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[7].x, Points2D[7].y, Points2D[4].x, Points2D[4].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[0].x, Points2D[0].y, Points2D[4].x, Points2D[4].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[1].x, Points2D[1].y, Points2D[5].x, Points2D[5].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[2].x, Points2D[2].y, Points2D[6].x, Points2D[6].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[3].x, Points2D[3].y, Points2D[7].x, Points2D[7].y, RGB(r, g, b));
		}
		else if (ProjectionType == 1) {
			for (int i = 0; i < 8; i++) {
				Points2D[i] = GetPerspectiveProjectionPoint(Points[i]);
			}
			//12条边为：AB BC CD DA EF FG GH HE AE BF CG DH
			this->DDALine(pDC, Points2D[0].x, Points2D[0].y, Points2D[1].x, Points2D[1].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[1].x, Points2D[1].y, Points2D[2].x, Points2D[2].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[2].x, Points2D[2].y, Points2D[3].x, Points2D[3].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[3].x, Points2D[3].y, Points2D[0].x, Points2D[0].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[4].x, Points2D[4].y, Points2D[5].x, Points2D[5].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[5].x, Points2D[5].y, Points2D[6].x, Points2D[6].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[6].x, Points2D[6].y, Points2D[7].x, Points2D[7].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[7].x, Points2D[7].y, Points2D[4].x, Points2D[4].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[0].x, Points2D[0].y, Points2D[6].x, Points2D[6].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[1].x, Points2D[1].y, Points2D[7].x, Points2D[7].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[2].x, Points2D[2].y, Points2D[4].x, Points2D[4].y, RGB(r, g, b));
			this->DDALine(pDC, Points2D[3].x, Points2D[3].y, Points2D[5].x, Points2D[5].y, RGB(r, g, b));
		}
		//DrawEdges(pDC);
		NeedDraw = false;

	}
}

CPoint CProject3View::GetParallelProjectionPoint(Eigen::RowVector4d Point)
{
	Eigen::Matrix4d op;
	op << 1, 0, 0, 0,
		0, 1, 0, 0,
		-0.354, -0.354, 0, 0,
		0, 0, 0, 1;
	CPoint result;
	Point = Point * op;
	result.x = (int)Point[0];
	result.y = (int)Point[1];
	return result;
}

CPoint CProject3View::GetPerspectiveProjectionPoint(Eigen::RowVector4d Point){
	Eigen::Matrix4d op;
	op << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 1 / PerspectiveProjection,
		0, 0, 0, 0;
	Point = Point * op;
	CPoint result;
	result.x = Point[0] / (Point[2] / PerspectiveProjection);
	result.y = Point[1] / (Point[2] / PerspectiveProjection);
	return result;
}

void CProject3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_RIGHT) {
		for (int i = 0; i< 8 ; i++)
			translation(Points[i], TranslationStep, 0, 0);
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == VK_LEFT) {
		for (int i = 0; i < 8; i++)
			translation(Points[i], -TranslationStep, 0, 0);
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == VK_UP) {
		for (int i = 0; i < 8; i++)
			translation(Points[i], 0, -TranslationStep, 0);
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == VK_DOWN) {
		for(int i = 0; i < 8; i++)
			translation(Points[i], 0, TranslationStep, 0);
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CProject3View::OnPingxing() //斜二测投影
{
	ProjectionType = 0;
	NeedDraw = true;
	this->Invalidate(); //清屏
	this->UpdateWindow();
}

void CProject3View::OnToushi() 
{
	ProjectionType = 1;
	SetProjectionCenter dia;
	dia.x = ProjectionCenter[0];
	dia.y = ProjectionCenter[1];
	dia.z = ProjectionCenter[2];
	if (dia.DoModal() == IDOK) {
		ProjectionCenter << dia.x, dia.y, dia.z;
	}
	NeedDraw = true;
	this->Invalidate(); //清屏
	this->UpdateWindow();
}


void CProject3View::DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	double dx, dy, e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx /= e;
	dy /= e;
	x = x1;
	y = y1;
	for (int i = 1; i <= e; i++)
	{
		pDC->SetPixel((int)(x + 0.5), (int)(y + 0.5), color);
		x += dx;
		y += dy;
	}
}

void CProject3View::DrawEdges(CDC* pDC) 	//12条边为：AB BC CD DA EF FG GH HE AE BF CG DH
{
	this->DDALine(pDC, Points2D[0].x, Points2D[0].y, Points2D[1].x, Points2D[1].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[1].x, Points2D[1].y, Points2D[2].x, Points2D[2].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[2].x, Points2D[2].y, Points2D[3].x, Points2D[3].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[3].x, Points2D[3].y, Points2D[0].x, Points2D[0].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[4].x, Points2D[4].y, Points2D[5].x, Points2D[5].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[5].x, Points2D[5].y, Points2D[6].x, Points2D[6].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[6].x, Points2D[6].y, Points2D[7].x, Points2D[7].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[7].x, Points2D[7].y, Points2D[4].x, Points2D[4].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[0].x, Points2D[0].y, Points2D[4].x, Points2D[4].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[1].x, Points2D[1].y, Points2D[5].x, Points2D[5].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[2].x, Points2D[2].y, Points2D[6].x, Points2D[6].y, RGB(r, g, b));
	this->DDALine(pDC, Points2D[3].x, Points2D[3].y, Points2D[7].x, Points2D[7].y, RGB(r, g, b));
}

void CProject3View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'z') {
		for (int i = 0; i < 8; i++) {
			rotateZ(Points[i], RotateDegree);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();

	}
	else if (nChar == 'x') {
		for (int i = 0; i < 8; i++) {
			rotateX(Points[i], RotateDegree);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == 'c') {
		for (int i = 0; i < 8; i++) {
			rotateY(Points[i], RotateDegree);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == 'a') {
		for (int i = 0; i < 8; i++) {
			rotateZ(Points[i], -RotateDegree);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();

	}
	else if (nChar == 's') {
		for (int i = 0; i < 8; i++) {
			rotateX(Points[i], -RotateDegree);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == 'd') {
		for (int i = 0; i < 8; i++) {
			rotateY(Points[i], -RotateDegree);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == 'i') {
		for (int i = 0; i < 8; i++) {
			scaleTransformation(Points[i], Zoomin);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == 'k') {
		for (int i = 0; i < 8; i++) {
			scaleTransformation(Points[i], Zoomout);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == 'h') {
		for (int i = 0; i < 8; i++) {
			translation(Points[i], 0, 0, -TranslationStep);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}
	else if (nChar == 'n') {
		for (int i = 0; i < 8; i++) {
			translation(Points[i], 0, 0, TranslationStep);
		}
		this->Invalidate(); //清屏
		this->UpdateWindow();
	}

	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CProject3View::OnIddToushitouying()
{
	
}

void CProject3View::translation(Eigen::RowVector4d & Point, int dx, int dy, int dz) //平移
{
	Eigen::Matrix4d op;
	op << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		dx, dy, dz, 1;
	Point = Point * op;
	NeedDraw = true;
}

void CProject3View::rotateX(Eigen::RowVector4d & Point, double theta) {
	Eigen::Matrix4d op;
	op << 1, 0, 0, 0,
		0, cos(theta), sin(theta), 0,
		0, -sin(theta), cos(theta), 0,
		0, 0, 0, 1;
	Point = Point * op;
	NeedDraw = true;
}

void CProject3View::rotateY(Eigen::RowVector4d & Point, double theta) {
	Eigen::Matrix4d op;
	op << cos(theta), 0, -sin(theta), 0,
		0, 1, 0, 0,
		sin(theta), 0, -cos(theta), 0,
		0, 0, 0, 1;
	Point = Point * op;
	NeedDraw = true;
}

void CProject3View::rotateZ(Eigen::RowVector4d & Point, double theta) {
	Eigen::Matrix4d op;
	op << cos(theta), sin(theta), 0, 0,
		-sin(theta), cos(theta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
	Point = Point * op;
	NeedDraw = true;
}

void CProject3View::scaleTransformation(Eigen::RowVector4d & Point, double rate) {
	Eigen::Matrix4d op;
	op << rate, 0, 0, 0,
		0, rate, 0, 0,
		0, 0, rate, 0,
		0, 0, 0, 1;
	Point = Point * op;
	NeedDraw = true;
}

// CProject3View 打印

BOOL CProject3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CProject3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CProject3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CProject3View 诊断

#ifdef _DEBUG
void CProject3View::AssertValid() const
{
	CView::AssertValid();
}

void CProject3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProject3Doc* CProject3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProject3Doc)));
	return (CProject3Doc*)m_pDocument;
}
#endif //_DEBUG
