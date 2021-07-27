
INCLUDEPATH += $$PWD/pageSysOverview \
               $$PWD/pageSysSettings \
               $$PWD/pageDataManage \
               $$PWD/pageAlarm \
               $$PWD/pageExtend \
               $$PWD/pageSysOverview/hostPage \

HEADERS += $$PWD/pageSysOverview/pagesysoverview.h \
           $$PWD/pageSysOverview/pagesysoverview_.h \
           $$PWD/pageAlarm/wgthistory.h \
           $$PWD/pageDataManage/historyData.h \
           $$PWD/pageSysOverview/alarmborder.h \
           $$PWD/pageSysOverview/alarmwidget.h \
           $$PWD/pageSysOverview/tableWidgetDelegate.h \
           $$PWD/pageSysOverview/hostPage/hostpage.h \
           $$PWD/pageSysSettings/accesscontrol.h \
           $$PWD/pageSysSettings/gettext.h \
           $$PWD/pageSysSettings/pagesystemsettings.h \
           $$PWD/pageSysSettings/pagenetworkset.h \
           $$PWD/pageSysSettings/pagetranslate.h\
           $$PWD/pageSysSettings/Gpio.h\
           $$PWD/pageDataManage/pagedatamanage.h \
           $$PWD/pageAlarm/pagealarm.h \
           $$PWD/pageExtend/pageextend.h \
           $$PWD/pageSysSettings/sysfunction.h\
           $$PWD/pageSysSettings/switchbutton.h \
           $$PWD/pageSysSettings/cardopert.h \



SOURCES += $$PWD/pageSysOverview/pagesysoverview.cpp \
           $$PWD/pageSysOverview/pagesysoverview_.cpp \
           $$PWD/pageAlarm/wgthistory.cpp \
           $$PWD/pageDataManage/historyData.cpp \
           $$PWD/pageSysOverview/alarmborder.cpp \
           $$PWD/pageSysOverview/alarmwidget.cpp \
           $$PWD/pageSysOverview/hostPage/hostpage.cpp \
           $$PWD/pageSysSettings/accesscontrol.cpp \
           $$PWD/pageSysSettings/gettext.cpp \
           $$PWD/pageSysSettings/pagesystemsettings.cpp\
           $$PWD/pageSysSettings/pagenetworkset.cpp\
           $$PWD/pageSysSettings/pagetranslate.cpp\
           $$PWD/pageSysSettings/Gpio.cpp\
           $$PWD/pageDataManage/pagedatamanage.cpp \
           $$PWD/pageAlarm/pagealarm.cpp \
           $$PWD/pageExtend/pageextend.cpp \
           $$PWD/pageSysSettings/sysfunction.cpp\
           $$PWD/pageSysSettings/switchbutton.cpp\
           $$PWD/pageSysSettings/cardopert.cpp \

FORMS   += $$PWD/pageSysOverview/pagesysoverview.ui \
           $$PWD/pageSysOverview/pagesysoverview_.ui \
           $$PWD/pageSysOverview/hostPage/hostpage.ui \
           $$PWD/pageDataManage/historyData.ui \
           $$PWD/pageSysOverview/alarmborder.ui \
           $$PWD/pageAlarm/wgthistory.ui \
           $$PWD/pageSysSettings/accesscontrol.ui \
           $$PWD/pageSysSettings/pagetranslate.ui \
           $$PWD/pageSysSettings/sysfunction.ui

DISTFILES +=
