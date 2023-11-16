#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

std::streamsize fileSize(std::string fileName) {

    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    std::streamsize size = 0;

    if (file.is_open()) {
        size = file.tellg();
        file.close();
    }

    return size;
}

int main(int argc, char** args)
{
    std::string fileName(args[1]);
    std::ofstream fout;

    HANDLE hReadySignal = CreateEventA(NULL, TRUE, FALSE, args[2]);
    HANDLE hMaxNumRecordsSem = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, TRUE, args[3]);

    SetEvent(hReadySignal);

    std::string command = "";

    while (1) {

        std::cout << "enter send and a message or exit" << std::endl;
        getline(std::cin, command);

        if (command == "exit") {
            return 0;
        }

        WaitForSingleObject(hMaxNumRecordsSem, INFINITE);

        if (command.find(' ') != std::string::npos && command.substr(0, command.find(' ')) == "send") {
            std::string message = command.substr(command.find(' ') + 1);

            if (message.length() > 20) {
                std::cout << "message length should not be greater than 20" << std::endl;

                ReleaseSemaphore(hMaxNumRecordsSem, 1, NULL);

                continue;
            }

            fout.open(fileName, std::ios::binary | std::ios::app);

            fout.write(message.c_str(), 20);

            fout.close();
        }
        else {
            std::cout << "unknown command" << std::endl;
            ReleaseSemaphore(hMaxNumRecordsSem, 1, NULL);
        }
        
    }

    system("pause");
    return 0;
}