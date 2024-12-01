// MiniMap.cpp: 구현 파일
//
#include "pch.h"
#include "MFCApplication1.h"
#include "MiniMap.h"
#include "Resource.h"
// MiniMap
IMPLEMENT_DYNAMIC(MiniMap, CWnd)
MiniMap::MiniMap()
{

}
MiniMap::~MiniMap()
{
}

BEGIN_MESSAGE_MAP(MiniMap, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// MiniMap 메시지 처리기
int MiniMap::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_bitmap = BitmapPtr(new Bitmap(100, 100));
	// Graphics* g = Graphics::FromImage(m_bitmap.get());
	// SolidBrush brush(Color(255, 150, 0, 150));

	//int cellSize = 10;
	//int i, j, row;
	//row = 0;
	//i = 0;
	//for (; row < m_bitmap.get()->GetHeight(); ++i) {
	//	j = (i % 2 == 0) ? 0 : cellSize;
	//	for (; j < m_bitmap.get()->GetWidth(); j += 2 * cellSize) {
	//		g->FillRectangle(&brush, j, row, cellSize, cellSize);
	//	}
	//	row += cellSize;
	//}

	m_fViewBox = {0, 0, 0, 0};
	return 0;
}


void MiniMap::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bitmap.reset();
}


BOOL MiniMap::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;

	CRect Rect, ParentRect;
	GetClientRect(&Rect);
	GetParent()->GetClientRect(&ParentRect);

	CPoint ptLeftTop = CPoint(0, 0);
	ClientToScreen(&ptLeftTop);
	GetParent()->ScreenToClient(&ptLeftTop);

	CDC MemDC;
	CBitmap Bmp;

	MemDC.CreateCompatibleDC(NULL);
	Bmp.CreateBitmap(ParentRect.Width(), ParentRect.Height(),
		MemDC.GetDeviceCaps(PLANES),
		MemDC.GetDeviceCaps(BITSPIXEL), NULL);
	CBitmap* pOldBmp = MemDC.SelectObject(&Bmp);
	GetParent()->SendMessage(UWM_GETBKGROUND, (WPARAM)MemDC.m_hDC);
	pDC->BitBlt(0, 0, Rect.Width(), Rect.Height(), &MemDC, ptLeftTop.x, ptLeftTop.y, SRCCOPY);
	MemDC.SelectObject(pOldBmp);

	//Pen wndpen(Color(255, 0, 0, 0), 2);

	//Graphics memG(pDC->GetSafeHdc());
	//memG.DrawRectangle(&wndpen, 0, 0, 100, 100);

	return TRUE;
}


void MiniMap::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CDC* pDC;
	pDC = &dc;
	CRect rc;
	GetClientRect(rc);
	
	Bitmap memBmp(rc.Width(), rc.Height());
	Graphics memG(&memBmp);
	SolidBrush brs(Color(155, 255, 255, 255));
	Pen boxpen(Color(255, 255, 0, 0), 2);
	Pen wndpen(Color(255, 0, 0, 0), 2);
	Gdiplus::Graphics g(pDC->GetSafeHdc());
	CRect curRect;
	USES_CONVERSION;
	//memG.FillRectangle(&brs, 0, 0, rc.Width(), rc.Height());
	
	curRect.left = m_fViewrc.left;
	curRect.right = m_fViewrc.right;
	curRect.top = m_fViewrc.top;
	curRect.bottom = m_fViewrc.bottom;




	CFRect curFRect;
	CFRect overFRect;

	curFRect.left = m_fViewrc.left;
	curFRect.right = m_fViewrc.right;
	curFRect.top = m_fViewrc.top;
	curFRect.bottom = m_fViewrc.bottom;

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



	ImageAttributes attr;
	ColorMatrix cmat = {
		2.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Red
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // Green
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // Blue
		0.0f, 0.0f, 0.0f, 0.7f, 0.0f,   // Alpha (70%)
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f    // must be 1
	};
	attr.SetColorMatrix(&cmat);

	memG.DrawImage(m_bitmap.get(),
		toZoom,
		bmpOfsX_n, bmpOfsY_n,
		bmpW_n, bmpH_n,
		Gdiplus::UnitPixel, &attr, NULL, NULL);


	CRect rcbox;
	rcbox.left  = (m_fViewBox.left  - m_fViewrc.left) * m_wndSize.cx / m_fViewrc.Width();
	rcbox.right = (m_fViewBox.right - m_fViewrc.left) * m_wndSize.cx / m_fViewrc.Width();
	rcbox.top    = (m_fViewBox.top - m_fViewrc.top) * m_wndSize.cy / m_fViewrc.Height();
	rcbox.bottom = (m_fViewBox.bottom - m_fViewrc.top) * m_wndSize.cy / m_fViewrc.Height();

	memG.DrawRectangle(&wndpen, 0, 0, rc.Width(), rc.Height());
	memG.DrawRectangle(&boxpen, rcbox.left, rcbox.top, rcbox.Width(), rcbox.Height());
	
	g.DrawImage(&memBmp, 0, 0);
}


void MiniMap::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_wndSize.cx = cx;
	m_wndSize.cy = cy;
	if ((cx == 0) || (cy == 0)) {
		return;
	}

	double cr = (double)cx / cy;
	double r = m_fSize.x / m_fSize.y;

	if (cr > r) {
		double new_width = cr * m_fSize.y;
		double new_left = m_fCenter.x - new_width / 2;
		double new_right = m_fCenter.x + new_width / 2;
		m_fViewrc.left = new_left;
		m_fViewrc.right = new_right;
		m_fViewrc.top    = m_fSize.x / 2 * (-1.0);
		m_fViewrc.bottom = m_fSize.x / 2 * (1.0);
	}
	else {
		double new_height = m_fSize.x / cr;
		double new_top = m_fCenter.y - new_height / 2;
		double new_bottom = m_fCenter.y + new_height / 2;
		m_fViewrc.top = new_top;
		m_fViewrc.bottom = new_bottom;
		m_fViewrc.left  = m_fSize.y / 2 * (-1.0);
		m_fViewrc.right = m_fSize.y / 2 * (1.0);
	}

}
