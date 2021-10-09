#pragma once
#include "functions.h"
#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

namespace bear
{
    class Functions
    {
    public:
        static void gui_callback();
    };

}

#endif // _FUNCTIONS_H_