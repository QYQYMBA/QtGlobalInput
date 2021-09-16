#include "globalinputfilter.h"

#include <QDebug>

#include "qtglobalinput.h"
GlobalInputFilter::GlobalInputFilter(void *qgi)
{
    _qtglobalinput = qgi;
}

bool GlobalInputFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    MSG* msg = (MSG*) message;
    switch (msg->message)
    {
    case WM_INPUT:
        UINT dwSize;
        LPARAM lParam = msg->lParam;

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[dwSize];
        if (lpb == NULL)
        {
            return 0;
        }

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
            qDebug() << "GetRawInputData does not return correct size !\n";

        RAWINPUT* raw = (RAWINPUT*)lpb;

        if (raw->header.dwType == RIM_TYPEKEYBOARD || raw->header.dwType == RIM_TYPEMOUSE)
        {
            //QtConcurrent::run(sendKeyInput, _layoutController, raw->data.keyboard);
            ((QtGlobalInput*) _qtglobalinput)->newInput(*raw);
        }
        break;
    }
    return false;
}
