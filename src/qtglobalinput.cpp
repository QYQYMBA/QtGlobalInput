#include "qtglobalinput.h"

HWND QtGlobalInput::_hwnd;

void QtGlobalInput::init(HWND hwnd)
{
    _hwnd = hwnd;

    RawInput::init(hwnd);
}

void QtGlobalInput::deinitialize()
{
    RawInput::deinitialize();
}

bool QtGlobalInput::removeLlKeyboardHook(uint id)
{
    return llKeyboardHooks::removeLlHook(id);
}

bool QtGlobalInput::removeKeyPress(uint id)
{
    return RawInput::removeKeyPress(id);
}

bool QtGlobalInput::removeMousePress(uint id)
{
    return RawInput::removeMousePress(id);
}

bool QtGlobalInput::removeWindowSwitch(uint id)
{
    return WindowSwitch::removeWindowSwitch(id);
}

