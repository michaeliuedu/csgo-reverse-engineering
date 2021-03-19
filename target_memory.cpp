#include <iostream>
#include <Windows.h>
#include <time.h>

static int value = 100;
int main()
{
    
    
    while (true) {
        value -= 1;
        std::cout << value << "\n";
        Sleep(3000);
    }

}


