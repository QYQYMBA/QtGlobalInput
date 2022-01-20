#ifndef KEYBOARDHOOKS_H
#define KEYBOARDHOOKS_H

#include <QVector>

#include <windows.h>
#include <functional>

#include "structures.h"

class llKeyboardHooks
{
public:
    template<typename A, typename B>
    static uint setLlHook(uint vkCode, EventType type, A callback, B obj, bool async = false);
    static bool removeLlHook(uint id);

    static void newInput(int nCode, WPARAM wParam, LPARAM lParam);
private:
    llKeyboardHooks() {}

    static LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam);

    static void installHook();
    static void removeHook();

    static uint _id;

    static bool _hookInstalled;

    static QVector<llKeyHook> _llKeyHooks;

    static HHOOK _keyHook;
};

template<typename A, typename B>
uint llKeyboardHooks::setLlHook(uint vkCode, EventType type, A callback, B obj, bool async)
{
    llKeyHook kh;
    kh.callback = std::bind(callback, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    kh.vkCode = vkCode;
    kh.type = type;
    kh.id = _id++;
    kh.async = async;

    _llKeyHooks.push_back(kh);

    if(!_hookInstalled)
    {
        installHook();
    }

    return kh.id;
}

#endif // KEYBOARDHOOKS_H
