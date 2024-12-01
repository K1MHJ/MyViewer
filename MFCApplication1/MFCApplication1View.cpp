
// MFCApplication1View.cpp: CMFCApplication1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"
#include <string>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#define SMOOTH_DRAG
#ifdef SMOOTH_DRAG
#undef SMOOTH_DRAG
#endif

// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_MOUSEHOVER()
	ON_MESSAGE(UWM_GETBKGROUND, &CMFCApplication1View::OnUwmGetbkground)
END_MESSAGE_MAP()

// CMFCApplication1View 생성/소멸

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	
}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 그리기

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	
}


// CMFCApplication1View 인쇄

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCApplication1View 진단

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 메시지 처리기


int CMFCApplication1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_wndSize.cx = lpCreateStruct->cx;
	m_wndSize.cy = lpCreateStruct->cy;
	
	m_fViewMax = {-500, -500, 500, 500};
	m_minimap.m_fSize = { 1000, 1000 };
	m_minimap.m_fCenter = { 0, 0 };
	m_minimap.Create(NULL, NULL, WS_CHILDWINDOW, { 0,0,100,100 }, this, 0);
	m_minimap.ShowWindow(SW_SHOW);

	m_fViewrc.left = -500;
	m_fViewrc.top = -500;
	m_fViewrc.right = 500;
	m_fViewrc.bottom = 500;
	
	m_curBmp = BitmapPtr(new Bitmap(0, 0));
	m_bitmap = BitmapPtr(new Bitmap(1000, 1000));
	m_bitmaprc = {0,0,500,500};
	Gdiplus::Graphics* g = Gdiplus::Graphics::FromImage(m_bitmap.get());
	SolidBrush brush(Color(255, 150, 150, 150));

	int cellSize = 200;
	int i, j, row;
	row = 0;
	i = 0;
	for (; row < m_bitmap.get()->GetHeight(); ++i) {
		j = (i % 2 == 0) ? 0 : cellSize;
		for (; j < m_bitmap.get()->GetWidth(); j += 2 * cellSize) {
			g->FillRectangle(&brush, j, row, cellSize, cellSize);
		}
		row += cellSize;
	}

	Gdiplus::Rect toZoom(0, 0, m_minimap.m_bitmap.get()->GetWidth(), m_minimap.m_bitmap.get()->GetHeight());

	m_minimap.m_bitmaprc = m_bitmaprc;
	Gdiplus::Graphics* chg = Gdiplus::Graphics::FromImage(m_minimap.m_bitmap.get());
	
	chg->DrawImage(m_bitmap.get(),
		toZoom,
		0, 0,
		m_bitmap.get()->GetWidth() , m_bitmap.get()->GetHeight(),
		Gdiplus::UnitPixel);
	delete chg;
	return 0;
}


void CMFCApplication1View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bitmap.reset();
	m_curBmp.reset();
}


