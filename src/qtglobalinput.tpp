#include "qtglobalinput.h"

template<typename A, typename B>
uint QtGlobalInput::setKeyPress(uint vkCode, EventType type, A callback, B obj, bool async)
{
    KeyHook kh;
    kh.callback = std::bind(callback, obj, std::placeholders::_1);
    kh.vkCode = vkCode;
    kh.type = type;
    kh.id = _id++;
    kh.async = async;

    _keyHooks.push_back(kh);

    if(!_nativeFilterInstalled)
    {
        installNativeEventFilter();
    }

    return kh.id;
}

template<typename A, typename B>
uint QtGlobalInput::setMousePress(uint vkCode, EventType type, A callback, B obj, bool async)
{
    MouseHook mh;
    mh.callback = std::bind(callback, obj, std::placeholders::_1);
    mh.vkCode = vkCode;
    mh.type = type;
    mh.id = _id++;
    mh.async = async;

    _mouseHooks.push_back(mh);

    if(!_nativeFilterInstalled)
    {
        installNativeEventFilter();
    }

    return mh.id;
}
