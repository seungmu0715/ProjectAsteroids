#pragma once
#include <d2d1_2.h>
class Tail
{
protected:
	D2D1_POINT_2F	m_Position;

public:
	virtual bool Update(DWORD delta) = 0;

	D2D1_POINT_2F GetPosition() { return m_Position; };
};

