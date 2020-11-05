#include "WinInclude.h"
#include "Engine.h"
#include "Input.h"
#include "InputMapper.h"
#include "Game.h"
#include <DbgHelp.h>
#include <strsafe.h>
#include "EngineException.h"

#ifdef _DEBUG
#pragma comment(lib, "Game_Debug.lib")
#pragma comment(lib, "dbghelp.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Game_Release.lib")
#pragma comment(lib, "dbghelp.lib")
#endif // NDEBUG

using namespace CommonUtilities;

#ifdef _DEBUG
#define USE_CONSOLE_COMMAND
#endif
void InitConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 6031 )
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
#pragma warning( pop )
}

void CloseConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
	fclose(stdin); 
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
}

int CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers, std::wstring &aSubPath)
{
	BOOL bMiniDumpSuccessful;
	WCHAR szPath[MAX_PATH] = L"Crashes";
	WCHAR szSubPath[MAX_PATH];
	WCHAR szFileName[MAX_PATH];
	//const WCHAR* szAppName = L"Crashes";
	const WCHAR* szName = L"MiniDump";
	//DWORD dwBufferSize = MAX_PATH;
	HANDLE hDumpFile;
	SYSTEMTIME stLocalTime;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	GetLocalTime(&stLocalTime);
	//GetTempPath(dwBufferSize, szPath);

	StringCchPrintf(szFileName, MAX_PATH, L"%s", szPath);
	CreateDirectory(szFileName, NULL);

	StringCchPrintf(szSubPath, MAX_PATH, L"%s\\%s_%02d%02d_%02d%02d_ThreadID%ld",
		szPath, L"Crash", stLocalTime.wMonth, stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, GetCurrentThreadId());
	CreateDirectory(szSubPath, NULL);

	StringCchPrintf(szFileName, MAX_PATH, L"%s\\%s.dmp", szSubPath, szName);

	hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = someExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

	OutputDebugStringW(szPath);

	aSubPath = szSubPath;
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP, std::wstring &aSubPath)
{
	CreateMiniDump(aExceptionP, aSubPath);
	return EXCEPTION_EXECUTE_HANDLER;
}

void RunGame(CEngine &anEngine) {
	CWindowHandler::SWindowData windowData;
	windowData.myX = 100;
	windowData.myY = 100;
	windowData.myWidth = 1280;
	windowData.myHeight = 720;

	bool shouldRun = anEngine.Init(windowData);
	if (!shouldRun)
		return;

	CGame game;
	game.myWindowWidth = anEngine.GetWindowHandler()->GetWidth();
	game.myWindowHeight = anEngine.GetWindowHandler()->GetHeight();
	game.Init();

	MSG windowMessage = { 0 };
	while (shouldRun)
	{
		while (PeekMessage(&windowMessage, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&windowMessage);
			DispatchMessage(&windowMessage);

			if (windowMessage.message == WM_QUIT)
			{
				shouldRun = false;
			}
		}

		anEngine.BeginFrame();
		game.Update();
		anEngine.RenderFrame();
		anEngine.EndFrame();
		CInputMapper::GetInstance()->Update();
	}
}
	CEngine engine;
	std::wstring subPath = L"";

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{

	hInstance;
	hPrevInstance;
	lpCmdLine;
	nShowCmd;

#ifdef USE_CONSOLE_COMMAND
	InitConsole();
#endif

	__try {
		RunGame(engine);
	}
	__except (ExceptionFilterFunction(GetExceptionInformation(), subPath)) 
	{
		engine.ScreenShot(subPath);
		ENGINE_ERROR_BOOL_MESSAGE(false, "Program crashed! A minidump was created at Bin/Crashes, please tell a programmer.");
	}

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif
	return 0;
}