#include "actparaitem_lineedit.h"
#include "ui_actparaitem_lineedit.h"
#include "wgtkeypad.h"
#include "wgtparaitem_ths.h"
#include "wgtparaitem_ac.h"
#include "wgtparaitem_eleak.h"
#include "gettext.h"
#include "formmain.h"
#include <QDebug>
#include <QDateTime>

ActParaItem_lineEdit::ActParaItem_lineEdit(DevTypeEnum devType, int devId, Menu menu, QWidget *parent)
    :QWidget(parent),
    ui(new Ui::ActParaItem_lineEdit),
    m_menu(menu),
    m_devtype(devType),
    m_devid(devId)
{
    //qRegisterMetaType<ths_set>("ths_set");

    ui->setupUi(this);

    lieVector.append(ui->lineParaVal);
    ui->lineParaVal->installEventFilter(this);

    m_name  = langN.show[m_menu.show_name];//m_menu.show_name;
    m_value = QString::number(0);

    ui->lblParaName->setText(m_name);
    ui->lineParaVal->setText(m_value);
}

ActParaItem_lineEdit::~ActParaItem_lineEdit()
{
    delete ui;
}

bool ActParaItem_lineEdit::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLineEdit"))
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj);
            connect(WgtKeyPad::Instance(), &WgtKeyPad::sendInputBufferAreaText, [=](QString text)
            {
                QRegExp regExp("[0-9.0-9]*");
                if (!regExp.exactMatch(text)) {
                    return;
                }

                if(lineEdit != 0)
                {
                    m_value = text;
                    lineEdit->setText(m_value);

                    //将数据写回共享内存
                    writeData();
                    return;
                }
            });

            foreach (QLineEdit *lie, lieVector) {
                if(lie == lineEdit)
                {
                    WgtKeyPad::Instance()->showInputBufferArea(lie->text());
                    WgtKeyPad::Instance()->show();
                    return  true;
                }
            }
        }
    }
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

void ActParaItem_lineEdit::setValue(QString val)
{
    m_value = val;
    ui->lineParaVal->setText(m_value);
}

void ActParaItem_lineEdit::setUnit(QString unit)
{
    if(unit != ""){
        QString str =m_name+ "("+unit+")";
        ui->lblParaName->setText(str);
    }

}

void ActParaItem_lineEdit::writeData(void)
{
    if(m_devtype == DevTypeEnum::Ths)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ths::ThsParaEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eThsParaNum))
            return;

        ths_set ths_val;
        ths_val.devNo = m_devid;
        ths_val.para  = (ThsParaEnum)index;
        ths_val.val   = m_value.toDouble();

        emit signal_ths(ths_val);
    }
    else if(m_devtype == DevTypeEnum::Ups)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ups::UpsCtlEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eUpsCtlNum))
            return;

        ups_set ups_val;
        ups_val.devNo = m_devid;
        ups_val.para  = (UpsCtlEnum)index;
        ups_val.val   = m_value.toInt();

        emit signal_ups(ups_val);
    }
    else if(m_devtype == DevTypeEnum::Ac)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ac::AcRunParaEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eAcParaNum))
            return;

        ac_set ac_val;
        ac_val.devNo = m_devid;
        if(m_menu.parameter_type == TYPE_PARA_SETTING){
            QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ac::AcRunParaEnum>();
            //把参数对应的值转换成枚举值
            int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
            if((-1 == index) && (index >= eAcParaNum))
                return;
            ac_val.union_ac.para  = (AcRunParaEnum)index;
            ac_val.paraType = TYPE_PARA_SETTING;
        }else if(m_menu.parameter_type == TYPE_STAT_SETTING) {
            QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ac::AcRunStatusEnum>();
            //把参数对应的值转换成枚举值
            int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
            if((-1 == index) && (index >= eAcStsNum))
                return;
            ac_val.union_ac.sta_para = (AcRunStatusEnum)index;
            ac_val.paraType = TYPE_STAT_SETTING;
        }
        ac_val.val   = m_value.toInt();

        emit signal_ac(ac_val);
    }
    else if(m_devtype == DevTypeEnum::Leak)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
        //把参数对应的值转换成枚举值
        int index   = metaEnum.keysToValue(m_menu.parameter_name.toLatin1().data());
        if((-1 == index) && (index >= eLeakParaNum))
            return;

        leak_set leak_val;
        leak_val.devNo = m_devid;
        leak_val.para  = (LeakParaEnum)index;
        leak_val.val   = m_value.toInt();

        emit signal_leak(leak_val);
    }
}

void ActParaItem_lineEdit::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}





