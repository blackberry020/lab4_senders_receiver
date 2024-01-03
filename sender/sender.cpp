#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

// get size of an occupied file's space 
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

    // get ready event and semaphore for the amount of records
    HANDLE hReadySignal = CreateEventA(NULL, TRUE, FALSE, args[2]);
    HANDLE hMaxNumRecordsSem = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, TRUE, args[3]);

    // mark process as ready
    SetEvent(hReadySignal);

    std::string command = "";

    // constantly getting commands
    while (1) {

        std::cout << "enter send and a message or exit" << std::endl;
        getline(std::cin, command);

        if (command == "exit") {
            return 0;
        }

        // if there is a space for the record - write, otherwise wait until receiver reads one record
        WaitForSingleObject(hMaxNumRecordsSem, INFINITE);

        // if the command is "read"
        if (command.find(' ') != std::string::npos && command.substr(0, command.find(' ')) == "send") {
            // get the message
            std::string message = command.substr(command.find(' ') + 1);

            // if the message is too long skip it
            if (message.length() > 20) {
                std::cout << "message length should not be greater than 20" << std::endl;
                ReleaseSemaphore(hMaxNumRecordsSem, 1, NULL);
                continue;
            }

            // write the message 
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