void CMFCApplication1View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.
	
	CDC *pDC;
	pDC = &dc;
	CRect rc;
	GetClientRect(rc);
	
	if (m_curBmp.get()->GetWidth() != rc.Width() || m_curBmp.get()->GetHeight() != rc.Height()) {
		m_curBmp.reset();
		m_curBmp = BitmapPtr(new Bitmap(rc.Width(), rc.Height()));
	}
	Bitmap memBmp(rc.Width(), rc.Height());
	Gdiplus::Graphics memG(&memBmp);
	SolidBrush brs(Color(255, 255, 255, 255));
	Gdiplus::Graphics g(pDC->GetSafeHdc());

	FontFamily  fontFamily(L"Times New Roman");
	Gdiplus::Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(0.0f, 0.0f);
	SolidBrush  solidBrush(Color(255, 0, 0, 255));
	std::string str;
	CRect curRect;
	CFRect curFRect;
	CFRect overFRect;
	USES_CONVERSION;

	memG.FillRectangle(&brs, 0, 0, rc.Width(), rc.Height());	

	if (m_mousePressed) {
		curFRect.left = m_fPressingViewrc.left;
		curFRect.right = m_fPressingViewrc.right;
		curFRect.top = m_fPressingViewrc.top;
		curFRect.bottom = m_fPressingViewrc.bottom;
	}
	else {
		curFRect.left = m_fViewrc.left;
		curFRect.right = m_fViewrc.right;
		curFRect.top = m_fViewrc.top;
		curFRect.bottom = m_fViewrc.bottom;
	}

	if (curFRect.right < m_bitmaprc.left || curFRect.left > m_bitmaprc.right ||
		curFRect.bottom < m_bitmaprc.top || curFRect.top > m_bitmaprc.bottom) {
		overFRect.top = max(curFRect.top, m_bitmaprc.top);
		overFRect.bottom = min(curFRect.bottom, m_bitmaprc.bottom);
		overFRect.left = max(curFRect.left, m_bitmaprc.left);
		overFRect.right = min(curFRect.right, m_bitmaprc.right);
	}
	else {
		overFRect.top = max(curFRect.top, m_bitmaprc.top);
		overFRect.bottom = min(curFRect.bottom, m_bitmaprc.bottom);
		overFRect.left = max(curFRect.left, m_bitmaprc.left);
		overFRect.right = min(curFRect.right, m_bitmaprc.right);
	}

	double bmpOfsX = overFRect.left - m_bitmaprc.left;
	double bmpOfsY = overFRect.top - m_bitmaprc.top;

	int bmpOfsX_n = bmpOfsX * m_bitmap.get()->GetWidth() / m_bitmaprc.Width();
	int bmpOfsY_n = bmpOfsY * m_bitmap.get()->GetHeight() / m_bitmaprc.Height();
	int bmpW_n = overFRect.Width() * m_bitmap.get()->GetWidth() / m_bitmaprc.Width();
	int bmpH_n = overFRect.Height() * m_bitmap.get()->GetHeight() / m_bitmaprc.Height();

	double rcOfsX = overFRect.left - curFRect.left;
	double rcOfsY = overFRect.top - curFRect.top;

	int rcOfsX_n = rcOfsX * rc.Width() / curFRect.Width();
	int rcOfsY_n = rcOfsY * rc.Height() / curFRect.Height();
	int rcW_n = overFRect.Width() * rc.Width() / curFRect.Width();
	int rcH_n = overFRect.Height() * rc.Height() / curFRect.Height();
	Gdiplus::Rect toZoom(rcOfsX_n, rcOfsY_n, rcW_n, rcH_n);

	memG.DrawImage(m_bitmap.get(),
		toZoom,
		bmpOfsX_n, bmpOfsY_n,
		bmpW_n, bmpH_n,
		Gdiplus::UnitPixel);

	/*
	str = Format("Left : %d Top : %d Width: %d Height : %d \n", curRect.left, curRect.top, curRect.Width(), curRect.Height());
	str.append(Format("Cur: %f ,%f\n", m_fMouseCurPos.x, m_fMouseCurPos.y));

#ifdef SMOOTH_DRAG
	str.append(Format("CurSpd: %f ,%f\n", m_spdDragX, m_spdDragY));
#endif

	memG.DrawString(A2W(str.c_str()), -1, &font, pointF, &solidBrush);
	*/

	Gdiplus::Graphics curmemG(m_curBmp.get());
	curmemG.DrawImage(&memBmp, 0, 0);

	g.DrawImage(&memBmp, 0, 0);
}


void CMFCApplication1View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (cx == 0 || cy == 0) {
		return;
	}
	m_wndSize.cx = cx;
	m_wndSize.cy = cy;
	m_minimap.MoveWindow(0, 0, 100, 100);

	double cr = (double)m_wndSize.cx / m_wndSize.cy;

	if (m_fViewrc.Height() * cr > m_fViewrc.Width()) {
		double t = m_fViewrc.CenterPoint().y - m_fViewrc.Width() / cr / 2.0;
		double b = m_fViewrc.CenterPoint().y + m_fViewrc.Width() / cr / 2.0;
		m_fViewrc.top = t;
		m_fViewrc.bottom = b;
	}
	else {
		double L = m_fViewrc.CenterPoint().x - m_fViewrc.Height() * cr / 2.0;
		double r = m_fViewrc.CenterPoint().x + m_fViewrc.Height() * cr / 2.0;
		m_fViewrc.left = L;
		m_fViewrc.right = r;
	}
	m_minimap.m_fViewBox = m_fViewrc;
}


