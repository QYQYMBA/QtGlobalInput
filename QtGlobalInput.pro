QT += gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/rawinput.cpp \
    src/globalinputfilter.cpp \
    src/keyboardhooks.cpp \
    src/qtglobalinput.cpp \
    src/windowswitch.cpp

HEADERS += \
    src/rawinput.h \
    src/globalinputfilter.h \
    src/keyboardhooks.h \
    src/qtglobalinput.h \
    src/structures.h \
    src/windowswitch.h

LIBS += \
    -luser32 \
    -lKernel32 \
    -lgdi32 \
    -lwinspool \
    -lcomdlg32 \
    -ladvapi32 \
    -lshell32 \
    -lole32 \
    -loleaut32 \
    -luuid \
    -lodbc32 \
    -lodbccp32

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
