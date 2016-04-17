#include "pch.h"
#include "Direct2DManager.h"


Direct2DManager::Direct2DManager()
{
	m_RenderTarget = nullptr;
}

bool Direct2DManager::Initialize(HWND hWnd)
{
	HRESULT result;
	ID2D1Factory2* factory;
	RECT rect;

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (FAILED(result))
		return false;

	GetClientRect(hWnd, &rect);

	result = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)), &m_RenderTarget);
	if (FAILED(result))
		return false;

	factory->Release();
	factory = nullptr;

	return true;
}

void Direct2DManager::Shutdown()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Release();
		m_RenderTarget = nullptr;
	}
}

void Direct2DManager::Clear(float r, float g, float b, float a)
{
	m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(r, g, b, a)));
}
