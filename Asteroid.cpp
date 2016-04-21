#include "pch.h"
#include "Asteroid.h"


void Asteroid::Initialize(Sprite * sprite, int type, int fromX, int fromY, int toX, int toY, float size)
{
	m_Sprite = sprite;
	m_Type = type;

	m_PosX = fromX;
	m_PosY = fromY;
	m_DeltaX = (float)((toX - fromX) / Util::GetDistance(fromX, fromY, toX, toY));
	m_DeltaY = (float)((toY - fromY) / Util::GetDistance(fromX, fromY, toX, toY));
	m_Size = size;
	m_Radius = IMAGE_SIZE * m_Size * 0.5f;
	m_Speed = 1.0f;
	m_IsDying = false;
	m_IsDead = false;

	m_Frame = 0;
	m_Rotation = rand() % 360;
}

void Asteroid::Update(DWORD delta)
{
	m_PosX += (int)((m_DeltaX / 10) * delta * m_Speed);
	m_PosY += (int)((m_DeltaY / 10) * delta * m_Speed);

	if (m_Rotation++ > 360 || m_Type != -1)
		m_Rotation = 0;

	if ((SCREEN_WIDTH + IMAGE_SIZE < m_PosX || m_PosX < -IMAGE_SIZE) || (SCREEN_HEIGHT + IMAGE_SIZE < m_PosY || m_PosY < -IMAGE_SIZE))
		SetDead();
}

void Asteroid::Render()
{
	int frame = (m_Frame++ / 4) % 4;

	if (!m_IsDying)
		m_Sprite->Draw(m_PosX, m_PosY, m_Size, 0, 0, 1.0f, m_Rotation);
	else
	{
		if (!m_IsDead)
		{
			if (frame == 3)
				m_IsDead = true;

			float modif = (frame / 10.0f) * 2;		// 0.0 ~ 0.6; 투명화 등 프레임에 따른 효과를 위함

			if (m_Type == -1)
				m_Sprite->Draw(m_PosX, m_PosY, m_Size, 1, frame, 1.0f - modif, m_Rotation);
			else
				m_Sprite->Draw(m_PosX, m_PosY, m_Size + modif, 0, 0, 1.0f - modif, m_Rotation);
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
