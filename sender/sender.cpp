#include <iostream>
#include <windows.h>

int main(int argc, char** args)
{

    HANDLE hStartSignal = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"hStartSignal");

    std::cout << hStartSignal << std::endl;

    WaitForSingleObject(hStartSignal, INFINITE);

    std::cout << "sender " << args[1] << std::endl;

    system("pause");
    return 0;
}