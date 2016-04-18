#include "pch.h"
#include "Player.h"


void Player::Initialize(ID2D1HwndRenderTarget* renderTarget, Sprite * sprite)
{
	m_RenderTarget = renderTarget;
	m_Sprite = sprite;
	
	m_ReservX = SCREEN_WIDTH / 2;
	m_ReservY = SCREEN_HEIGHT / 2;
	m_Duration = 1000;
	m_Radius = IMAGE_SIZE * 0.5f * 0.5f;
	m_IsDead = false;
	m_IsDead = false;

	m_Frame = 0;

	m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_Brush);
}

void Player::Update(DWORD delta)
{
	// Path의 수명을 소모하게 한다. 수명이 다 한 Path는 삭제한다
	for (std::vector<Path*>::iterator itr = m_Path.begin(); itr != m_Path.end();)
	{
		if ((*itr)->Update(delta))
		{
			delete *itr;
			itr = m_Path.erase(itr);
		}
		else
			itr++;
	}

	// Loop의 고리를 조여들게 한다. 끝까지 조여진 Loop는 삭제한다
	for (std::vector<Loop*>::iterator itr = m_Loop.begin(); itr != m_Loop.end();)
	{
		if ((*itr)->Update(delta))
		{
			delete *itr;
			itr = m_Loop.erase(itr);
		}
		else
			itr++;
	}

	if (m_IsDead)
		return;

	// 마우스 위치가 바뀌었다면 새로운 Path를 생성한다
	if (m_PosX != m_ReservX || m_PosY != m_ReservY)
	{
		Path* path = new Path();
		path->Initialize(D2D1::Point2F((float)m_ReservX, (float)m_ReservY), m_Duration);
		m_Path.push_back(path);
	}

	m_PosX = m_ReservX;
	m_PosY = m_ReservY;

	// 현재 Path의 길이가 4 이상 (3 이하만으로는 고리를 이룰 수 없다), 현재 Loop가 없을 경우
	// 고리 형태를 이루는 Path가 존재하는지 검사하고 생성한다
	if (m_Path.size() > 3 && m_Loop.size() == 0)
	{
		bool found = false;
		bool nConstant = false;
		float x, y;					// 교점 좌표

		float angle_n;
		// (Pn, Pn-1)로 이루어진 선분을 구함
		D2D1_POINT_2F Pn = m_Path.at(m_Path.size() - 1)->GetPosition();		// n번째 점
		D2D1_POINT_2F Pn_1 = m_Path.at(m_Path.size() - 2)->GetPosition();	// n-1번재 점
		if (Pn.x - Pn_1.x == 0)
		{
			nConstant = true;
			x = Pn.x;
		}
		else
			angle_n = (Pn.y - Pn_1.y) / (Pn.x - Pn_1.x);

		std::vector<Path*>::reverse_iterator itr = m_Path.rbegin();		// n
		itr = std::next(itr);											// n-1
		itr = std::next(itr);											// n-2

		D2D1_POINT_2F Pk, Pk_1;
		float angle_k;
		float minX, minY, maxX, maxY;

		for (; itr != std::prev(m_Path.rend()); itr++)								// n-2 부터 0까지
		{
			bool kConstant = false;

			Pk = (*itr)->GetPosition();
			Pk_1 = (*std::next(itr))->GetPosition();
			if (Pk.x - Pk_1.x == 0)
			{
				if (nConstant)					// 평행이거나 일치
					continue;
				else
				{
					kConstant = true;
					x = Pk.x;
				}
			}
			else
				angle_k = (Pk.y - Pk_1.y) / (Pk.x - Pk_1.x);
			
			if (!nConstant && !kConstant)
			{
				if (angle_n - angle_k == 0)		// 평행이거나 일치
					continue;

				// 교점 좌표 계산
				x = ((angle_n * Pn_1.x) - (angle_k * Pk_1.x) - Pn_1.y + Pk_1.y) / (angle_n - angle_k);
				y = angle_n * (x - Pn_1.x) + Pn_1.y;
			}
			else
			{
				if (nConstant)
					y = angle_k * (x - Pk_1.x) + Pk_1.y;
				else
					y = angle_n * (x - Pn_1.x) + Pn_1.y;
			}

			// 교점이 유효 범위인지 검사
			minX = Util::GetMax(Util::GetMin(Pn.x, Pn_1.x), Util::GetMin(Pk.x, Pk_1.x));
			maxX = Util::GetMin(Util::GetMax(Pn.x, Pn_1.x), Util::GetMax(Pk.x, Pk_1.x));

			minY = Util::GetMax(Util::GetMin(Pn.y, Pn_1.y), Util::GetMin(Pk.y, Pk_1.y));
			maxY = Util::GetMin(Util::GetMax(Pn.y, Pn_1.y), Util::GetMax(Pk.y, Pk_1.y));

			if ((minX <= x && x <= maxX) && (minY <= y && y <= maxY))
			{
				// 유효하다면 종료
				found = true;
				break;
			}
			else
				continue;
		}

		// 루프를 만듦
		if (found)
		{
			std::vector<Path*>::iterator tempItr;
			Loop* newLoop;

			float tempX, tempY;
			float sumX = 0, sumY = 0, count = 0;
			D2D1_POINT_2F position;

			// 고리를 이루는 Path의 좌표들을 Loop로 옮긴다
			for (; itr != m_Path.rbegin();)
			{
				position = (*itr)->GetPosition();
				tempX = position.x;
				tempY = position.y;

				sumX += tempX;
				sumY += tempY;
				count++;

				newLoop = new Loop();
				newLoop->Initialize(position);
				m_Loop.push_back(newLoop);

				delete *itr;
				tempItr = m_Path.erase(--itr.base());
				itr = std::vector<Path*>::reverse_iterator(++tempItr);
			}

			// 교점의 좌표를 Loop에 넣는다
			position.x = x;
			position.y = y;

			sumX += position.x;
			sumY += position.y;
			count++;

			newLoop = new Loop();
			newLoop->Initialize(position);
			m_Loop.push_back(newLoop);

			D2D1_POINT_2F destination;
			destination.x = sumX / count;
			destination.y = sumY / count;

			for (std::vector<Loop*>::iterator itr = m_Loop.begin(); itr != m_Loop.end(); itr++)
			{
				(*itr)->SetDestination(destination);
			}

			m_IsLoopValid = true;
		}
	}
}

