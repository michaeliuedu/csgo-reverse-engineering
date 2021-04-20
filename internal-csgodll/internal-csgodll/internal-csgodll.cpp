// internal-csgodll.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "internal-csgodll.h"


// This is an example of an exported variable
INTERNALCSGODLL_API int ninternalcsgodll=0;

// This is an example of an exported function.
INTERNALCSGODLL_API int fninternalcsgodll(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cinternalcsgodll::Cinternalcsgodll()
{
    return;
}
