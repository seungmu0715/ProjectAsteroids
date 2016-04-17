#include "pch.h"
#include "GameController.h"


bool GameController::CreateLevels()
{
	m_Levels[0] = new MenuLevel();
	if (!m_Levels[0])
		return false;

	m_Levels[1] = new Level1();
	if (!m_Levels[1])
		return false;

	return true;
}

GameController::GameController()
{
	m_Direct2DManager = nullptr;

	for (int i = 0; i < ARRAYSIZE(m_Levels); i++)
	{
		m_Levels[i] = nullptr;
		m_IsLevelLoaded[i] = false;
	}

	m_Loading = true;
	m_CurrentLevel = 0;
}

bool GameController::Initialize(HWND hWnd)
{
	bool result;

	GetClientRect(hWnd, &m_Rect);

	m_Direct2DManager = new Direct2DManager();
	if (!m_Direct2DManager)
		return false;

	result = m_Direct2DManager->Initialize(hWnd);
	if (!result)
		return false;

	result = CreateLevels();
	if (!result)
		return false;

	result = LoadMenuLevel();
	if (!result)
		return false;

	return true;
}

void GameController::Render()
{
	m_Direct2DManager->BeginDraw();
	m_Direct2DManager->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	if (!m_Loading)
		m_Levels[m_CurrentLevel]->Render();

	m_Direct2DManager->EndDraw();
}

void GameController::Shutdown()
{
	for (int i = 0; i < ARRAYSIZE(m_Levels); i++)
	{
		if (m_Levels[i] != nullptr)
		{
			m_Levels[i]->UnLoad();
			delete m_Levels[i];
			m_Levels[i] = nullptr;
		}
	}

	if (m_Direct2DManager)
	{
		m_Direct2DManager->Shutdown();
		delete m_Direct2DManager;
		m_Direct2DManager = nullptr;
	}
}

bool GameController::LoadMenuLevel()
{
	bool result;

	if (!m_IsLevelLoaded[0])
	{
		m_Loading = true;
		result = m_Levels[0]->Load(m_Direct2DManager->GetRenderTarget(), m_Rect, std::bind(&GameController::LoadMenuLevel, this), std::bind(&GameController::LoadNextLevel, this));
		m_Loading = false;

		if (!result)
			return false;
		
		if (m_IsLevelLoaded[m_CurrentLevel] == true)
		{
			if (m_Levels[m_CurrentLevel] != nullptr)
			{
				m_Levels[m_CurrentLevel]->UnLoad();
				m_IsLevelLoaded[m_CurrentLevel] = false;
			}
		}

		m_CurrentLevel = 0;
		m_IsLevelLoaded[m_CurrentLevel] = true;
	}

	return m_IsLevelLoaded[0];
}

bool GameController::LoadNextLevel()
{
	bool result;

	m_Loading = true;
	result = m_Levels[m_CurrentLevel + 1]->Load(m_Direct2DManager->GetRenderTarget(), m_Rect, std::bind(&GameController::LoadMenuLevel, this), std::bind(&GameController::LoadNextLevel, this));
	m_Loading = false;

	if (!result)
		return false;

	if (m_IsLevelLoaded[m_CurrentLevel] == true)
	{
		if (m_Levels[m_CurrentLevel] != nullptr)
		{
			m_Levels[m_CurrentLevel]->UnLoad();
			m_IsLevelLoaded[m_CurrentLevel] = false;
		}
	}

	m_IsLevelLoaded[++m_CurrentLevel] = true;

	return m_Levels[m_CurrentLevel];
}

void GameController::OnMouseMessage(HWND hWnd, UINT message, LPARAM lParam)
{
	// 공통 명령 처리
	switch (message)
	{
	case WM_MOUSEMOVE:
		m_MouseX = GET_X_LPARAM(lParam);
		m_MouseY = GET_Y_LPARAM(lParam);

		////////////////////
		//	debug
		////////////////////
		char buffer[64];
		wsprintf(buffer, TEXT("%d, %d"), m_MouseX, m_MouseY);
		SetWindowText(hWnd, buffer);
		////////////////////
		//	debug
		////////////////////

		break;
	}

	// 특정 레벨에 따른 특수 명령 처리
	m_Levels[m_CurrentLevel]->OnMouseMessage(message, lParam);
}

void GameController::OnKeyboardMessage(HWND hWnd, WPARAM wParam)
{
	// 공통 명령 처리

	// 특정 레벨에 따른 특수 명령 처리
	m_Levels[m_CurrentLevel]->OnKeyboardMessage(wParam);
}

