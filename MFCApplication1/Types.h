#pragma once

class CFSize;
class CFPoint;
class CFRect;

class CFSize {
public:
	CFSize() :x(0), y(0) {}
	CFSize(double _x, double _y) :x(_x), y(_y) {}
	double x;
	double y;
};
class CFPoint {
public:
	CFPoint() :x(0), y(0) {}
	CFPoint(double _x, double _y) :x(_x), y(_y) {}
	double x;
	double y;
};
class CFRect {
public:
	double left;
	double top;
	double right;
	double bottom;
	CFRect(): left(0), right(0), top(0), bottom(0){
	}
	CFRect(double _left, double _top, double _right, double _bottom)
		: left(_left), right(_right), top(_top), bottom(_bottom) {
	}

	double Width() const {
		return right - left;
	}
	double Height() const {
		return bottom - top;
	}
	CFPoint CenterPoint() const {
		return CFPoint((left + right)/2.0, (top + bottom)/2.0);
	}
	CFRect(const CFRect& t) {
		this->left = t.left;
		this->right = t.right;
		this->top = t.top;
		this->bottom = t.bottom;
	}
	CFRect& operator=(const CFRect&t) {
		this->left = t.left;
		this->right = t.right;
		this->top = t.top;
		this->bottom = t.bottom;
		return *this;
	}
	void OffsetRect(double x, double y) {
		left += x;
		right += x;
		top += y;
		bottom += y;
	}
	void OffsetRect(CFSize ofs) {
		left += ofs.x;
		right += ofs.x;
		top += ofs.y;
		bottom += ofs.y;
	}
};
