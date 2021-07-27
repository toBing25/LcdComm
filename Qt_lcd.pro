QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += printsupport
TARGET = LcdComm
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGSqt \
            QT_NO_WARNING_OUTPUT \
            QT_NO_DEBUG_OUTPUT

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



DEFINES += iSMART_APP
contains(DEFINES,iSMART_APP) {
    DEFINES += iSMART_APP=1
}else{
    DEFINES += iSMART_APP=0
}

RESOURCES += resource/image/image.qrc \
             resource/language/language.qrc

TRANSLATIONS = resource/language/text_cn.ts \
               resource/language/text_en.ts


include(Formmain/formmain.pri)
include(gxwidget/gxwidget.pri)
include(pages/pages.pri)
include(loginmanage/loginmanage.pri)
include(readShareMemData/readsharemem.pri)
include(util/util.pri)
include(language/language.pri)

#DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += \
    software_version.h


