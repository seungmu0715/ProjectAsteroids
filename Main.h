#pragma once
#include "GameController.h"
class Main
{
private:
	bool InitializeWindow(HINSTANCE hInstance);

	GameController*		m_GameController;

	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	LPCSTR		m_ApplicationName;

public:
	Main();
	~Main();

	bool Initialize(HINSTANCE hInstance);
	void Run();
	void Release();

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static Main* applicationHandle = 0;

