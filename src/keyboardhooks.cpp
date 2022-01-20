#include "keyboardhooks.h"

QVector<llKeyHook> llKeyboardHooks::_llKeyHooks;
HHOOK llKeyboardHooks::_keyHook;
bool llKeyboardHooks::_hookInstalled = false;
uint llKeyboardHooks::_id;

LRESULT llKeyboardHooks::keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    newInput(nCode, wParam, lParam);
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool llKeyboardHooks::removeLlHook(uint id)
{
    for(qsizetype i = 0; i < _llKeyHooks.size(); i++)
    {
        if(_llKeyHooks[i].id == id)
        {
            _llKeyHooks.removeAt(i);
            if(_llKeyHooks.empty())
            {
                if(_hookInstalled)
                    removeHook();
                return true;
            }
        }
    }
    return false;
}

void llKeyboardHooks::newInput(int nCode, WPARAM wParam, LPARAM lParam)
{
    for(qsizetype i = 0; i < _llKeyHooks.size(); i++)
    {
        if(_llKeyHooks[i].type == EventType::All
                || (_llKeyHooks[i].type == EventType::ButtonDown && wParam == WM_KEYDOWN)
                || (_llKeyHooks[i].type == EventType::ButtonDown && wParam == WM_SYSKEYDOWN)
                || (_llKeyHooks[i].type == EventType::ButtonUp && wParam == WM_KEYUP)
                || (_llKeyHooks[i].type == EventType::ButtonUp && wParam == WM_SYSKEYUP))
        {
            PKBDLLHOOKSTRUCT key  = (PKBDLLHOOKSTRUCT)lParam;
            if(_llKeyHooks[i].vkCode == 0 || _llKeyHooks[i].vkCode == key->vkCode)
            {
                _llKeyHooks[i].callback(nCode, wParam, lParam);
            }
        }
    }
}

void llKeyboardHooks::installHook()
{
    _keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, NULL, 0);
    _hookInstalled = true;
}

void llKeyboardHooks::removeHook()
{
    UnhookWindowsHookEx(_keyHook);
    _hookInstalled = false;
}
