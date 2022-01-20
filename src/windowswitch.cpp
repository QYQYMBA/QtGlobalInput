#include "windowswitch.h"

QVector<WindowSwitchHook> WindowSwitch::windowSwitches = QVector<WindowSwitchHook>();

int WindowSwitch::_id = 0;

HWINEVENTHOOK WindowSwitch::_winEvent;

bool WindowSwitch::removeWindowSwitch(uint id)
{
    for(qsizetype i = 0; i < windowSwitches.size(); i++)
    {
        if(windowSwitches[i].id == id)
        {
            windowSwitches.removeAt(i);
            if(windowSwitches.empty())
            {
                return UnhookWinEvent(_winEvent);
            }
            break;
        }
    }
    return false;
}

void CALLBACK WindowSwitch::WindowSwitchedCallback(
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
