#include <iostream>
#include <windows.h>

int main(int argc, char** args)
{
    std::cout << args[2] << std::endl;

    HANDLE hReadySignal = CreateEventA(NULL, TRUE, FALSE, args[2]);

    std::cout << hReadySignal << std::endl;

    SetEvent(hReadySignal);

    system("pause");
    return 0;
}