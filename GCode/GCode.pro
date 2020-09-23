QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AboutThisWindow.cpp \
    AboutUsWindow.cpp \
    CodeEdit.cpp \
    Dialog.cpp \
    FRWindow.cpp \
    LineArea.cpp \
    OptionWindow.cpp \
    Slide.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AboutThisWindow.h \
    AboutUsWindow.h \
    CodeEdit.h \
    Dialog.h \
    FRWindow.h \
    LineArea.h \
    MainWindow.h \
    OptionWindow.h \
    Slide.h

FORMS += \
    AboutThisWindow.ui \
    AboutUsWindow.ui \
    Dialog.ui \
    FRWindow.ui \
    MainWindow.ui \
    OptionWindow.ui \
    Slide.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    keywords/Keywords.txt \
    stylesheets/MainWindowD.qss \
    stylesheets/MainWindowL.qss \
    stylesheets/PushButtonL.qss

RESOURCES += \
    icons.qrc \
    keywords.qrc \
    stylesheets.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Highlighter/release/ -lHighlighter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Highlighter/debug/ -lHighlighter
else:unix: LIBS += -L$$OUT_PWD/../Highlighter/ -lHighlighter

INCLUDEPATH += $$PWD/../Highlighter
DEPENDPATH += $$PWD/../Highlighter
