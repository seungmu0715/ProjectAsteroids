#pragma once
#include "GameLevel.h"
class MenuLevel : public GameLevel
{
public:
	MenuLevel();

	virtual bool Load(ID2D1HwndRenderTarget* renderTarget, RECT rect, LoadMenuLevel loadMenuLevel, LoadNextLevel loadNextLevel) override;
	virtual void UnLoad() override;
	virtual void Update(DWORD delta) override;
	virtual void Render() override;

	virtual void OnMouseMessage(UINT message, LPARAM lParam) override;
	virtual void OnKeyboardMessage(WPARAM wParam) override { ; };
};

