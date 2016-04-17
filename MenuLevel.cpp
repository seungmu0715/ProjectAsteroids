#include "pch.h"
#include "MenuLevel.h"


MenuLevel::MenuLevel()
{
	m_RenderTarget = nullptr;
	m_ResourceManager = nullptr;
}

bool MenuLevel::Load(ID2D1HwndRenderTarget * renderTarget, RECT rect, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel)
{
	bool result;

	m_RenderTarget = renderTarget;
	m_Rect = rect;

	m_CallBackMenuLevel = std::move(loadMenuLevel);
	m_CallBackNextLevel = std::move(loadNextLevel);

	m_ResourceManager = new ResourceManager();
	if (!m_ResourceManager)
		return false;

	result = m_ResourceManager->LoadDirectory(renderTarget, L"res");
	if (!result)
		return false;

	return true;
}

void MenuLevel::UnLoad()
{
	if (m_ResourceManager)
	{
		m_ResourceManager->Shutdown();
		delete m_ResourceManager;
		m_ResourceManager = nullptr;
	}
}

void MenuLevel::Update(DWORD delta)
{
}

void MenuLevel::Render()
{
}

void MenuLevel::OnMouseMessage(UINT message, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		int m_MouseX = GET_X_LPARAM(lParam);
		int m_MouseY = GET_Y_LPARAM(lParam);

		if ((100 < m_MouseX && m_MouseX < 300) && (100 < m_MouseY && m_MouseY < 300))
		{
			m_CallBackNextLevel();
		}
		break;
	}
}