BOOL CMFCApplication1View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	double scale;
	ScreenToClient(&pt);

	double cr = (double)m_wndSize.cx / m_wndSize.cy;
	double r = m_fViewMax.Width() / m_fViewMax.Height();

	int left_w = pt.x;
	int right_w = m_wndSize.cx - pt.x;
	int top_h = pt.y;
	int bottom_h = m_wndSize.cy - pt.y;

	double w = m_fViewrc.Width();
	double h = m_fViewrc.Height();

	CFPoint fPos;
	m_fMouseCurPos.x = fPos.x = m_fViewrc.left + m_fViewrc.Width() * pt.x / m_wndSize.cx;
	m_fMouseCurPos.y = fPos.y = m_fViewrc.top + m_fViewrc.Height() * pt.y / m_wndSize.cy;

	double next_width;
	double next_height;
	scale = (zDelta > 0) ? 1.5 : 0.8;
	next_width = scale * m_fViewrc.Width();
	next_height = scale * m_fViewrc.Height();

	double left_fw = (double)left_w / (left_w + right_w);
	double right_fw = (double)right_w / (left_w + right_w);
	double top_fw = (double)top_h / (top_h + bottom_h);
	double bottom_fw = (double)bottom_h / (top_h + bottom_h);

	double left = fPos.x - next_width * left_fw;
	double right = fPos.x + next_width * right_fw;
	double top = fPos.y - next_height * top_fw;
	double bottom = fPos.y + next_height * bottom_fw;

	if (cr > r) {
		if (left < m_fViewMax.left) {
			left = m_fViewMax.left;
			right = left + next_width;
			if (right > m_fViewMax.right) {
				right = m_fViewMax.right;
				next_height = m_fViewMax.Width() / cr;
				top = fPos.y - next_height * top_fw;
				bottom = fPos.y + next_height * bottom_fw;
			}
		}
	}
	else {
		if (top < m_fViewMax.top) {
			top = m_fViewMax.top;
			bottom = top + next_height;
			if (bottom > m_fViewMax.bottom) {
				bottom = m_fViewMax.bottom;
				next_width = m_fViewMax.Height() * cr;
				left = fPos.x - next_width * left_fw;
				right = fPos.x + next_width * right_fw;
			}
		}
	}
	m_fViewrc.left = left;
	m_fViewrc.right = right;
	m_fViewrc.top = top;
	m_fViewrc.bottom = bottom;
	m_minimap.m_fViewBox = m_fViewrc;


	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
void CALLBACK clock(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime)
{
	OutputDebugStringA("clock\n");
}
void CMFCApplication1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mousePressed = true;
	m_mousePressedPos = point;
#ifdef SMOOTH_DRAG
	m_dragBeginTime = std::chrono::system_clock::now();
	m_dragBeginPos = point;
	
	KillTimer(1);
#endif
	CView::OnLButtonDown(nFlags, point);
}


void CMFCApplication1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_mousePressed) {
		m_mousePressed = false;
		m_fViewrc = m_fPressingViewrc;
	}
#ifdef SMOOTH_DRAG
	m_OnTimerLastTime = m_dragBeginTime = std::chrono::system_clock::now();
	KillTimer(1);
	OutputDebugStringA(Format("UP %f, %f \n", m_dragBeginSpeed_X, m_dragBeginSpeed_Y).c_str());
	
	m_timerID = SetTimer(1, 33, NULL);
#endif
	CView::OnLButtonUp(nFlags, point);
}


