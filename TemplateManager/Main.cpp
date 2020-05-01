#include <windows.h>
#include <string>
#include "SettingsHandler.h"
#include "TimeClock.h"
#include "TemplateHandler.h"

#define IDC_MAIN_EDIT 101
#define ID_FILE_EXIT 9001
#define ID_ABOUT 9002
#define ID_HELP 9003
#define ID_OPEN_SETTINGS 9005
#define ID_EDIT_TEMPLATES 9006
#define ID_IN_PROGRESS 9020
#define ID_TEMPBASE 9050 //Nothing above this.


//Global Entities
HWND hMainWindow = { 0 };
const char g_szClassName[] = "myMainWindow";
const char g_WindowTitle[] = "Template Manager V0.0.3";
unsigned g_LastCreatedY = 15;
SettingsHandler g_Settings;
HWND hName, hEmail, hMisc1, hMisc2, hMisc3;
TimeClock g_Timer;
TemplateManager g_Templates(g_Settings, g_Timer);
std::vector<HWND> hTemplates;
RECT g_MainWin;
HWND hAddTemplateTitle, hAddTemplateText, hRemoveTemplateTitle;

//Forward Declarations
void CreateMainWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void AddMenu(HWND hwnd);
void AddControls(HWND hwnd);
void RegisterSettingsWindow(HINSTANCE hInst);
void OpenSettingsWindow(HWND hWnd);
LRESULT CALLBACK SetWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void RegisterEditWindow(HINSTANCE hInst);
void OpenEditWindow(HWND hWnd);
LRESULT CALLBACK EditWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(1, 22, 53));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = (HICON)LoadImage(hInstance, "Resources\\TM Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	wc.hIcon = (HICON)LoadImage(hInstance, "Resources\\TM Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	RegisterSettingsWindow(hInstance);
	RegisterEditWindow(hInstance);

	CreateMainWindow(hInstance);
	
	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}


void CreateMainWindow(HINSTANCE hInstance)
{
	unsigned approxWinHeight = ((g_Templates.GetTemplateCount() + 1) * 50) + 30;
	unsigned screenY = GetSystemMetrics(SM_CYSCREEN) - 50;

	if (approxWinHeight > screenY) {
		approxWinHeight = screenY;
	}

	if (approxWinHeight < 130) {
		approxWinHeight = 130;
	}

	hMainWindow = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, g_WindowTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		GetSystemMetrics(SM_CXSCREEN) - 350, 0, 350, approxWinHeight, NULL, NULL, hInstance, NULL);

	if (hMainWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		AddMenu(hwnd);
		AddControls(hwnd);
		break;
	case WM_COMMAND:
		if (wParam == ID_FILE_EXIT) { 
			PostQuitMessage(0); 
		}
		else if (wParam == ID_ABOUT) {
			MessageBox(NULL, "Just a quick template manager to copy templates on a button press.\n\n-Marius Ventus", "About", MB_OK | MB_ICONINFORMATION);
		}
		else if (wParam == ID_HELP) {
			MessageBox(NULL, "No help, only Zuul.\nOr reaching me on Teams.\n\nOr the Readme:\nhttps://github.com/MariusVentus", "Halp", MB_OK | MB_ICONINFORMATION);
		}
		else if (wParam == ID_OPEN_SETTINGS) {
			OpenSettingsWindow(hwnd);
		}
		else if (wParam == ID_EDIT_TEMPLATES) {
			OpenEditWindow(hwnd);
			//if (MessageBoxW(hwnd, L"A UI has not been completed for Templates. This will directly open the Templates.txt settings file, and this access was for testing purposes.\nA app restart is required to populate changes.\n\nAre you sure your want to enter?", L"Warning",
			//	MB_OKCANCEL | MB_ICONERROR) == IDOK) {
			//	ShellExecute(hwnd, "open", g_Templates.GetTemplateFileLoc().c_str(), NULL, NULL, SW_SHOW);
			//}
		}
		else if (wParam == ID_IN_PROGRESS) {
			MessageBox(NULL, "Apologies, this feature is under construction.", "Under Construction", MB_OK | MB_ICONEXCLAMATION);
		}
		else if (wParam >= ID_TEMPBASE && wParam < ID_TEMPBASE + g_Templates.GetTemplateCount()) {
			unsigned butNum = wParam - ID_TEMPBASE;
			std::string stringNote = g_Templates.GetTemplateXContent(butNum);
			//Copy to Clipboard
			OpenClipboard(hwnd);
			EmptyClipboard();
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, stringNote.size() + 1);
			if (!hg) {
				CloseClipboard();
			}
			else {
				memcpy(GlobalLock(hg), stringNote.c_str(), stringNote.size() + 1);
				GlobalUnlock(hg);
				SetClipboardData(CF_TEXT, hg);
				CloseClipboard();
			}
			GlobalFree(hg);
		}
		break;
	case WM_MOUSEWHEEL:
		//MessageBox(NULL, "Wheel", "Wheel", MB_OK | MB_ICONEXCLAMATION);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void AddMenu(HWND hwnd)
{
	HMENU hMenu, hFileMenu;
	hMenu = CreateMenu();
	//File Menu
	hFileMenu = CreatePopupMenu();
	AppendMenu(hFileMenu, MF_STRING, ID_EDIT_TEMPLATES, "Edit Templates");
	AppendMenu(hFileMenu, MF_STRING, ID_OPEN_SETTINGS, "Settings");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, "Exit");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hFileMenu, "File");

	//Remaining Main Menu Items1e1e1e
	AppendMenu(hMenu, MF_STRING, ID_ABOUT, "About");
	AppendMenu(hMenu, MF_STRING, ID_HELP, "Help");

	SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd)
{
	//HWND hInterior; Was going to try making the Template buttons show up in a scrollable box, but graphics issues insued. Will come back to this. 

	//hInterior = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "",
	//	WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
	//	0, 0, 350, 500, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
	//std::string holding;
	//for (unsigned i = 0; i < g_templateCount; i++) {
	//	holding.append("\r\n");
	//	holding.append("\r\n");
	//	holding.append("\r\n");
	//	holding.append("\r\n");
	//	holding.append("\r\n");
	//	holding.append("\r\n");
	//}
	//SetWindowText(hInterior, holding.c_str());

	for (unsigned i = 0; i < g_Templates.GetTemplateCount(); i++) {
		hTemplates.push_back({ 0 });
	}

	for (unsigned i = 0; i < g_Templates.GetTemplateCount(); i++) {
		hTemplates[i] = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", g_Templates.GetTemplateXTitle(i).c_str(), WS_CHILD | WS_VISIBLE,
			15, g_LastCreatedY, 285, 40, hwnd, (HMENU)(ID_TEMPBASE+i), GetModuleHandle(NULL), NULL);
		g_LastCreatedY += 50;
	}
}

