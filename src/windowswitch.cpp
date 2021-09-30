#include "windowswitch.h"

QVector<WindowSwitch::WindowSwitchHook> WindowSwitch::windowSwitches = QVector<WindowSwitch::WindowSwitchHook>();

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
