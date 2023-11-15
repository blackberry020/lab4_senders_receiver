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

    SetEvent(hReadySignal);

    int maxRecordsNum = atoi(args[3]);

    std::string command = "";

    while (1) {

        std::cout << "enter send and a message or exit" << std::endl;
        getline(std::cin, command);

        if (command == "exit") {
            return 0;
        }   
        else {
            if (command.find(' ') != std::string::npos && command.substr(0, command.find(' ')) == "send") {
                std::string message = command.substr(command.find(' ') + 1);

                if (fileSize(fileName) > maxRecordsNum) {
                    std::cout << "file is full, can't write this message" << std::endl;
                    continue;
                }

                if (message.length() > 20) {
                    std::cout << "message length should not be greater than 20" << std::endl;
                    continue;
                }

                fout.open(fileName, std::ios::binary | std::ios::app);

                fout.write(message.c_str(), 20);

                fout.close();
            }
            else std::cout << "unknown command" << std::endl;
        }
    }

    system("pause");
    return 0;
}