#ifndef _IDOMRECT_H_
#define _IDOMRECT_H_

class IDOMRECT
{
protected:
	int m_iTop;
	int m_iLeft;
	int m_iRight;

public:
	IDOMRECT(int top, int left, int right):m_iTop(top), m_iLeft(left), m_iRight(right)
	{

	}

	int bottom()
	{
		return(m_iTop + height());
	}
	int top()
	{
		return(m_iTop);
	}
	int right()
	{
		return(m_iLeft + width());
	}
	int left()
	{
		return(m_iLeft);
	}


	virtual int height() = 0;
	virtual int width() = 0;
	/*virtual void add_element(element* el) = 0;
	virtual bool can_hold(element* el, white_space ws) = 0;
	virtual void finish(bool last_box = false) = 0;
	virtual bool is_empty() = 0;
	virtual int baseline() = 0;
	virtual void get_elements(elements_vector& els) = 0;
	virtual int top_margin() = 0;
	virtual int bottom_margin() = 0;
	virtual void y_shift(int shift) = 0;
	virtual void new_width(int left, int right, elements_vector& els) = 0;*/
};

#endif
