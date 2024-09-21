QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat.cpp \
    datebaseutils.cpp \
    main.cpp \
    message.cpp \
    register.cpp \
    userface.cpp \
    widget.cpp

HEADERS += \
    chat.h \
    datebaseutils.h \
    message.h \
    register.h \
    userface.h \
    widget.h

FORMS += \
    chat.ui \
    register.ui \
    userface.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc

DISTFILES += \
    resource/chat1.png \
    resource/chat2.png
