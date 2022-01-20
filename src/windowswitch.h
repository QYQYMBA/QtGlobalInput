#ifndef WINDOWSWITCH_H
#define WINDOWSWITCH_H

#include <QVector>

#include <windows.h>
#include <functional>

#include "structures.h"

class WindowSwitch
{   

public:
    static QVector<WindowSwitchHook> windowSwitches;

    template<typename A, typename B>
    static uint setWindowSwitch(A callback, B obj);

    static bool removeWindowSwitch(uint id);

private:
    WindowSwitch() {}

    static void CALLBACK WindowSwitchedCallback(
      HWINEVENTHOOK hWinEventHook,
      DWORD event,
      HWND hwnd,
      LONG idObject,
      LONG idChild,
      DWORD idEventThread,
      DWORD dwmsEventTime
    );

    static HWINEVENTHOOK _winEvent;

    static int _id;
};

template<typename A, typename B>
uint WindowSwitch::setWindowSwitch(A callback, B obj)
{
    WindowSwitchHook ws;
    ws.callback = std::bind(callback, obj, std::placeholders::_1);
    ws.id = ++_id;

    windowSwitches.push_back(ws);

    if(!_winEvent)
    {
        _winEvent = SetWinEventHook(EVENT_SYSTEM_FOREGROUND ,
                                    EVENT_SYSTEM_FOREGROUND , NULL,
                                    WindowSwitchedCallback, 0, 0,
                                    WINEVENT_OUTOFCONTEXT);
    }

    return ws.id;
}

#endif // WINDOWSWITCH_H
