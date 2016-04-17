#pragma once
#include <d2d1_2.h>
#pragma comment(lib, "d2d1.lib")
class Direct2DManager
{
private:
	ID2D1HwndRenderTarget*	m_RenderTarget;

public:
	Direct2DManager();

	bool Initialize(HWND hWnd);
	void Shutdown();

	void BeginDraw() { m_RenderTarget->BeginDraw(); };
	void Clear(float r, float g, float b, float a);
	void EndDraw() { m_RenderTarget->EndDraw(); };
	ID2D1HwndRenderTarget* GetRenderTarget() { return m_RenderTarget; };
};