void Player::Render()
{
	int frame;
	
	frame = (m_Frame++ / 4) % 4;

	for (std::vector<Path*>::iterator itr = m_Path.begin(); itr != m_Path.end() && std::next(itr) != m_Path.end(); itr++)
	{
		m_RenderTarget->DrawLine((*itr)->GetPosition(), (*std::next(itr))->GetPosition(), m_Brush);
	}

	for (std::vector<Loop*>::iterator itr = m_Loop.begin(); itr != m_Loop.end() && std::next(itr) != m_Loop.end(); itr++)
	{
		m_RenderTarget->DrawLine((*itr)->GetPosition(), (*std::next(itr))->GetPosition(), m_Brush);
	}

	// 닫힌 원을 만들기 위해 Loop의 마지막 원소부터 첫번째 원소로 이어지는 선을 추가로 그린다
	if (m_Loop.size() > 2)
		m_RenderTarget->DrawLine(m_Loop.back()->GetPosition(), m_Loop.front()->GetPosition(), m_Brush);

	if (!m_IsDying)
		m_Sprite->Draw(m_PosX, m_PosY, 0.5f, 0, frame, 1.0f);
	else
	{
		if (!m_IsDead)
		{
			if (frame == 3)
				m_IsDead = true;

			m_Sprite->Draw(m_PosX, m_PosY, 0.5f, 1, frame, 1.0f);
		}
	}
}

void Player::Shutdown()
{
	for (std::vector<Loop*>::iterator itr = m_Loop.begin(); itr != m_Loop.end();)
	{
		delete *itr;
		itr = m_Loop.erase(itr);
	}
	m_Loop.clear();

	for (std::vector<Path*>::iterator itr = m_Path.begin(); itr != m_Path.end();)
	{
		delete *itr;
		itr = m_Path.erase(itr);
	}
	m_Path.clear();
}

std::vector<Loop*>::iterator Player::GetLoopBegin()
{
	std::vector<Loop*>::iterator itr = m_Loop.begin();
	return itr;
}

std::vector<Loop*>::iterator Player::GetLoopEnd()
{
	std::vector<Loop*>::iterator itr = m_Loop.end();
	return itr;
}

void Player::SetPosition(int x, int y)
{
	m_ReservX = x;
	m_ReservY = y;
}

void Player::SetDead()
{
	if (!m_IsDying)
	{
		m_Frame = 0;
		m_IsDying = true;
	}
}
