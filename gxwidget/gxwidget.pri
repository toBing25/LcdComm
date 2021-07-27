
INCLUDEPATH += $$PWD/wgtbgscreen \
               $$PWD/wgtkeypad/ \
               $$PWD/wgttitlebar/\
               $$PWD/wgtmenubar/\
               $$PWD/wgtmenubar_ext/\
               $$PWD/wgtparaitem/\
               $$PWD/wgtchart/\
               $$PWD/wgtmsgbox/\
               $$PWD/Buzzer/\
               $$PWD/wgtdoorbox/\

HEADERS     += $$PWD/wgtbgscreen/wgtbgscreen.h \
               $$PWD/Buzzer/buzzer.h \
               $$PWD/wgtbgscreen/wgtbgrender.h \
               $$PWD/wgtbgscreen/wgtwait.h \
               $$PWD/wgtbgscreen/progressindicator.h \
               $$PWD/wgtdoorbox/wgtdoorbox.h \
               $$PWD/wgtparaitem/actparaitem_define.h \
               $$PWD/wgtparaitem/actparawidget.h \
               $$PWD/wgtparaitem/flickeritem.h \
               $$PWD/wgtparaitem/flowlayout.h \
               $$PWD/wgtparaitem/wgtequipmentoverview.h \
    $$PWD/wgtparaitem/wgtparaitem_sms.h \
               $$PWD/wgtparaitem/wgtparaitem_spds.h \
               $$PWD/wgttitlebar/wgttitlebar.h \
               $$PWD/wgtkeypad/wgtkeypad.h\
               $$PWD/wgtmenubar/wgtmenubar.h\
               $$PWD/wgtmenubar/wgtmenuitem.h\
               $$PWD/wgtmenubar_ext/wgtmenubar_ext.h\
               $$PWD/wgtmenubar_ext/wgtlistviewdelegate.h\
               $$PWD/wgtmenubar_ext/wgtlistviewmodel.h\
               $$PWD/wgtmenubar_ext/mscrollarea.h  \
               $$PWD/wgtmenubar_ext/wgtmenuitem_ext.h\
               $$PWD/wgtparaitem/wgtparaitem_ups.h \
               $$PWD/wgtparaitem/wgtparaitem_meter.h \
               $$PWD/wgtparaitem/wgtparaitem_ac.h \
               $$PWD/wgtparaitem/wgtparaitem_eleak.h \
               $$PWD/wgtparaitem/wgtparaitem_ths.h \
               $$PWD/wgtparaitem/actparaitem.h \
               $$PWD/wgtparaitem/actparaitem_combobox.h \
               $$PWD/wgtparaitem/actparaitem_lineedit.h \
               $$PWD/wgtparaitem/actparaitem_bit.h \
               $$PWD/wgtparaitem/actparaitem_btn.h \
               $$PWD/wgtparaitem/actbitoperation.h \
               $$PWD/wgtparaitem/wgtdatetime.h \
               $$PWD/wgtparaitem/wgtccombox.h \
               $$PWD/wgtparaitem/wgtCheckbox.h \
               $$PWD/wgtparaitem/wgtscrollbar.h \
               $$PWD/wgtparaitem/signalgraph.h \
               $$PWD/wgtchart/wgtrose.h \
               $$PWD/wgtchart/wgtdashboard.h \
               $$PWD/wgtchart/curvechart.h \
               $$PWD/wgtchart/CGaugeSemiCircle.h \
               $$PWD/wgtchart/CProgressWater.h \
               $$PWD/wgtchart/wgtdial.h \
               $$PWD/wgtmsgbox/wgtmsgbox_h.h \

