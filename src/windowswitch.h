#ifndef WINDOWSWITCH_H
#define WINDOWSWITCH_H

#include <QVector>

#include <windows.h>
#include <functional>

class WindowSwitch
{   

public:
    struct WindowSwitchHook{
        uint id;
        std::function<void(HWND)> callback;
    };

    static QVector<WindowSwitchHook> windowSwitches;

    template<typename A, typename B>
    uint setWindowSwitch(A callback, B obj, uint id);

    bool removeWindowSwitch(uint id);

    static void CALLBACK WindowSwitchedCallback(
      HWINEVENTHOOK hWinEventHook,
      DWORD event,
      HWND hwnd,
      LONG idObject,
      LONG idChild,
      DWORD idEventThread,
      DWORD dwmsEventTime
    )
    {
        for(size_t i = 0; i < WindowSwitch::windowSwitches.size(); i++)
        {
            WindowSwitch::windowSwitches[i].callback(hwnd);
        }
    };

private:
    HWINEVENTHOOK winEvent = NULL;
};

template<typename A, typename B>
uint WindowSwitch::setWindowSwitch(A callback, B obj, uint id)
{
    WindowSwitchHook ws;
    ws.callback = std::bind(callback, obj, std::placeholders::_1);
    ws.id = id;

    windowSwitches.push_back(ws);

    if(!winEvent)
    {
        winEvent = SetWinEventHook(EVENT_SYSTEM_FOREGROUND ,
                                    EVENT_SYSTEM_FOREGROUND , NULL,
                                    WindowSwitchedCallback, 0, 0,
                                    WINEVENT_OUTOFCONTEXT);
    }

    return ws.id;
}

#endif // WINDOWSWITCH_H
