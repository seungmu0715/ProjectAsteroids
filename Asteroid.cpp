#include "pch.h"
#include "Asteroid.h"


void Asteroid::Initialize(Sprite * sprite, int fromX, int fromY, int toX, int toY, float size)
{
	m_Sprite = sprite;

	m_PosX = fromX;
	m_PosY = fromY;
	m_DeltaX = (toX - fromX) / Util::GetDistance(fromX, fromY, toX, toY);
	m_DeltaY = (toY - fromY) / Util::GetDistance(fromX, fromY, toX, toY);
	m_Size = size;
	m_Radius = IMAGE_SIZE * m_Size * 0.5f;
	m_IsDying = false;
	m_IsDead = false;

	m_Frame = 0;
}

void Asteroid::Update(DWORD delta)
{
	m_PosX += (m_DeltaX / 10) * delta;
	m_PosY += (m_DeltaY / 10) * delta;

	if ((SCREEN_WIDTH + IMAGE_SIZE < m_PosX || m_PosX < -IMAGE_SIZE) || (SCREEN_HEIGHT + IMAGE_SIZE < m_PosY || m_PosY < -IMAGE_SIZE))
		SetDead();
}

void Asteroid::Render()
{
	int frame = (m_Frame++ / 4) % 4;

	if (!m_IsDying)
		m_Sprite->Draw(m_PosX, m_PosY, m_Size, 0, 0, 1.0f);
	else
	{
		if (!m_IsDead)
		{
			if (frame == 3)
				m_IsDead = true;

			m_Sprite->Draw(m_PosX, m_PosY, m_Size, 1, frame, 1.0f);
		}
	}
}

void Asteroid::SetDead()
{
	if (!m_IsDying)
	{
		m_Frame = 0;
		m_IsDying = true;
	}
}
