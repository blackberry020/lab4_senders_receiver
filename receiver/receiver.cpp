#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

bool isNumber(std::wstring str) {

    if (str.empty()) return false;

    for (int i = 0; i < str.size(); i++) {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return false;
    }

    return true;
}

int main()
{
    std::wstring fileName = L"";

    while (fileName.find('.') == std::string::npos || fileName.substr(fileName.find('.') + 1) != L"bin" || count(fileName.begin(), fileName.end(), '.') > 1) {
        std::wcout << "Enter your bin file name" << std::endl;
        std::wcin >> fileName;
    }

    std::ofstream outfile(fileName);

    if (!outfile.is_open()) std::wcout << "didn't create" << std::endl;
    outfile.close();

    std::wstring maxRecordNumStr = L"";

    while (!isNumber(maxRecordNumStr)) {
        std::wcout << "Enter a maximum number of records in your file" << std::endl;
        std::wcin >> maxRecordNumStr;
    }

    int maxRecordNum = std::stoi(maxRecordNumStr.c_str());

    std::wstring senderCntStr = L"";
    while (!isNumber(senderCntStr)) {
        std::wcout << "Enter the number of senders" << std::endl;
        std::wcin >> senderCntStr;
    }

    int senderCnt = std::stoi(senderCntStr.c_str());

    STARTUPINFO* si = new STARTUPINFO[senderCnt + 1];
    PROCESS_INFORMATION* piCom = new PROCESS_INFORMATION[senderCnt + 1];

    for (int i = 0; i < senderCnt; i++) {
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);
    }

    std::wstring commandLine = L"sender.exe ";
    wchar_t* finalCommand;

    HANDLE* readyEvents = new HANDLE[senderCnt + 2];

    for (int i = 0; i < senderCnt; i++) {
        std::wstring dop = L"event" + std::to_wstring(i);
        readyEvents[i] = CreateEvent(NULL, TRUE, FALSE, _wcsdup(dop.c_str()));
    }

    for (int i = 0; i < senderCnt; i++) {

        finalCommand = _wcsdup((commandLine + fileName + L" event" + std::to_wstring(i) + L" " + maxRecordNumStr).c_str());

        std::wcout << finalCommand << std::endl;

        bool status = CreateProcessW(NULL, finalCommand, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &piCom[i]);

        if (!status) std::wcout << "didn't create process " << i << std::endl;
    }

    for (int i = 0; i < senderCnt; i++) {
        WaitForSingleObject(readyEvents[i], INFINITE);
    }

    while (1) {
        std::wcout << "Enter read or exit" << std::endl;

        std::wstring option;
        std::wcin >> option;

        if (option == L"read") {
            std::wcout << "reading" << std::endl;
            // wait if file is empty
        }
        else if (option == L"exit") {
            return 0;
        }
        else {
            std::wcout << "You entered wrong command" << std::endl;
        }
    }

    system("pause");

    return 0;
}