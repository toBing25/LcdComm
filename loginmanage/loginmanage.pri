QT       +=  sql

INCLUDEPATH += $$PWD

HEADERS += $$PWD/loginmanage.h \
    $$PWD/loginui.h \
    $$PWD/writesqlquerymodel.h \




SOURCES += $$PWD/loginmanage.cpp \
    $$PWD/loginui.cpp \
           $$PWD/writesqlquerymodel.cpp \

RESOURCES +=

FORMS += \
    $$PWD/loginui.ui
