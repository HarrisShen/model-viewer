QT       += core gui widgets charts

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dataloader.cpp \
    interactivechart.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dataloader.h \
    interactivechart.h \
    mainwindow.h \
    qdatacolumn.h \
    qdataframe.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
