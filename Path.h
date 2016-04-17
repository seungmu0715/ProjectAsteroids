#pragma once
#include "Tail.h"
class Path : public Tail
{
private:
	int				m_Lifetime;

public:
	void Initialize(D2D1_POINT_2F position, int duration);
	virtual bool Update(DWORD delta) override;
};

