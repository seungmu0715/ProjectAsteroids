#pragma once
#include <math.h>
#include "Tail.h"
class Loop : public Tail
{
private:
	D2D1_POINT_2F m_Destination;
	float m_DeltaX, m_DeltaY;

public:
	void Initialize(D2D1_POINT_2F position);
	virtual bool Update(DWORD delta);

	void SetDestination(D2D1_POINT_2F destination);
};

