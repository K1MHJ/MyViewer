#pragma once

#include "Types.h"

// MiniMap
typedef std::shared_ptr<Gdiplus::Bitmap> BitmapPtr;
class MiniMap : public CWnd
{
	DECLARE_DYNAMIC(MiniMap)

public:
	MiniMap();
	virtual ~MiniMap();
	BitmapPtr m_bitmap;
	CFRect m_bitmaprc;

	CFSize m_fSize;
	CFPoint m_fCenter;

	CFRect m_fViewrc;
	CSize m_wndSize;

	CFRect m_fViewBox;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


