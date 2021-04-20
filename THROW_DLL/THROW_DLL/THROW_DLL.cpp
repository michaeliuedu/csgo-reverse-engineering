// THROW_DLL.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "THROW_DLL.h"


// This is an example of an exported variable
THROWDLL_API int nTHROWDLL=0;

// This is an example of an exported function.
THROWDLL_API int fnTHROWDLL(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CTHROWDLL::CTHROWDLL()
{
    return;
}
