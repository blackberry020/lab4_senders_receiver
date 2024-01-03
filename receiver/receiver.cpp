#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

#include "..\func.h"

int main()
{
    std::string fileName;
    std::cout << "Enter your file name" << std::endl;
    std::cin >> fileName;
    std::ofstream outfile(fileName, std::ios_base::trunc);

    if (!outfile.is_open()) std::cout << "didn't create" << std::endl;
    outfile.close();

    // read maximum amount of records in file
    std::string maxRecordNumStr = "";

    while (!isNumber(maxRecordNumStr)) {
        std::cout << "Enter a maximum number of records in your file" << std::endl;
        std::cin >> maxRecordNumStr;
    }

    int maxRecordNum = std::stoi(maxRecordNumStr.c_str());

    // read number of senders
    std::string senderCntStr = "";
    while (!isNumber(senderCntStr)) {
        std::cout << "Enter the number of senders" << std::endl;
        std::cin >> senderCntStr;
    }

    int senderCnt = std::stoi(senderCntStr.c_str());

    // for processes
    STARTUPINFOA* si = new STARTUPINFOA[senderCnt + 1];
    PROCESS_INFORMATION* piCom = new PROCESS_INFORMATION[senderCnt + 1];
    HANDLE* readyEvents = new HANDLE[senderCnt + 2];

    for (int i = 0; i < senderCnt; i++) {
        ZeroMemory(&si[i], sizeof(STARTUPINFOA));
        si[i].cb = sizeof(STARTUPINFOA);
    }

    std::string commandLine = "sender.exe ";
    std::string finalCommandStr = "";

    // create events to show what processes are ready 
    for (int i = 0; i < senderCnt; i++) {
        std::string dop = "event" + std::to_string(i);
        readyEvents[i] = CreateEventA(NULL, TRUE, FALSE, dop.c_str());
    }

    // semaphore to control the amount of records
    HANDLE hMaxNumRecordsSem = CreateSemaphoreA(NULL, maxRecordNum, maxRecordNum, "hMaxNumRecordsSem");

    for (int i = 0; i < senderCnt; i++) {
        // forming a command
        finalCommandStr = commandLine + fileName + " event" + std::to_string(i) + " " + 
                          "hMaxNumRecordsSem";
        char* finalCommand = new char[finalCommandStr.length() + 1];
        strcpy_s(finalCommand, finalCommandStr.length() + 1, finalCommandStr.c_str());

        bool status = CreateProcessA(NULL, finalCommand, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &piCom[i]);

        if (!status) std::cout << "didn't create process " << i << std::endl;
    }

    // wait for all processes
    for (int i = 0; i < senderCnt; i++) {
        WaitForSingleObject(readyEvents[i], INFINITE);
    }

    char* charMessage = new char[20];
    std::string fileNameStr(fileName.begin(), fileName.end());
    std::ifstream fin;
    std::ofstream fout;

    bool ableToRead = false;

    // constantly waiting for a command
    while (1) {
        std::cout << "Enter read or exit" << std::endl;

        std::string option;
        std::cin >> option;

        if (option == "read") {

            // read if we can, otherwise wait for the appearance of a new record
            while (!ableToRead) {

                fin.open(fileName, std::ios_base::binary);

                if (fin.peek() != EOF) {
                    ableToRead = true;
                }
                
                fin.close();
            }

            ableToRead = false;

            // split the message and left records
            fin.open(fileName, std::ios_base::binary);
            fin.read(charMessage, 20);
            std::cout << charMessage << std::endl;

            char dop;
            std::string extraInfoStr = "";

            while (1) {
                dop = fin.get();
                if (fin.fail()) break;
                else extraInfoStr += dop;
            }

            fin.close();

            //rewrite file without read record
            fout.open(fileName, std::ios_base::binary | std::ios_base::trunc);
            fout.write(extraInfoStr.c_str(), extraInfoStr.length());
            fout.close();

            // we have read one record, so this space is available for the other 
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