void CMFCApplication1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	if (PtInRect(rect, point)) {
		// マウス監視開始(LEAVEイベント)
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = this->m_hWnd;
		_TrackMouseEvent(&tme);
	}

	m_mouseCurrentPos = point;

	m_fMouseCurPos.x = m_fViewrc.left + m_fViewrc.Width() * point.x / m_wndSize.cx;
	m_fMouseCurPos.y = m_fViewrc.top + m_fViewrc.Height() * point.y / m_wndSize.cy;

	if (m_mousePressed) {
		m_fMouseCurPos.x = m_fViewrc.left + m_fViewrc.Width() * m_mousePressedPos.x / m_wndSize.cx;
		m_fMouseCurPos.y = m_fViewrc.top + m_fViewrc.Height() * m_mousePressedPos.y / m_wndSize.cy;

		CSize d = m_mousePressedPos - m_mouseCurrentPos;
		m_fPressingViewrc = m_fViewrc;
		double w = m_fPressingViewrc.Width();
		double h = m_fPressingViewrc.Height();

		double fw = w / m_wndSize.cx * d.cx;
		double fh = h / m_wndSize.cy * d.cy;
		
		m_fPressingViewrc.OffsetRect(fw, fh);
		if (m_fPressingViewrc.left < m_fViewMax.left || m_fPressingViewrc.right > m_fViewMax.right) {
			if (m_fPressingViewrc.left < m_fViewMax.left) {
				m_fPressingViewrc.left = m_fViewMax.left;
				m_fPressingViewrc.right = m_fViewMax.left + w;
			}
			if (m_fPressingViewrc.right > m_fViewMax.right) {
				m_fPressingViewrc.left = m_fViewMax.right - w;
				m_fPressingViewrc.right = m_fViewMax.right;
			}
		}
		if (m_fPressingViewrc.top < m_fViewMax.top || m_fPressingViewrc.bottom > m_fViewMax.bottom) {
			if (m_fPressingViewrc.top < m_fViewMax.top) {
				m_fPressingViewrc.top = m_fViewMax.top;
				m_fPressingViewrc.bottom = m_fViewMax.top + h;
}
			if (m_fPressingViewrc.bottom > m_fViewMax.bottom) {
				m_fPressingViewrc.top = m_fViewMax.bottom - h;
				m_fPressingViewrc.bottom = m_fViewMax.bottom;
			}
		}
		
		m_minimap.m_fViewBox = m_fPressingViewrc;
#ifdef SMOOTH_DRAG
		auto now_t = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = now_t - m_dragBeginTime;

		//m_dragBeginSpeed_X = m_dragBeginSpeed_Y = 0;
		OutputDebugStringA(Format("Move Time: %f \n", elapsed_seconds.count()).c_str());
		if (elapsed_seconds.count() > 0.1) {
			// 100msec passed
			m_dragBeginTime = std::chrono::system_clock::now();
			auto dp = m_dragBeginPos - point;
			if (sqrt(pow((double)dp.cx, 2) + pow((double)dp.cy, 2)) < 50) {
				m_dragBeginSpeed_X = m_dragBeginSpeed_Y = 0;
				OutputDebugStringA(Format("50 > %f \n", sqrt(pow((double)dp.cx, 2) + pow((double)dp.cy, 2))).c_str());
			}
			else {
				OutputDebugStringA(Format("50 < %f \n", sqrt(pow((double)dp.cx, 2) + pow((double)dp.cy, 2))).c_str());

				m_dragBeginSpeed_X = dp.cx / elapsed_seconds.count(); // pxl / sec
				m_dragBeginSpeed_Y = dp.cy / elapsed_seconds.count(); // pxl / sec
			}
			m_dragBeginPos = point;
		}
		else {
			m_dragBeginSpeed_X = m_dragBeginSpeed_Y = 0;
		}
#endif
		Invalidate();
	}
	
	CView::OnMouseMove(nFlags, point);
}


void CMFCApplication1View::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_mousePressed) {
		m_mousePressed = false;
		m_fViewrc = m_fPressingViewrc;
	}
	CView::OnMouseLeave();
}


BOOL CMFCApplication1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//Gdiplus::Graphics g(pDC->GetSafeHdc());
	//Pen wndpen(Color(255, 0, 0, 0), 2);
	//g.DrawRectangle(&wndpen, 0, 0, 100, 100);
	return TRUE;
}
void CMFCApplication1View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
#ifdef SMOOTH_DRAG
	if (nIDEvent == 1) {
		double keep_time = 0.5;//sec
		double deacce_time = 1;//sec

		auto now_t = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = now_t - m_dragBeginTime;

		double spd_x;
		double spd_y;

		if (elapsed_seconds.count() < keep_time) {
			spd_x = m_dragBeginSpeed_X;
			spd_y = m_dragBeginSpeed_Y;
		}
		else {
			double dT = deacce_time + keep_time - elapsed_seconds.count();
			if (dT < 0.0) {
				KillTimer(nIDEvent);
				return;
			}
			spd_x = (elapsed_seconds.count() - keep_time) * m_dragBeginSpeed_X / deacce_time * (-1) + m_dragBeginSpeed_X;
			spd_y = (elapsed_seconds.count() - keep_time) * m_dragBeginSpeed_Y / deacce_time * (-1) + m_dragBeginSpeed_Y;
		}
		m_spdDragX = spd_x;
		m_spdDragY = spd_y;

		std::chrono::duration<double> elapsedTimer_seconds = now_t - m_OnTimerLastTime;
		m_OnTimerLastTime = std::chrono::system_clock::now();

		double dX = spd_x * elapsedTimer_seconds.count(); //pxl
		double dY = spd_y * elapsedTimer_seconds.count();
		
		double w = m_fViewrc.Width();
		double h = m_fViewrc.Height();

		double fw = w / m_wndSize.cx * dX;
		double fh = h / m_wndSize.cy * dY;

		m_fViewrc.OffsetRect(fw, fh);
		Invalidate(FALSE);
	}
#endif
	CView::OnTimer(nIDEvent);
}


void CMFCApplication1View::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseHover(nFlags, point);
}


void CMFCApplication1View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ModifyStyleEx(NULL, WS_CLIPCHILDREN);
}


afx_msg LRESULT CMFCApplication1View::OnUwmGetbkground(WPARAM wParam, LPARAM lParam)
{
	HDC hDC = (HDC)wParam;
	Gdiplus::Graphics g(hDC);
	g.DrawImage(m_curBmp.get(), 0, 0);
	return TRUE;
}
