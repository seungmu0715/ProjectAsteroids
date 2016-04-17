#include "pch.h"
#include "Path.h"


void Path::Initialize(D2D1_POINT_2F position, int duration)
{
	m_Position = position;
	m_Lifetime = duration;
}

bool Path::Update(DWORD delta)
{
	m_Lifetime -= delta;

	if (m_Lifetime < 0)
	{
		return true;
	}

	return false;
}
