#include "pch.h"
#include "Main.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	bool result;

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			Main* main = nullptr;

			main = new Main();
			if (!main)
				return 1;

			result = main->Initialize(hInstance);
			if (!result)
				return 2;

			main->Run();
			main->Release();
			delete main;
			main = nullptr;
		}

		CoUninitialize();
	}

	return 0;
}

bool Main::InitializeWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass;

	m_hInstance = hInstance;
	applicationHandle = this;

	m_ApplicationName = TEXT("Engine");

	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = m_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = m_ApplicationName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(NULL, TEXT("Could not register Window Class."), TEXT("Error"), MB_OK);
		return false;
	}

	DWORD style = WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	if ((m_hWnd = CreateWindow(m_ApplicationName, m_ApplicationName, style, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, m_hInstance, NULL)) == NULL)
	{
		MessageBox(NULL, TEXT("Could not Create Window."), TEXT("Error"), MB_OK);
		UnregisterClass(m_ApplicationName, m_hInstance);
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}

Main::Main()
{
	m_GameController = nullptr;
}


Main::~Main()
{
}

bool Main::Initialize(HINSTANCE hInstance)
{
	bool result;

	result = InitializeWindow(hInstance);
	if (!result)
		return false;

	m_GameController = new GameController();
	if (!m_GameController)
		return false;

	result = m_GameController->Initialize(m_hWnd);
	if (!result)
		return false;

	return true;
}

void Main::Run()
{
	MSG message;
	static DWORD time = GetTickCount();

	ZeroMemory(&message, sizeof(message));

	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			DWORD currentTime = GetTickCount();
			DWORD delta = currentTime - time;
			time = currentTime;

			m_GameController->Update(delta);
			m_GameController->Render();
		}
	}
}

void Main::Release()
{
	if (m_GameController)
	{
		m_GameController->Shutdown();
		delete m_GameController;
		m_GameController = nullptr;
	}

	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	UnregisterClass(m_ApplicationName, m_hInstance);
	m_hInstance = nullptr;

	m_ApplicationName = nullptr;
}

LRESULT Main::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSELEAVE:
		m_GameController->OnMouseMessage(m_hWnd, message, lParam);
		break;
	case WM_KEYDOWN:
		m_GameController->OnKeyboardMessage(m_hWnd, wParam);
		break;
	case WM_PAINT:
	{
		HDC hdc;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd, &ps);

		m_GameController->Render();

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return applicationHandle->MessageHandler(hWnd, message, wParam, lParam);
}