SOURCES     += $$PWD/wgtbgscreen/wgtbgscreen.cpp \
               $$PWD/Buzzer/buzzer.cpp \
    $$PWD/wgtbgscreen/wgtbgrender.cpp \
               $$PWD/wgtbgscreen/wgtwait.cpp \
               $$PWD/wgtbgscreen/progressindicator.cpp \
               $$PWD/wgtdoorbox/wgtdoorbox.cpp \
               $$PWD/wgtparaitem/actparawidget.cpp \
               $$PWD/wgtparaitem/flickeritem.cpp \
    $$PWD/wgtparaitem/flowlayout.cpp \
               $$PWD/wgtparaitem/wgtequipmentoverview.cpp \
    $$PWD/wgtparaitem/wgtparaitem_sms.cpp \
			   $$PWD/wgtparaitem/wgtparaitem_spds.cpp \
               $$PWD/wgttitlebar/wgttitlebar.cpp \
               $$PWD/wgtkeypad/wgtkeypad.cpp\
               $$PWD/wgtmenubar/wgtmenubar.cpp \
               $$PWD/wgtmenubar/wgtmenuitem.cpp\
               $$PWD/wgtmenubar_ext/wgtmenubar_ext.cpp\
               $$PWD/wgtmenubar_ext/wgtlistviewdelegate.cpp\
               $$PWD/wgtmenubar_ext/wgtlistviewmodel.cpp\
               $$PWD/wgtmenubar_ext/mscrollarea.cpp  \
               $$PWD/wgtmenubar_ext/wgtmenuitem_ext.cpp\
               $$PWD/wgtparaitem/wgtparaitem_ups.cpp \
               $$PWD/wgtparaitem/wgtparaitem_meter.cpp \
               $$PWD/wgtparaitem/wgtparaitem_ac.cpp \
               $$PWD/wgtparaitem/wgtparaitem_eleak.cpp \
               $$PWD/wgtparaitem/wgtparaitem_ths.cpp \
               $$PWD/wgtparaitem/actparaitem.cpp \
               $$PWD/wgtparaitem/actparaitem_combobox.cpp \
               $$PWD/wgtparaitem/actparaitem_lineedit.cpp \
               $$PWD/wgtparaitem/actparaitem_bit.cpp \
               $$PWD/wgtparaitem/actparaitem_btn.cpp \
               $$PWD/wgtparaitem/actbitoperation.cpp \
               $$PWD/wgtparaitem/wgtdatetime.cpp\
               $$PWD/wgtparaitem/wgtccombox.cpp \
               $$PWD/wgtparaitem/wgtscrollbar.cpp \
               $$PWD/wgtparaitem/signalgraph.cpp \
               $$PWD/wgtchart/wgtrose.cpp \
               $$PWD/wgtchart/wgtdashboard.cpp \
               $$PWD/wgtchart/curvechart.cpp \
               $$PWD/wgtchart/CGaugeSemiCircle.cpp \
               $$PWD/wgtchart/CProgressWater.cpp \
               $$PWD/wgtchart/wgtdial.cpp \
               $$PWD/wgtmsgbox/wgtmsgbox_h.cpp \

FORMS       += $$PWD/wgttitlebar/wgttitlebar.ui \
			   $$PWD/wgtbgscreen/wgtbgrender.ui \
               $$PWD/wgtbgscreen/wgtwait.ui \
               $$PWD/wgtdoorbox/wgtdoorbox.ui \
               $$PWD/wgtparaitem/actparawidget.ui \
               $$PWD/wgtparaitem/wgtequipmentoverview.ui \
    $$PWD/wgtparaitem/wgtparaitem_sms.ui \
			   $$PWD/wgtparaitem/wgtparaitem_spds.ui \
               $$PWD/wgtparaitem/wgtparaitem_ac.ui \
               $$PWD/wgtparaitem/wgtparaitem_eleak.ui \
               $$PWD/wgtparaitem/wgtparaitem_meter.ui \
               $$PWD/wgtparaitem/wgtparaitem_ths.ui \
               $$PWD/wgtparaitem/wgtparaitem_ups.ui \
               $$PWD/wgtparaitem/actparaitem.ui \
               $$PWD/wgtparaitem/actparaitem_combobox.ui \
               $$PWD/wgtparaitem/actparaitem_lineedit.ui \
               $$PWD/wgtparaitem/actparaitem_bit.ui \
               $$PWD/wgtparaitem/actparaitem_btn.ui \
               $$PWD/wgtparaitem/actbitoperation.ui \
               $$PWD/wgtmsgbox/formwgtmsgbox_h.ui \









