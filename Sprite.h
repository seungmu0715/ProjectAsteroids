#pragma once
#include <d2d1_2.h>
#include <wincodec.h>
#pragma comment (lib, "windowscodecs.lib")
class Sprite
{
private:
	ID2D1HwndRenderTarget*	m_RenderTarget;
	ID2D1Bitmap*			m_Bitmap;

	int						m_NumberOfFrame, m_NumberOfAction;

public:
	Sprite();
	Sprite(ID2D1HwndRenderTarget* renderTarget);

	void Initialize(int numberOfFrame, int NumberOfAction);
	void Shutdown();
	void Draw(int x, int y, float size, int actionIndex, int frameIndex, float opacity);
	ID2D1Bitmap** GetBitmap() { return &m_Bitmap; };
};

