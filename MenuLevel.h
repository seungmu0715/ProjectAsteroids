#pragma once
#include "GameLevel.h"
class MenuLevel : public GameLevel
{
private:
	bool InitializeGUI();

	GUIButton*	m_Btn[2];

	int			m_MouseX, m_MouseY;

	bool		m_GUIInitialized;

public:
	MenuLevel();

	virtual bool Load(HWND hWnd, ID2D1HwndRenderTarget * renderTarget, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel, ExitGame exitGame) override;
	virtual void UnLoad() override;
	virtual void Update(DWORD delta) override;
	virtual void Render() override;

	virtual void OnMouseMessage(UINT message, LPARAM lParam) override;
	virtual void OnKeyboardMessage(WPARAM wParam) override { ; };
	virtual void OnTimer() override { ; };
};

