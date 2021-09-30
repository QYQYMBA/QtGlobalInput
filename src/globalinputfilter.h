#ifndef GLOBALINPUTFILTER_H
#define GLOBALINPUTFILTER_H

#include <QAbstractNativeEventFilter>

#include "windows.h"
class GlobalInputFilter : public QAbstractNativeEventFilter
{
public:
    GlobalInputFilter();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
};

#endif // GLOBALINPUTFILTER_H
