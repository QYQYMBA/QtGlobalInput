#include "qtglobalinput.h"

#include <QAbstractEventDispatcher>
#include <QtConcurrent/QtConcurrent>


QtGlobalInput::QtGlobalInput(HWND hwnd)
    :_globalInputFilter(this)
    ,_hwnd(hwnd)
    ,_id(0)
    ,_nativeFilterInstalled(false)
{
    registerKeyboardInput();
    registerMouseInput();
}

bool QtGlobalInput::removeKeyPress(uint id)
{
    for(qsizetype i = 0; i < _keyHooks.size(); i++)
    {
        if(_keyHooks[i].id == id)
        {
            _keyHooks.removeAt(i);
            if(_keyHooks.empty() && _mouseHooks.empty())
            {
                removeNativeEventFilter();
            }
            break;
        }
    }
    return 0;
}

bool QtGlobalInput::removeMousePress(uint id)
{
    for(qsizetype i = 0; i < _mouseHooks.size(); i++)
    {
        if(_mouseHooks[i].id == id)
        {
            _mouseHooks.removeAt(i);
            if(_keyHooks.empty() && _mouseHooks.empty())
            {
                removeNativeEventFilter();
            }
            break;
        }
    }
    return 0;
}

void QtGlobalInput::newInput(RAWINPUT raw)
{
    if(raw.header.dwType == RIM_TYPEKEYBOARD)
    {
        RAWKEYBOARD rk = raw.data.keyboard;
        for(qsizetype i = 0; i < _keyHooks.size(); i++)
        {
            if(_keyHooks[i].type == EventType::All
                    || (_keyHooks[i].type == EventType::ButtonDown && rk.Message == WM_KEYDOWN)
                    || (_keyHooks[i].type == EventType::ButtonDown && rk.Message == WM_SYSKEYDOWN)
                    || (_keyHooks[i].type == EventType::ButtonUp && rk.Message == WM_KEYUP)
                    || (_keyHooks[i].type == EventType::ButtonUp && rk.Message == WM_SYSKEYUP))
                if(_keyHooks[i].vkCode == 0 || _keyHooks[i].vkCode == rk.VKey)
                {
                    if(!_keyHooks[i].async)
                        _keyHooks[i].callback(rk);
                    else
                    {
                        QtConcurrent::run(_keyHooks[i].callback, rk);
                    }
                }
        }
    }

    if(raw.header.dwType == RIM_TYPEMOUSE)
    {
        RAWMOUSE rm = raw.data.mouse;

        for(qsizetype i = 0; i < _mouseHooks.size(); i++)
        {
            bool down = (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
                    || (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
                    || (rm.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN);

            bool up = (rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
                    || (rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
                    || (rm.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP);

            if(!up && !down)
                return;

            uint button;

            if(rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP || rm.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
            {
                    button = VK_LBUTTON;
            }
            if(rm.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP || rm.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
            {
                    button = VK_MBUTTON;
            }
            if(rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP || rm.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
            {
                    button = VK_RBUTTON;
            }

            if(_mouseHooks[i].type == EventType::All
                    || (_mouseHooks[i].type == EventType::ButtonDown && down)
                    || (_mouseHooks[i].type == EventType::ButtonUp && up))
                if(_mouseHooks[i].vkCode == button || _mouseHooks[i].vkCode == 0)
                {
                    if(!_mouseHooks[i].async)
                        _mouseHooks[i].callback(rm);
                    else
                    {
                        QtConcurrent::run(_mouseHooks[i].callback, rm);
                    }
                }
        }
    }
}

void QtGlobalInput::installNativeEventFilter()
{
    _nativeFilterInstalled = true;
    QAbstractEventDispatcher::instance()->installNativeEventFilter(&_globalInputFilter);
}

void QtGlobalInput::removeNativeEventFilter()
{
    _nativeFilterInstalled = false;
    QAbstractEventDispatcher::instance()->removeNativeEventFilter(&_globalInputFilter);
}

bool QtGlobalInput::registerKeyboardInput()
{
    RAWINPUTDEVICE Rid[1];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x06;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = reinterpret_cast<HWND>(_hwnd);

    if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
    {
         qDebug() << "Raw input device registration failed!\n";
         return false;
    }

    return true;
}

bool QtGlobalInput::removeKeyboardInput()
{
    RAWINPUTDEVICE Rid[1];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x06;
    Rid[0].dwFlags = RIDEV_REMOVE;
    Rid[0].hwndTarget = reinterpret_cast<HWND>(_hwnd);

    if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
    {
         qDebug() << "Raw input device registration failed!\n";
         return false;
    }

    return true;
}

bool QtGlobalInput::registerMouseInput()
{
    RAWINPUTDEVICE Rid[1];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = reinterpret_cast<HWND>(_hwnd);

    if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
    {
         qDebug() << "Raw input device registration failed!\n";
         return false;
    }

    return true;
}

bool QtGlobalInput::removeMouseInput()
{
    RAWINPUTDEVICE Rid[1];

    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = RIDEV_REMOVE;
    Rid[0].hwndTarget = reinterpret_cast<HWND>(_hwnd);

    if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
    {
         qDebug() << "Raw input device registration failed!\n";
         return false;
    }

    return true;
}
