#pragma once

class CLabel
{
public:
	POINT m_LeftEye;
	POINT m_RightEye;
	POINT m_CenterMouth;

	CLabel(void);
	bool IsInsideRect(CRect rect);

};
