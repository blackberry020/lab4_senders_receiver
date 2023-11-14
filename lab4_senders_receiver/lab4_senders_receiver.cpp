#include <iostream>
#include <windows.h>

void runProcess(wchar_t* command) {

	STARTUPINFO si;
	PROCESS_INFORMATION piCom;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	bool status = CreateProcessW(NULL, command, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom);

	if (!status) {
		std::wcout << "did not create";
		exit(-1);
	}

	WaitForSingleObject(piCom.hProcess, INFINITE);

	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);

	return;
}

int main()
{
	std::wstring commandLine = L"receiver.exe";
	wchar_t* finalCommand = _wcsdup(commandLine.c_str());

	STARTUPINFO si;
	PROCESS_INFORMATION piCom;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	bool status = CreateProcessW(NULL, finalCommand, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom);

	if (!status) {
		std::wcout << "did not create receiver";
		exit(-1);
	}
}