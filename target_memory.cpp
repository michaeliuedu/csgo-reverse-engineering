#include <iostream>
#include <Windows.h>
#include <time.h>

int main()
{
    int value = 100;
    
    while (true) {
        value -= 1;
        std::cout << value << "\n";
        Sleep(3000);
    }

}