void RegisterSettingsWindow(HINSTANCE hInst) {
	WNDCLASSEX setWin = { 0 };

	setWin.cbSize = sizeof(WNDCLASSEX);
	setWin.style = 0;
	setWin.lpfnWndProc = SetWinProc;
	setWin.cbClsExtra = 0;
	setWin.cbWndExtra = 0;
	setWin.hInstance = hInst;
	setWin.hCursor = LoadCursor(NULL, IDC_ARROW);
	setWin.hbrBackground = (HBRUSH)COLOR_WINDOW;
	setWin.lpszMenuName = NULL;
	setWin.lpszClassName = "mySettingsWindow";
	setWin.hIconSm = (HICON)LoadImage(hInst, "Resources\\TM Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);


	RegisterClassEx(&setWin);
}

void OpenSettingsWindow(HWND hWnd) {

	GetWindowRect(hMainWindow, &g_MainWin);

	HWND hSetWindow = CreateWindowEx(WS_EX_CLIENTEDGE, "mySettingsWindow", "Settings", WS_VISIBLE | WS_OVERLAPPEDWINDOW, g_MainWin.left, g_MainWin.top, 300, 400, hWnd, NULL, NULL, NULL);
	if (hSetWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	CreateWindowEx(NULL, "STATIC", "Name", WS_CHILD | WS_VISIBLE, 
		20, 10, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	hName = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", g_Settings.GetName().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 35, 200, 25, hSetWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
	
	CreateWindowEx(NULL, "STATIC", "Email", WS_CHILD | WS_VISIBLE,
		20, 60, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	hEmail = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", g_Settings.GetEmail().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 85, 200, 25, hSetWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
	
	CreateWindowEx(NULL, "STATIC", "Miscellaneous", WS_CHILD | WS_VISIBLE,
		20, 110, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	hMisc1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", g_Settings.GetMisc1().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 135, 200, 25, hSetWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
	
	CreateWindowEx(NULL, "STATIC", "Miscellaneous", WS_CHILD | WS_VISIBLE,
		20, 160, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	hMisc2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", g_Settings.GetMisc2().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 185, 200, 25, hSetWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
	
	CreateWindowEx(NULL, "STATIC", "Miscellaneous", WS_CHILD | WS_VISIBLE,
		20, 210, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	hMisc3 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", g_Settings.GetMisc3().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 235, 200, 25, hSetWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);



	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Ok", WS_VISIBLE | WS_CHILD, 25, 300, 100, 40, hSetWindow, (HMENU)IDOK, NULL, NULL);
	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Cancel", WS_VISIBLE | WS_CHILD, 150, 300, 100, 40, hSetWindow, (HMENU)IDCANCEL, NULL, NULL);

	//Disable the main window, turning a Modless dialogue box into a modal dialogue
	EnableWindow(hWnd, false);
}


LRESULT CALLBACK SetWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		switch (wp)
		{
		case IDCANCEL:
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		case IDOK:
			//Input
			char setName[100] = "";
			GetWindowText(hName, setName, 100);
			g_Settings.SetName(setName);
			//
			char setEmail[100] = "";
			GetWindowText(hEmail, setEmail, 100);
			g_Settings.SetEmail(setEmail);
			//
			char setMisc1[100] = "";
			GetWindowText(hMisc1, setMisc1, 100);
			g_Settings.SetMisc1(setMisc1);
			//
			char setMisc2[100] = "";
			GetWindowText(hMisc2, setMisc2, 100);
			g_Settings.SetMisc2(setMisc2);
			//
			char setMisc3[100] = "";
			GetWindowText(hMisc3, setMisc3, 100);
			g_Settings.SetMisc3(setMisc3);
			//Save
			g_Settings.SaveSettingsToFile();
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_CLOSE:
		EnableWindow(hMainWindow, true);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}


void RegisterEditWindow(HINSTANCE hInst) {
	WNDCLASSEX editWin = { 0 };

	editWin.cbSize = sizeof(WNDCLASSEX);
	editWin.style = 0;
	editWin.lpfnWndProc = EditWinProc;
	editWin.cbClsExtra = 0;
	editWin.cbWndExtra = 0;
	editWin.hInstance = hInst;
	editWin.hCursor = LoadCursor(NULL, IDC_ARROW);
	editWin.hbrBackground = (HBRUSH)COLOR_WINDOW;
	editWin.lpszMenuName = NULL;
	editWin.lpszClassName = "myEditWindow";
	editWin.hIconSm = (HICON)LoadImage(hInst, "Resources\\TM Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

	RegisterClassEx(&editWin);
}

void OpenEditWindow(HWND hWnd) {

	GetWindowRect(hMainWindow, &g_MainWin);

	HWND hEditWindow = CreateWindowEx(WS_EX_CLIENTEDGE, "myEditWindow", "Edit Templates", WS_VISIBLE | WS_OVERLAPPEDWINDOW, g_MainWin.left-200, g_MainWin.top, 500, 525, hWnd, NULL, NULL, NULL);
	if (hEditWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	CreateWindowEx(NULL, "STATIC", "Template Title (Button Text)", WS_CHILD | WS_VISIBLE,
		20, 10, 440, 25, hEditWindow, NULL, GetModuleHandle(NULL), NULL);
	hAddTemplateTitle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 35, 440, 25, hEditWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);


	CreateWindowEx(NULL, "STATIC", "Template Text", WS_CHILD | WS_VISIBLE,
		20, 60, 440, 25, hEditWindow, NULL, GetModuleHandle(NULL), NULL);
	hAddTemplateText = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		20, 85, 440, 200, hEditWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Add", WS_VISIBLE | WS_CHILD, 360, 300, 100, 40, hEditWindow, (HMENU)1, NULL, NULL);

	CreateWindowEx(NULL, "STATIC", "Template for Removal (Button Text)", WS_CHILD | WS_VISIBLE,
		20, 325, 250, 25, hEditWindow, NULL, GetModuleHandle(NULL), NULL);
	hRemoveTemplateTitle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 350, 440, 25, hEditWindow, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);


	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Remove", WS_VISIBLE | WS_CHILD, 360, 385, 100, 40, hEditWindow, (HMENU)2, NULL, NULL);
	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Close", WS_VISIBLE | WS_CHILD, 360, 435, 100, 40, hEditWindow, (HMENU)3, NULL, NULL);

	//Disable the main window, turning a Modless dialogue box into a modal dialogue
	EnableWindow(hWnd, false);
}


LRESULT CALLBACK EditWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_CLOSE:
		EnableWindow(hMainWindow, true);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	case WM_COMMAND:
		char title[100] = "";
		std::string titleString = "";
		char text[5000] = "";
		std::string textString = "";
		unsigned newID = g_Templates.GetTemplateCount() + 1;
		switch (wp)
		{
		case 1:
			GetWindowText(hAddTemplateTitle, title, 100);
			titleString = title;
			GetWindowText(hAddTemplateText, text, 5000);
			textString = text;
			if (!textString.empty() && !titleString.empty()) {
				g_Templates.AddTemplate(newID, titleString, textString);
				g_Templates.SaveTemplates();
				MessageBox(NULL, "Added Template!", "Added!", MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		case 2:
			GetWindowText(hRemoveTemplateTitle, title, 100);
			titleString = title;
			if (!titleString.empty()) {
				if (g_Templates.RemoveTemplate(titleString)) {
					MessageBox(NULL, "Removed Template!", "Removed!", MB_OK | MB_ICONEXCLAMATION);
					g_Templates.SaveTemplates();
				}
				else
				{
					MessageBox(NULL, "Removal Failed?\nPlease check the entered template title!", "Not Found?", MB_OK | MB_ICONEXCLAMATION);
				}
			}
			break;
		case 3:
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		}
		break;
	}
	return 0;
}