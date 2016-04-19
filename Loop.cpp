#include "pch.h"
#include "Loop.h"


void Loop::Initialize(D2D1_POINT_2F position)
{
	m_Position = position;
}

bool Loop::Update(DWORD delta)
{
	m_DeltaX = (m_Destination.x - m_Position.x);
	m_DeltaY = (m_Destination.y - m_Position.y);

	m_Position.x += m_DeltaX * delta / 200;
	m_Position.y += m_DeltaY * delta / 200;

	if (abs((int)(m_Destination.x - m_Position.x)) < IMAGE_SIZE / 4.0f && abs((int)(m_Destination.y - m_Position.y)) < IMAGE_SIZE / 4.0f)
		return true;

	return false;
}

void Loop::SetDestination(D2D1_POINT_2F destination)
{
	m_Destination = destination;
}
