#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QVector>

#include <windows.h>
#include <functional>

enum class EventType
{
    All,
    ButtonDown,
    ButtonUp
};

struct llKeyHook{
    uint id;
    uint vkCode;
    EventType type;
    std::function<void(int, WPARAM, LPARAM)> callback;
    bool async;
};

struct KeyHook{
    uint id;
    uint vkCode;
    EventType type;
    std::function<void(RAWKEYBOARD)> callback;
    bool async;
};

struct MouseHook{
    uint id;
    uint vkCode;
    EventType type;
    std::function<void(RAWMOUSE)> callback;
    bool async;
};

struct WindowSwitchHook{
    uint id;
    std::function<void(HWND)> callback;
};

#endif // STRUCTURES_H
