#include "pch.h"
#include "Sprite.h"


Sprite::Sprite()
{
	m_RenderTarget = nullptr;
	m_Bitmap = nullptr;
}

Sprite::Sprite(ID2D1HwndRenderTarget * renderTarget)
	: Sprite()
{
	m_RenderTarget = renderTarget;
}

void Sprite::Initialize(int numberOfFrame, int NumberOfAction)
{
	m_NumberOfFrame = numberOfFrame;
	m_NumberOfAction = NumberOfAction;
}

void Sprite::Shutdown()
{
	if (m_Bitmap)
	{
		m_Bitmap->Release();
		m_Bitmap = nullptr;
	}
}

void Sprite::Draw(int x, int y, float size, int actionIndex, int frameIndex, float opacity, float rotation)
{
	float left, top, right, bottom;
	left = IMAGE_SIZE * frameIndex;
	top = IMAGE_SIZE * actionIndex;
	right = left + IMAGE_SIZE - 1;
	bottom = top + IMAGE_SIZE - 1;

	m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F((float)x, (float)y)));

	x -= IMAGE_SIZE * size / 2;
	y -= IMAGE_SIZE * size / 2;
	
	m_RenderTarget->DrawBitmap(m_Bitmap,
		D2D1::RectF((FLOAT)x, (FLOAT)y, x + IMAGE_SIZE * size, y + IMAGE_SIZE * size),
		opacity,		// Oppacity
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(left, top, right, bottom)		// src image rect
		);

	m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
