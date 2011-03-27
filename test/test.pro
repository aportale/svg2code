QT += svg

CONFIG += console

SOURCES += \
    testmain.cpp \
    ../codepaintdevice.cpp

HEADERS += \
    ../codepaintdevice.h

INCLUDEPATH += \
    ../

OTHER_FILES += \
    html5canvas/index.html \
    html5canvas/svg2code-test.js

RESOURCES += \
    resources.qrc
