#include "windowswitch.h"

QVector<WindowSwitch::WindowSwitchHook> WindowSwitch::windowSwitches = QVector<WindowSwitch::WindowSwitchHook>();

bool WindowSwitch::removeWindowSwitch(uint id)
{
    for(qsizetype i = 0; i < windowSwitches.size(); i++)
    {
        if(windowSwitches[i].id == id)
        {
            windowSwitches.removeAt(i);
            if(windowSwitches.empty())
            {
                UnhookWinEvent(winEvent);
            }
            break;
        }
    }
    return 0;
}
