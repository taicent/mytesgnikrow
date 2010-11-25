#include "stdafx.h"
#include "Label.h"

CLabel::CLabel(void)
{
	//m_LeftEye = POINT(0,0);
	//m_RightEye = POINT(0,0);
	//m_CenterMouth = POINT(0,0);
}

bool CLabel::IsInsideRect(CRect rect)
{
	if(!rect.PtInRect(m_LeftEye) || !rect.PtInRect(m_RightEye) || !rect.PtInRect(m_CenterMouth))
	{
		return false;
	}
	return true;
}

