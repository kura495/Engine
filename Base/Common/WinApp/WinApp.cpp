#include"WinApp.h"

std::string WinApp::dropFileName;

WinApp* WinApp::GetInstance()
{
	static WinApp instance;
	return &instance;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	TCHAR filename[100];

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	// ファイルをドロップしたら
	case WM_CREATE:
		// ドロップ登録
		DragAcceptFiles(hwnd, TRUE);
		break;
		// ドロップされていたら
	case WM_DROPFILES:

		// ドロップされたファイル名を取得
		DragQueryFile((HDROP)wparam, 0, filename, MAX_PATH);
		WinApp::dropFileName = ConvertString(filename);

		// メモリを解放
		DragFinish((HDROP)wparam);
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::Initialize(){

	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"CG2WindowClass";
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&wc);
	RECT wrc = { 0,0,kClientWidth,kClientHeight };

	AdjustWindowRect(&wrc,WS_OVERLAPPEDWINDOW,false);

	hwnd_ = CreateWindow(
		wc.lpszClassName,
		L"LE2A_08_クラモト_アツシ",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
	ShowWindow(hwnd_, SW_SHOW);
}






