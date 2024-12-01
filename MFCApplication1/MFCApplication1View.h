
// MFCApplication1View.h: CMFCApplication1View 클래스의 인터페이스
//

#pragma once
#include "MiniMap.h"
#include "Types.h"


typedef std::shared_ptr<Gdiplus::Bitmap> BitmapPtr;

class CMFCApplication1View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

// 특성입니다.
public:
	CMFCApplication1Doc* GetDocument() const;

// 작업입니다.
public:
	MiniMap m_minimap;
	BitmapPtr m_bitmap;
	CFRect m_bitmaprc;
	BitmapPtr m_curBmp;

	CFRect m_fViewMax;
	CFRect m_fViewrc;
	CFRect m_fPressingViewrc;
	CFPoint m_fMouseCurPos;

	CSize m_wndSize;
	

	bool m_mousePressed;
	CPoint m_mouseCurrentPos;
	CPoint m_mousePressedPos;
	UINT m_timerID;

	double m_dragBeginSpeed_X;
	double m_dragBeginSpeed_Y;
	CPoint m_dragBeginPos;
	std::chrono::system_clock::time_point m_dragBeginTime;
	std::chrono::system_clock::time_point m_OnTimerLastTime;
	double m_spdDragX;
	double m_spdDragY;
	
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	
protected:
	afx_msg LRESULT OnUwmGetbkground(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // MFCApplication1View.cpp의 디버그 버전
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

