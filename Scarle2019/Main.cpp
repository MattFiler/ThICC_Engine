//
// Main.cpp
//

#include "pch.h"
#include "Game.h"

using namespace DirectX;

namespace
{
	std::unique_ptr<Game> g_game;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _lpCmdLine, _In_ int _nCmdShow)
{

	//#ifdef _DEBUG
		//add a Debug Console to which cout is directed whilst in DEBUG
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	//#endif

	UNREFERENCED_PARAMETER(_hPrevInstance);
	UNREFERENCED_PARAMETER(_lpCmdLine);

	if (!XMVerifyCPUSupport())
		return 1;

	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))
		return 1;

	g_game = std::make_unique<Game>();

	// Register class and create window
	{
		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hInstance;
		wcex.hIcon = LoadIcon(_hInstance, L"IDI_ICON");
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"Scarle2019WindowClass";
		wcex.hIconSm = LoadIcon(wcex.hInstance, L"IDI_ICON");
		if (!RegisterClassEx(&wcex))
			return 1;

		// Create window
		int w, h;
		g_game->GetDefaultSize(w, h);

		RECT rc;
		rc.top = 0;
		rc.left = 0;
		rc.right = static_cast<LONG>(w);
		rc.bottom = static_cast<LONG>(h);

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		HWND hwnd =
#ifdef _DEBUG
			CreateWindowEx(0, L"Scarle2019WindowClass", L"Scarle2019", WS_OVERLAPPEDWINDOW,
#else
				CreateWindowEx(WS_EX_TOPMOST, L"Scarle2019WindowClass", L"Scarle2019", WS_POPUP,
#endif
					CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, _hInstance,
					nullptr);

		if (!hwnd)
			return 1;
#ifdef _DEBUG
		ShowWindow(hwnd, _nCmdShow);
#else
		ShowWindow(hwnd, SW_SHOWMAXIMIZED);
#endif

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));

		GetClientRect(hwnd, &rc);

		g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);

	}

	// Main message loop
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_game->Tick();
		}
	}

	g_game.reset();

	CoUninitialize();

	return (int)msg.wParam;
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen =
#ifdef _DEBUG
		false;
#else
		true;
#endif

	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));

	switch (_message)
	{
	case WM_PAINT:
		if (s_in_sizemove && game)
		{
			game->Tick();
		}
		else
		{
			hdc = BeginPaint(_hWnd, &ps);
			EndPaint(_hWnd, &ps);
		}
		break;

	case WM_SIZE:
		if (_wParam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				if (!s_in_suspend && game)
					game->OnSuspending();
				s_in_suspend = true;
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			if (s_in_suspend && game)
				game->OnResuming();
			s_in_suspend = false;
		}
		else if (!s_in_sizemove && game)
		{
			game->OnWindowSizeChanged(LOWORD(_lParam), HIWORD(_lParam));
		}
		break;

	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (game)
		{
			RECT rc;
			GetClientRect(_hWnd, &rc);

			game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(_lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_ACTIVATEAPP:
		if (game)
		{
			if (_wParam)
			{
				game->OnActivated();
			}
			else
			{
				game->OnDeactivated();
			}
		}
		Keyboard::ProcessMessage(_message, _wParam, _lParam);
		Mouse::ProcessMessage(_message, _wParam, _lParam);
		break;

	case WM_POWERBROADCAST:
		switch (_wParam)
		{
		case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend && game)
				game->OnSuspending();
			s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				if (s_in_suspend && game)
					game->OnResuming();
				s_in_suspend = false;
			}
			return TRUE;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SYSKEYDOWN:
		if (_wParam == VK_RETURN && (_lParam & 0x60000000) == 0x20000000)
		{
			// Implements the classic ALT+ENTER fullscreen toggle
			if (s_fullscreen)
			{
				SetWindowLongPtr(_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowLongPtr(_hWnd, GWL_EXSTYLE, 0);

				int width = 800;
				int height = 600;
				if (game)
					game->GetDefaultSize(width, height);

				ShowWindow(_hWnd, SW_SHOWNORMAL);

				SetWindowPos(_hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowLongPtr(_hWnd, GWL_STYLE, 0);
				SetWindowLongPtr(_hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

				SetWindowPos(_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

				ShowWindow(_hWnd, SW_SHOWMAXIMIZED);
			}

			s_fullscreen = !s_fullscreen;
		}

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(_message, _wParam, _lParam);
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(_message, _wParam, _lParam);
		break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond
		// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
		return MAKELRESULT(0, MNC_CLOSE);
	}

	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}


// Exit helper
void ExitGame()
{
	PostQuitMessage(0);
}