#ifndef GLOBALINPUTFILTER_H
#define GLOBALINPUTFILTER_H

#include <QAbstractNativeEventFilter>

#include "windows.h"
class GlobalInputFilter : public QAbstractNativeEventFilter
{
public:
    GlobalInputFilter(void* qgi);

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
private:

    void* _qtglobalinput;
};

#endif // GLOBALINPUTFILTER_H
