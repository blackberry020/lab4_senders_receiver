#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

bool isNumber(std::string str) {

    if (str.empty()) return false;

    for (int i = 0; i < str.size(); i++) {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return false;
    }

    return true;
}

int main()
{
    std::string fileName = "";

    while ( fileName.find('.') == std::string::npos || 
            fileName.substr(fileName.find('.') + 1) != "bin" || 
            count(fileName.begin(), fileName.end(), '.') > 1) {

        std::cout << "Enter your bin file name" << std::endl;
        std::cin >> fileName;
    }

    std::ofstream outfile(fileName, std::ios_base::trunc);
    if (!outfile.is_open()) std::cout << "didn't create" << std::endl;
    outfile.close();

    std::string maxRecordNumStr = "";

    while (!isNumber(maxRecordNumStr)) {
        std::cout << "Enter a maximum number of records in your file" << std::endl;
        std::cin >> maxRecordNumStr;
    }

    int maxRecordNum = std::stoi(maxRecordNumStr.c_str());

    std::string senderCntStr = "";
    while (!isNumber(senderCntStr)) {
        std::cout << "Enter the number of senders" << std::endl;
        std::cin >> senderCntStr;
    }

    int senderCnt = std::stoi(senderCntStr.c_str());

    STARTUPINFOA* si = new STARTUPINFOA[senderCnt + 1];
    PROCESS_INFORMATION* piCom = new PROCESS_INFORMATION[senderCnt + 1];

    for (int i = 0; i < senderCnt; i++) {
        ZeroMemory(&si[i], sizeof(STARTUPINFOA));
        si[i].cb = sizeof(STARTUPINFOA);
    }

    std::string commandLine = "sender.exe ";
    std::string finalCommandStr = "";

    HANDLE* readyEvents = new HANDLE[senderCnt + 2];

    for (int i = 0; i < senderCnt; i++) {
        std::string dop = "event" + std::to_string(i);
        readyEvents[i] = CreateEventA(NULL, TRUE, FALSE, dop.c_str());
    }

    HANDLE hMaxNumRecordsSem = CreateSemaphoreA(NULL, maxRecordNum, maxRecordNum, "hMaxNumRecordsSem");

    for (int i = 0; i < senderCnt; i++) {

        finalCommandStr = commandLine + fileName + " event" + std::to_string(i) + " " + 
                          "hMaxNumRecordsSem";
        
        std::cout << " " << finalCommandStr << std::endl;

        char* finalCommand = new char[finalCommandStr.length() + 1];
        strcpy_s(finalCommand, finalCommandStr.length() + 1, finalCommandStr.c_str());

        std::cout << finalCommand << std::endl;

        bool status = CreateProcessA(NULL, finalCommand, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &piCom[i]);

        if (!status) std::cout << "didn't create process " << i << std::endl;
    }

    for (int i = 0; i < senderCnt; i++) {
        WaitForSingleObject(readyEvents[i], INFINITE);
    }

    char* charMessage = new char[20];
    char* extraInfo = new char[20 * maxRecordNum];
    std::string fileNameStr(fileName.begin(), fileName.end());
    std::ifstream fin;
    std::ofstream fout;

    while (1) {
        std::cout << "Enter read or exit" << std::endl;

        std::string option;
        std::cin >> option;

        if (option == "read") {
            fin.open(fileName, std::ios_base::binary);

            if (fin.peek() == EOF) {
                // wait
                std::cout << "waiting" << std::endl;
                continue;
            }

            fin.read(charMessage, 20);
            std::cout << charMessage << std::endl;

            fin.read(extraInfo, 20 * (maxRecordNum - 1));
            fin.close();

            fout.open(fileName, std::ios_base::binary | std::ios_base::trunc);
            fout.write(extraInfo, 20 * maxRecordNum);
            fout.close();

            ReleaseSemaphore(hMaxNumRecordsSem, 1, NULL);
        }
        else if (option == "exit") {
            return 0;
        }
        else {
            std::cout << "You entered wrong command" << std::endl;
        }
    }

    system("pause");

    return 0;
}